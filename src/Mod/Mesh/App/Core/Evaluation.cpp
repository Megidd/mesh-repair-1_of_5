/***************************************************************************
 *   Copyright (c) 2005 Imetric 3D GmbH                                    *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"

#ifndef _PreComp_
# include <algorithm>
# include <vector>
#endif

#include <Mod/Mesh/App/WildMagic4/Wm4Matrix3.h>
#include <Mod/Mesh/App/WildMagic4/Wm4Vector3.h>

#include "Evaluation.h"
#include "Iterator.h"
#include "Algorithm.h"
#include "Approximation.h"
#include "MeshIO.h"
#include "Helpers.h"
#include "Grid.h"
#include "TopoAlgorithm.h"
#include "Functional.h"
#include <Base/Matrix.h>

#include <Base/Sequencer.h>

using namespace MeshCore;

MeshOrientationVisitor::MeshOrientationVisitor() : _nonuniformOrientation(false)
{
}

MeshOrientationCollector::MeshOrientationCollector(std::vector<unsigned long>& aulIndices, std::vector<unsigned long>& aulComplement)
 : _aulIndices(aulIndices), _aulComplement(aulComplement)
{
}

MeshSameOrientationCollector::MeshSameOrientationCollector(std::vector<unsigned long>& aulIndices)
  : _aulIndices(aulIndices)
{
}

MeshEvalOrientation::MeshEvalOrientation (const MeshKernel& rclM)
  : MeshEvaluation( rclM )
{
}

MeshEvalOrientation::~MeshEvalOrientation()
{
}

unsigned long MeshEvalOrientation::HasFalsePositives(const std::vector<unsigned long>& inds) const
{
    // All faces with wrong orientation (i.e. adjacent faces with a normal flip and their neighbours)
    // build a segment and are marked as TMP0. Now we check all border faces of the segments with 
    // their correct neighbours if there was really a normal flip. If there is no normal flip we have
    // a false positive.
    // False-positives can occur if the mesh structure has some defects which let the region-grow
    // algorithm fail to detect the faces with wrong orientation.
    const MeshFacetArray& rFAry = _rclMesh.GetFacets();
    MeshFacetArray::_TConstIterator iBeg = rFAry.begin();
    for (std::vector<unsigned long>::const_iterator it = inds.begin(); it != inds.end(); ++it) {
        const MeshFacet& f = iBeg[*it];
        for (int i = 0; i < 3; i++) {
            if (f._aulNeighbours[i] != ULONG_MAX) {
                const MeshFacet& n = iBeg[f._aulNeighbours[i]];
                if (f.IsFlag(MeshFacet::TMP0) && !n.IsFlag(MeshFacet::TMP0)) {
                    for (int j = 0; j < 3; j++) {
                        if (f.HasSameOrientation(n)) {
                            // adjacent face with same orientation => false positive
                            return f._aulNeighbours[i];
                        }
                    }
                }
            }
        }
    }

    return ULONG_MAX;
}


std::vector<unsigned long> MeshEvalOrientation::GetIndices() const
{
    unsigned long ulStartFacet, ulVisited;

    if (_rclMesh.CountFacets() == 0)
        return std::vector<unsigned long>();

    // reset VISIT flags
    MeshAlgorithm cAlg(_rclMesh);
    cAlg.ResetFacetFlag(MeshFacet::VISIT);
    cAlg.ResetFacetFlag(MeshFacet::TMP0);

    const MeshFacetArray& rFAry = _rclMesh.GetFacets();
    MeshFacetArray::_TConstIterator iTri = rFAry.begin();
    MeshFacetArray::_TConstIterator iBeg = rFAry.begin();
    MeshFacetArray::_TConstIterator iEnd = rFAry.end();

    ulStartFacet = 0;

    std::vector<unsigned long> uIndices, uComplement;
    MeshOrientationCollector clHarmonizer(uIndices, uComplement);

    while (ulStartFacet !=  ULONG_MAX) { 
        unsigned long wrongFacets = uIndices.size();

        uComplement.clear();
        uComplement.push_back( ulStartFacet );
        ulVisited = _rclMesh.VisitNeighbourFacets(clHarmonizer, ulStartFacet) + 1;

        // In the currently visited component we have found less than 40% as correct
        // oriented and the rest as false oriented. So, we decide that it should be the other
        // way round and swap the indices of this component.
        if (uComplement.size() < static_cast<unsigned long>(0.4f*static_cast<float>(ulVisited))) {
            uIndices.erase(uIndices.begin()+wrongFacets, uIndices.end());
            uIndices.insert(uIndices.end(), uComplement.begin(), uComplement.end());
        }

        // if the mesh consists of several topologic independent components
        // We can search from position 'iTri' on because all elements _before_ are already visited
        // what we know from the previous iteration.
        MeshIsNotFlag<MeshFacet> flag;
        iTri = std::find_if(iTri, iEnd, [flag](const MeshFacet& f) {
            return flag(f, MeshFacet::VISIT);
        });

        if (iTri < iEnd)
            ulStartFacet = iTri - iBeg;
        else
            ulStartFacet = ULONG_MAX;
    }

    // in some very rare cases where we have some strange artifacts in the mesh structure
    // we get false-positives. If we find some we check all 'invalid' faces again
    cAlg.ResetFacetFlag(MeshFacet::TMP0);
    cAlg.SetFacetsFlag(uIndices, MeshFacet::TMP0);
    ulStartFacet = HasFalsePositives(uIndices);
    while (ulStartFacet != ULONG_MAX) {
        cAlg.ResetFacetsFlag(uIndices, MeshFacet::VISIT);
        std::vector<unsigned long> falsePos;
        MeshSameOrientationCollector coll(falsePos);
        _rclMesh.VisitNeighbourFacets(coll, ulStartFacet);

        std::sort(uIndices.begin(), uIndices.end());
        std::sort(falsePos.begin(), falsePos.end());

        std::vector<unsigned long> diff;
        std::back_insert_iterator<std::vector<unsigned long> > biit(diff);
        std::set_difference(uIndices.begin(), uIndices.end(), falsePos.begin(), falsePos.end(), biit);
        uIndices = diff;

        cAlg.ResetFacetFlag(MeshFacet::TMP0);
        cAlg.SetFacetsFlag(uIndices, MeshFacet::TMP0);
        unsigned long current = ulStartFacet;
        ulStartFacet = HasFalsePositives(uIndices);
        if (current == ulStartFacet)
            break; // avoid an endless loop
    }

    return uIndices;
}
