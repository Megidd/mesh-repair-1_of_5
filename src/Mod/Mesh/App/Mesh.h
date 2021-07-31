/***************************************************************************
 *   Copyright (c) Jürgen Riegel <juergen.riegel@web.de>                   *
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


#ifndef MESH_MESH_H
#define MESH_MESH_H

#include <vector>
#include <list>
#include <set>
#include <string>
#include <map>

#include <Base/Matrix.h>
#include <Base/Vector3D.h>

#include <App/PropertyStandard.h>
#include <App/PropertyGeo.h>
#include <App/ComplexGeoData.h>

#include "Core/MeshKernel.h"
#include "Core/MeshIO.h"
#include "Core/Iterator.h"
#include "MeshPoint.h"
#include "Facet.h"
#include "MeshPoint.h"
#include "Segment.h"

namespace Py {
class List;
}

namespace Base {
  class Polygon2d;
  class ViewProjMethod;
}

namespace MeshCore {
class AbstractPolygonTriangulator;
}

namespace Mesh
{
/**
 * The MeshObject class provides an interface for the underlying MeshKernel class and
 * most of its algorithm on it.
 * @note Each instance of MeshObject has its own instance of a MeshKernel so it's not possible
 * that several instances of MeshObject manage one instance of MeshKernel.
 */
class MeshExport MeshObject : public Data::ComplexGeoData
{
    TYPESYSTEM_HEADER();

public:


    MeshObject();
    explicit MeshObject(const MeshCore::MeshKernel& Kernel);
    explicit MeshObject(const MeshCore::MeshKernel& Kernel, const Base::Matrix4D &Mtrx);
    MeshObject(const MeshObject&);
    virtual ~MeshObject();

   
    void harmonizeNormals();

private:
    MeshCore::MeshKernel _kernel;
};

} // namespace Mesh


#endif // MESH_MESH_H
