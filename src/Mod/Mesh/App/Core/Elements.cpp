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
#endif

#include <Mod/Mesh/App/WildMagic4/Wm4IntrSegment3Plane3.h>
#include <Mod/Mesh/App/WildMagic4/Wm4IntrSegment3Box3.h>
#include <Mod/Mesh/App/WildMagic4/Wm4DistVector3Triangle3.h>
#include <Mod/Mesh/App/WildMagic4/Wm4DistSegment3Triangle3.h>

#include "Elements.h"
#include "Algorithm.h"
#include "tritritest.h"
#include "Utilities.h"

using namespace MeshCore;

MeshPointArray::MeshPointArray(const MeshPointArray& ary)
  : TMeshPointArray(ary)
{
}

MeshFacetArray::MeshFacetArray(const MeshFacetArray& ary)
  : TMeshFacetArray(ary)
{
}

void MeshFacetArray::ResetFlag (MeshFacet::TFlagType tF) const
{
  for (MeshFacetArray::_TConstIterator i = begin(); i < end(); ++i) i->ResetFlag(tF);
}