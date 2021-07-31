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


#ifndef MESH_KERNEL_H
#define MESH_KERNEL_H

#include <assert.h>
#include <iosfwd>

#include "Elements.h"
#include "Helpers.h"

#include <Base/BoundBox.h>
#include <Base/Vector3D.h>
#include <Base/Matrix.h>

namespace Base{
  class Polygon2d;
  class ViewProjMethod;
}

namespace MeshCore {

// forward declarations
class MeshFacetIterator;
class MeshPointIterator;
class MeshGeomFacet;
class MeshFacet;
class MeshFacetVisitor;
class MeshPointVisitor;
class MeshFacetGrid;


/** 
 * The MeshKernel class is the basic class that holds the data points,
 * the edges and the facets describing a mesh object.
 * 
 * The bounding box is calculated during the buildup of the data
 * structure and gets only re-caclulated after insertion of new facets
 * but not after removal of facets.
 *
 * This class provides only some rudimental querying methods.
 */
class MeshExport MeshKernel
{
public:
    /// Construction
    MeshKernel (void);
    /// Construction
    MeshKernel (const MeshKernel &rclMesh);
    /// Destruction
    ~MeshKernel (void)
    { Clear(); }

    /** Returns true if the data structure is valid. */
    bool IsValid (void) const
    { return _bValid; }

    /** @name I/O methods */
    //@{
    /// Binary streaming of data
    void Write (std::ostream &rclOut) const;
    void Read (std::istream &rclIn);
    //@}

    /// Returns the number of facets
    unsigned long CountFacets (void) const
    { return static_cast<unsigned long>(_aclFacetArray.size()); }

    /**
     * This method visits all neighbour facets, i.e facets that share a common edge 
     * starting from the facet associated to index \a ulStartFacet. All facets having set the VISIT 
     * flag are ignored. Therefore the user have to set or unset this flag if needed.
     * All facets that get visited during this algorithm are marked as VISIT and the Visit() method
     * of the given MeshFacetVisitor gets invoked. 
     * If there are no unvisited neighbours any more the algorithms returns immediately and returns 
     * the number of visited facets.
     * \note For the start facet \a ulStartFacet MeshFacetVisitor::Visit() does not get invoked though
     * the facet gets marked as VISIT.
     */
    unsigned long VisitNeighbourFacets (MeshFacetVisitor &rclFVisitor, unsigned long ulStartFacet) const;
    
    /** Removes all as INVALID marked points and facets from the structure. */
    void RemoveInvalids ();
    /** Clears the whole data structure. */
    void Clear (void);
        /** Returns the array of all facets */
    const MeshFacetArray& GetFacets (void) const { return _aclFacetArray; }
    /** Returns an array of facets to the given indices. The indices
     * must not be out of range.
     */
    MeshFacetArray GetFacets(const std::vector<unsigned long>&) const;


protected:

    MeshPointArray   _aclPointArray; /**< Holds the array of geometric points. */
    MeshFacetArray   _aclFacetArray; /**< Holds the array of facets. */
    Base::BoundBox3f _clBoundBox;    /**< The current calculated bounding box. */
    bool            _bValid; /**< Current state of validality. */

    // friends
    friend class MeshAlgorithm;
    friend class MeshTopoAlgorithm;

};



} // namespace MeshCore

#endif // MESH_KERNEL_H 
