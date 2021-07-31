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


#ifndef VISITOR_H
#define VISITOR_H

namespace MeshCore {

class MeshFacetVisitor;


/**
 * Abstract base class for facet visitors. 
 * The MeshFacetVisitor class can be used for the so called
 * "Region growing" algorithms.
 */
class MeshExport MeshFacetVisitor
{
public:
    /// Construction 
    MeshFacetVisitor(void) { }
    /// Denstruction 
    virtual ~MeshFacetVisitor(void) { }
    /** Needs to be implemented in sub-classes.
     * \a rclFacet is the currently visited facet with the index \a ulFInd, \a rclFrom
     * is the last visited facet and \a ulLevel indicates the ring number around the start facet. 
     * If \a true is returned the next iteration is done if there are still facets to visit.
     * If \a false is returned the calling method stops immediately visiting further facets.
     */
  
};


} // namespace MeshCore

#endif // VISITOR_H 

