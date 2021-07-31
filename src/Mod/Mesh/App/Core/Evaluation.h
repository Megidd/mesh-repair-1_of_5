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


#ifndef MESH_EVALUATION_H
#define MESH_EVALUATION_H

#include <list>
#include <cmath>

#include "MeshKernel.h"
#include "Visitor.h"

namespace MeshCore {

/**
 * The MeshEvaluation class checks the mesh kernel for correctness with respect to a
 * certain criterion, such as manifoldness, self-intersections, etc.
 * The passed mesh kernel is read-only and cannot be modified.
 * @see MeshEvalTopology
 * @see MeshEvalGeometry
 * The class itself is abstract, hence the method Evaluate() must be implemented 
 * by subclasses.
 */
class MeshExport MeshEvaluation
{
public:
  MeshEvaluation (const MeshKernel &rclB) : _rclMesh(rclB) {}
  virtual ~MeshEvaluation () {}

protected:
  const MeshKernel& _rclMesh; /**< Mesh kernel */
};

/**
 * This class searches for nonuniform orientation of neighboured facets.
 * @author Werner Mayer
 */
class MeshExport MeshOrientationVisitor : public MeshFacetVisitor
{
public:
    MeshOrientationVisitor();

   

private:
    bool _nonuniformOrientation;
};

/**
 * This class searches for inconsistent orientation of neighboured facets.
 * Note: The 'TMP0' flag for facets must be reset before using this class.
 * @author Werner Mayer
 */
class MeshExport MeshOrientationCollector : public MeshOrientationVisitor
{
public:
    MeshOrientationCollector(std::vector<unsigned long>& aulIndices,
                             std::vector<unsigned long>& aulComplement);


private:
    std::vector<unsigned long>& _aulIndices;
    std::vector<unsigned long>& _aulComplement;
};

/**
 * @author Werner Mayer
 */
class MeshExport MeshSameOrientationCollector : public MeshOrientationVisitor
{
public:
    MeshSameOrientationCollector(std::vector<unsigned long>& aulIndices);
  

private:
    std::vector<unsigned long>& _aulIndices;
};

/**
 * The MeshEvalOrientation class checks the mesh kernel for consistent facet normals.
 * @author Werner Mayer
 */
class MeshExport MeshEvalOrientation : public MeshEvaluation
{
public:
    MeshEvalOrientation (const MeshKernel& rclM);
    ~MeshEvalOrientation();
    std::vector<unsigned long> GetIndices() const;

private:
    unsigned long HasFalsePositives(const std::vector<unsigned long>&) const;
};


} // namespace MeshCore

#endif // MESH_EVALUATION_H
