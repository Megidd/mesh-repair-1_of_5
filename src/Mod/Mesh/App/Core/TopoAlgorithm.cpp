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
# include <utility>
# include <queue>
#endif

#include <Mod/Mesh/App/WildMagic4/Wm4MeshCurvature.h>
#include <Mod/Mesh/App/WildMagic4/Wm4Vector3.h>

#include "TopoAlgorithm.h"
#include "Iterator.h"
#include "MeshKernel.h"
#include "Algorithm.h"
#include "Evaluation.h"
#include "Triangulation.h"
#include "Definitions.h"
#include <Base/Console.h>

using namespace MeshCore;

MeshTopoAlgorithm::MeshTopoAlgorithm (MeshKernel &rclM)
: _rclMesh(rclM), _needsCleanup(false), _cache(0)
{
}

MeshTopoAlgorithm::~MeshTopoAlgorithm (void)
{
  if ( _needsCleanup )
    Cleanup();
  EndCache();
}


void MeshTopoAlgorithm::Cleanup()
{
    _rclMesh.RemoveInvalids();
    _needsCleanup = false;
}

void MeshTopoAlgorithm::EndCache()
{
    if (_cache) {
        _cache->clear();
        delete _cache;
        _cache = 0;
    }
}


void MeshTopoAlgorithm::HarmonizeNormals (void)
{
  std::vector<unsigned long> uIndices = MeshEvalOrientation(_rclMesh).GetIndices();
  for ( std::vector<unsigned long>::iterator it = uIndices.begin(); it != uIndices.end(); ++it )
    _rclMesh._aclFacetArray[*it].FlipNormal();
}
