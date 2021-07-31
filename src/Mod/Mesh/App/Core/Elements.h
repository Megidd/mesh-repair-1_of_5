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


#ifndef MESH_ELEMENTS_H
#define MESH_ELEMENTS_H

#include <functional>
#include <vector>
#include <climits>
#include <cstring>

#include "Definitions.h"

#include <Base/BoundBox.h>
#include <Base/Vector3D.h>
#include <Base/Matrix.h>

// Cannot use namespace Base in constructors of MeshPoint
#ifdef _MSC_VER
#endif

namespace MeshCore {

/**
 * The MeshFacet class represent a triangle facet in the mesh data.structure. A facet indexes
 * three neighbour facets and also three corner points.
 * This class only keeps topologic information but no geometric information at all.
 * 
 * Here are the most important conventions concerning the facet's orientation:
 * \li neighbour or edge number of 0 is defined by corner 0 and 1
 * \li neighbour or edge number of 1 is defined by corner 1 and 2
 * \li neighbour or edge number of 2 is defined by corner 2 and 0
 * \li neighbour index is set to ULONG_MAX if there is no neighbour facet
 *
 * Note: The status flag SEGMENT mark a facet to be part of certain subset, a segment.
 * This flag must not be set by any algorithm unless it adds or removes facets to a segment.
 *
 * Note: The status flag SELECTED mark a facet to be selected which is e.g. used in the GUI.
 * This flag must not be set by any algorithm unless it adds or removes facets to the selection.
 */
class MeshFacet
{
public:
  enum TFlagType {INVALID=1, VISIT=2, SEGMENT=4, MARKED=8, SELECTED=16, REV=32, TMP0=64, TMP1=128};

public:
  /** @name Construction */
  //@{
  inline MeshFacet (void);
  inline MeshFacet(const MeshFacet &rclF);
  inline MeshFacet(unsigned long p1,unsigned long p2,unsigned long p3,unsigned long n1=ULONG_MAX,unsigned long n2=ULONG_MAX,unsigned long n3=ULONG_MAX);
  ~MeshFacet (void) { }
  //@}

  void ResetFlag (TFlagType tF) const
  { const_cast<MeshFacet*>(this)->_ucFlag &= ~static_cast<unsigned char>(tF); }
  
   void FlipNormal (void)
  {
    std::swap(_aulPoints[1], _aulPoints[2]);
    std::swap(_aulNeighbours[0], _aulNeighbours[2]);
  }

  public:
  unsigned long _aulPoints[3];     /**< Indices of corner points. */
  unsigned long _aulNeighbours[3]; /**< Indices of neighbour facets. */
};

typedef  std::vector<MeshPoint>  TMeshPointArray;
/**
 * Stores all data points of the mesh structure.
 */
class MeshExport MeshPointArray: public TMeshPointArray
{
public:
  // Iterator interface
  typedef std::vector<MeshPoint>::iterator        _TIterator;
  typedef std::vector<MeshPoint>::const_iterator  _TConstIterator;

  /** @name Construction */
  //@{
  // constructor
  MeshPointArray (void) { }
  // constructor
  MeshPointArray (unsigned long ulSize) : TMeshPointArray(ulSize) { }
  /// copy-constructor
  MeshPointArray (const MeshPointArray&);
  // Destructor
  ~MeshPointArray (void) { }
  //@}

};



typedef std::vector<MeshFacet>  TMeshFacetArray;

/**
 * Stores all facets of the mesh data-structure.
 */
class MeshExport MeshFacetArray: public TMeshFacetArray
{
public:
    // Iterator interface
    typedef std::vector<MeshFacet>::iterator        _TIterator;
    typedef std::vector<MeshFacet>::const_iterator  _TConstIterator;

    /** @name Construction */
    //@{
    /// constructor
    MeshFacetArray (void) { }
    /// constructor
    MeshFacetArray (unsigned long ulSize) : TMeshFacetArray(ulSize) { }
    /// copy-constructor
    MeshFacetArray (const MeshFacetArray&);
    /// destructor
    ~MeshFacetArray (void) { }
    //@}

    /// Resets the flag for all facets. 
    void ResetFlag (MeshFacet::TFlagType tF) const;
};

/**
 * Binary function to query the flags for use with generic STL functions.
 */
template <class TCLASS>
class MeshIsNotFlag
{
public:
    typedef TCLASS first_argument_type;
    typedef typename TCLASS::TFlagType second_argument_type;
    typedef bool result_type;
    bool operator () (const TCLASS& rclElem, typename TCLASS::TFlagType tFlag) const
    { return !rclElem.IsFlag(tFlag); }
};

} // namespace MeshCore

#endif // MESH_ELEMENTS_H 
