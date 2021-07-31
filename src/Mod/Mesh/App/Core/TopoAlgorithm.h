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


#ifndef MESH_TOPOALGORITHM_H
#define MESH_TOPOALGORITHM_H

#include <map>
#include <vector>

#include "Definitions.h"
#include "Iterator.h"
#include "MeshKernel.h"
#include "Elements.h"
#include "Visitor.h"
#include "Algorithm.h"

#include <Base/Vector3D.h>
#include <Base/Sequencer.h>

namespace MeshCore {


/**
 * The MeshTopoAlgorithm class provides several algorithms to manipulate a mesh.
 * It supports various mesh operations like inserting a new vertex, swapping the
 * common edge of two adjacent facets, split a facet, ...
 * @author Werner Mayer
 */
class MeshExport MeshTopoAlgorithm
{   
public:
    // construction/destruction
    MeshTopoAlgorithm (MeshKernel &rclM);
    virtual ~MeshTopoAlgorithm (void);

public:

    /**
     * Removes all invalid marked elements from the mesh structure.
     */
    void Cleanup();
   
    /**
     * Harmonizes the normals.
     */
    void HarmonizeNormals (void);
   
    /**
     * Caching facility.
     */
    void BeginCache();


    
private:
    MeshKernel& _rclMesh;
    bool _needsCleanup;

   // cache
    typedef std::map<Base::Vector3f,unsigned long,Vertex_Less> tCache;
    tCache* _cache;
};


} // namespace MeshCore

#endif // MESH_TOPOALGORITHM_H
