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
#endif

#include "Algorithm.h"
#include "Approximation.h"
#include "Elements.h"
#include "Iterator.h"
#include "Grid.h"
#include "Triangulation.h"

#include <Base/Console.h>
#include <Base/Sequencer.h>

using namespace MeshCore;


void MeshAlgorithm::SetFacetsFlag (const std::vector<unsigned long> &raulInds, MeshFacet::TFlagType tF) const
{
    for (std::vector<unsigned long>::const_iterator i = raulInds.begin(); i != raulInds.end(); ++i)
        _rclMesh._aclFacetArray[*i].SetFlag(tF);
}

void MeshAlgorithm::ResetFacetsFlag (const std::vector<unsigned long> &raulInds, MeshFacet::TFlagType tF) const
{
    for (std::vector<unsigned long>::const_iterator i = raulInds.begin(); i != raulInds.end(); ++i)
        _rclMesh._aclFacetArray[*i].ResetFlag(tF);
}

void MeshAlgorithm::ResetFacetFlag (MeshFacet::TFlagType tF) const
{
    _rclMesh._aclFacetArray.ResetFlag(tF);
}
