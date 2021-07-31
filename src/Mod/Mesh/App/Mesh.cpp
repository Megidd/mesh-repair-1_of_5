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


#include "PreCompiled.h"
#ifndef _PreComp_
# include <algorithm>
# include <sstream>
#endif

#include <CXX/Objects.hxx>
#include <Base/Builder3D.h>
#include <Base/Console.h>
#include <Base/Exception.h>
#include <Base/Writer.h>
#include <Base/Reader.h>
#include <Base/Interpreter.h>
#include <Base/Sequencer.h>
#include <Base/Tools.h>
#include <Base/ViewProj.h>

#include "Core/Builder.h"
#include "Core/MeshKernel.h"
#include "Core/Grid.h"
#include "Core/Iterator.h"
#include "Core/Info.h"
#include "Core/TopoAlgorithm.h"
#include "Core/Evaluation.h"
#include "Core/Degeneration.h"
#include "Core/Segmentation.h"
#include "Core/SetOperations.h"
#include "Core/Triangulation.h"
#include "Core/Trim.h"
#include "Core/TrimByPlane.h"
#include "Core/Visitor.h"
#include "Core/Decimation.h"

#include "Mesh.h"
#include "MeshPy.h"

using namespace Mesh;


TYPESYSTEM_SOURCE(Mesh::MeshObject, Data::ComplexGeoData)

MeshObject::MeshObject()
{
}

MeshObject::MeshObject(const MeshCore::MeshKernel& Kernel)
  : _kernel(Kernel)
{
    // copy the mesh structure
}

MeshObject::MeshObject(const MeshCore::MeshKernel& Kernel, const Base::Matrix4D &Mtrx)
  : _Mtrx(Mtrx),_kernel(Kernel)
{
    // copy the mesh structure
}

MeshObject::MeshObject(const MeshObject& mesh)
  : _Mtrx(mesh._Mtrx),_kernel(mesh._kernel)
{
    // copy the mesh structure
    copySegments(mesh);
}

MeshObject::~MeshObject()
{
}

void MeshObject::harmonizeNormals()
{
    MeshCore::MeshTopoAlgorithm alg(_kernel);
    alg.HarmonizeNormals();
}
