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


#ifndef MESHALGORITHM_H
#define MESHALGORITHM_H

#include <set>
#include <vector>
#include <map>

#include "MeshKernel.h"
#include "Elements.h"
#include <Base/Vector3D.h>


namespace MeshCore {

/**
 * The MeshAlgorithm class provides algorithms base on meshes.
 */
class MeshExport MeshAlgorithm
{
public:
  /// Construction
  MeshAlgorithm (const MeshKernel &rclM) : _rclMesh(rclM) { }
  /// Destruction
  ~MeshAlgorithm (void) { }

public:
    /** Resets of all facets the flag \a tF. */
    void ResetFacetFlag (MeshFacet::TFlagType tF) const;
    /** Resets from all facets in \a raulInds the flag \a tF. */
    void ResetFacetsFlag (const std::vector<unsigned long> &raulInds, MeshFacet::TFlagType tF) const;
    /** Resets from all facets in \a raulInds the flag \a tF. */
    void ResetFacetsFlag (const std::vector<unsigned long> &raulInds, MeshFacet::TFlagType tF) const;
  
protected:
  const MeshKernel      &_rclMesh; /**< The mesh kernel. */
};

} // namespace MeshCore 

#endif  // MESH_ALGORITHM_H 
