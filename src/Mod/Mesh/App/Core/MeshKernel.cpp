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
# include <stdexcept>
# include <map>
# include <queue>
#endif

#include <Base/Exception.h>
#include <Base/Sequencer.h>
#include <Base/Stream.h>
#include <Base/Swap.h>

#include "Algorithm.h"
#include "Approximation.h"
#include "Helpers.h"
#include "MeshKernel.h"
#include "Iterator.h"
#include "Evaluation.h"
#include "Builder.h"
#include "Smoothing.h"
#include "MeshIO.h"

using namespace MeshCore;

MeshKernel::MeshKernel (void)
: _bValid(true)
{
    _clBoundBox.SetVoid();
}

MeshKernel::MeshKernel (const MeshKernel &rclMesh)
{
    *this = rclMesh;    
}

void MeshKernel::Clear (void)
{
    _aclPointArray.clear();
    _aclFacetArray.clear();

    // release memory
    MeshPointArray().swap(_aclPointArray);
    MeshFacetArray().swap(_aclFacetArray);

    _clBoundBox.SetVoid();
}


void MeshKernel::RemoveInvalids ()
{
    std::vector<unsigned long> aulDecrements;
    std::vector<unsigned long>::iterator pDIter;
    unsigned long ulDec, i, k;
    MeshPointArray::_TIterator pPIter, pPEnd;
    MeshFacetArray::_TIterator pFIter, pFEnd;

    // generate array of decrements
    aulDecrements.resize(_aclPointArray.size());
    pDIter = aulDecrements.begin();
    ulDec  = 0;
    pPEnd  = _aclPointArray.end();
    for (pPIter = _aclPointArray.begin(); pPIter != pPEnd; ++pPIter) {
        *pDIter++ = ulDec;
        if (pPIter->IsValid() == false)
            ulDec++;
    }

    // correct point indices of the facets
    pFEnd  = _aclFacetArray.end();
    for (pFIter = _aclFacetArray.begin(); pFIter != pFEnd; ++pFIter) {
        if (pFIter->IsValid() == true) {
            pFIter->_aulPoints[0] -= aulDecrements[pFIter->_aulPoints[0]];
            pFIter->_aulPoints[1] -= aulDecrements[pFIter->_aulPoints[1]];
            pFIter->_aulPoints[2] -= aulDecrements[pFIter->_aulPoints[2]];
        }
    }

    // delete point, number of valid points
    unsigned long ulNewPts = std::count_if(_aclPointArray.begin(), _aclPointArray.end(),
                                           [](const MeshPoint& p) { return p.IsValid(); });
    // tmp. point array
    MeshPointArray  aclTempPt(ulNewPts);
    MeshPointArray::_TIterator pPTemp = aclTempPt.begin();
    pPEnd = _aclPointArray.end();
    for (pPIter = _aclPointArray.begin(); pPIter != pPEnd; ++pPIter) {
        if (pPIter->IsValid() == true)
            *pPTemp++ = *pPIter;
    }

    // free memory
    //_aclPointArray = aclTempPt;
    //aclTempPt.clear();
    _aclPointArray.swap(aclTempPt);
    MeshPointArray().swap(aclTempPt);

    // generate array of facet decrements
    aulDecrements.resize(_aclFacetArray.size());
    pDIter = aulDecrements.begin();
    ulDec  = 0;
    pFEnd  = _aclFacetArray.end();
    for (pFIter = _aclFacetArray.begin(); pFIter != pFEnd; ++pFIter, ++pDIter) {
        *pDIter = ulDec;
        if (pFIter->IsValid() == false)
            ulDec++;
    }

    // correct neighbour indices of the facets
    pFEnd = _aclFacetArray.end();
    for (pFIter = _aclFacetArray.begin(); pFIter != pFEnd; ++pFIter) {
        if (pFIter->IsValid() == true) {
            for (i = 0; i < 3; i++) {
                k = pFIter->_aulNeighbours[i];
                if (k != ULONG_MAX) {
                    if (_aclFacetArray[k].IsValid() == true)
                        pFIter->_aulNeighbours[i] -= aulDecrements[k];
                    else
                        pFIter->_aulNeighbours[i] = ULONG_MAX;
                }
            }
        }
    }

    // delete facets, number of valid facets
    unsigned long ulDelFacets = std::count_if(_aclFacetArray.begin(), _aclFacetArray.end(),
                                              [](const MeshFacet& f) { return f.IsValid(); });
    MeshFacetArray aclFArray(ulDelFacets);
    MeshFacetArray::_TIterator pFTemp = aclFArray.begin();  
    pFEnd  = _aclFacetArray.end();
    for (pFIter = _aclFacetArray.begin(); pFIter != pFEnd; ++pFIter) {
        if (pFIter->IsValid() == true)
            *pFTemp++ = *pFIter;
    }

    // free memory
    //_aclFacetArray = aclFArray;
    _aclFacetArray.swap(aclFArray);
}


// void MeshKernel::Write (std::ostream &rclOut) const 
// {
//     if (!rclOut || rclOut.bad())
//         return;

//     Base::OutputStream str(rclOut);

//     // Write a header with a "magic number" and a version
//     str << static_cast<uint32_t>(0xA0B0C0D0);
//     str << static_cast<uint32_t>(0x010000);

//     char szInfo[257]; // needs an additional byte for zero-termination
//     strcpy(szInfo, "MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-"
//                    "MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-"
//                    "MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-MESH-"
//                    "MESH-MESH-MESH-\n");
//     rclOut.write(szInfo, 256);

//     // write the number of points and facets
//     str << static_cast<uint32_t>(CountPoints()) << static_cast<uint32_t>(CountFacets());

//     // write the data
//     for (MeshPointArray::_TConstIterator it = _aclPointArray.begin(); it != _aclPointArray.end(); ++it) {
//         str << it->x << it->y << it->z;
//     }

//     for (MeshFacetArray::_TConstIterator it = _aclFacetArray.begin(); it != _aclFacetArray.end(); ++it) {
//         str << static_cast<uint32_t>(it->_aulPoints[0])
//             << static_cast<uint32_t>(it->_aulPoints[1])
//             << static_cast<uint32_t>(it->_aulPoints[2]);
//         str << static_cast<uint32_t>(it->_aulNeighbours[0])
//             << static_cast<uint32_t>(it->_aulNeighbours[1])
//             << static_cast<uint32_t>(it->_aulNeighbours[2]);
//     }

//     str << _clBoundBox.MinX << _clBoundBox.MaxX;
//     str << _clBoundBox.MinY << _clBoundBox.MaxY;
//     str << _clBoundBox.MinZ << _clBoundBox.MaxZ;
// }


// void MeshKernel::Read (std::istream &rclIn)
// {
//     if (!rclIn || rclIn.bad())
//         return;

//     // get header
//     Base::InputStream str(rclIn);

//     // Read the header with a "magic number" and a version
//     uint32_t magic, version, swap_magic, swap_version;
//     str >> magic >> version;
//     swap_magic = magic; Base::SwapEndian(swap_magic);
//     swap_version = version; Base::SwapEndian(swap_version);
//     uint32_t open_edge = 0xffffffff; // value to mark an open edge

//     // is it the new or old format?
//     bool new_format = false;
//     if (magic == 0xA0B0C0D0 && version == 0x010000) {
//         new_format = true;
//     }
//     else if (swap_magic == 0xA0B0C0D0 && swap_version == 0x010000) {
//         new_format = true;
//         str.setByteOrder(Base::Stream::BigEndian);
//     }

//     if (new_format) {
//         char szInfo[256];
//         rclIn.read(szInfo, 256);

//         // read the number of points and facets
//         uint32_t uCtPts=0, uCtFts=0;
//         str >> uCtPts >> uCtFts;

//         try {
//             // read the data
//             MeshPointArray pointArray;
//             pointArray.resize(uCtPts);
//             for (MeshPointArray::_TIterator it = pointArray.begin(); it != pointArray.end(); ++it) {
//                 str >> it->x >> it->y >> it->z;
//             }
          
//             MeshFacetArray facetArray;
//             facetArray.resize(uCtFts);

//             uint32_t v1, v2, v3;
//             for (MeshFacetArray::_TIterator it = facetArray.begin(); it != facetArray.end(); ++it) {
//                 str >> v1 >> v2 >> v3;

//                 // make sure to have valid indices
//                 if (v1 >= uCtPts || v2 >= uCtPts || v3 >= uCtPts)
//                     throw Base::BadFormatError("Invalid data structure");

//                 it->_aulPoints[0] = v1;
//                 it->_aulPoints[1] = v2;
//                 it->_aulPoints[2] = v3;

//                 // On systems where an 'unsigned long' is a 64-bit value
//                 // the empty neighbour must be explicitly set to 'ULONG_MAX'
//                 // because in algorithms this value is always used to check
//                 // for open edges.
//                 str >> v1 >> v2 >> v3;

//                 // make sure to have valid indices
//                 if (v1 >= uCtFts && v1 < open_edge)
//                     throw Base::BadFormatError("Invalid data structure");
//                 if (v2 >= uCtFts && v2 < open_edge)
//                     throw Base::BadFormatError("Invalid data structure");
//                 if (v3 >= uCtFts && v3 < open_edge)
//                     throw Base::BadFormatError("Invalid data structure");

//                 if (v1 < open_edge)
//                     it->_aulNeighbours[0] = v1;
//                 else
//                     it->_aulNeighbours[0] = ULONG_MAX;

//                 if (v2 < open_edge)
//                     it->_aulNeighbours[1] = v2;
//                 else
//                     it->_aulNeighbours[1] = ULONG_MAX;

//                 if (v3 < open_edge)
//                     it->_aulNeighbours[2] = v3;
//                 else
//                     it->_aulNeighbours[2] = ULONG_MAX;
//             }

//             str >> _clBoundBox.MinX >> _clBoundBox.MaxX;
//             str >> _clBoundBox.MinY >> _clBoundBox.MaxY;
//             str >> _clBoundBox.MinZ >> _clBoundBox.MaxZ;

//             // If we reach this block no exception occurred and we can safely assign the mesh
//             _aclPointArray.swap(pointArray);
//             _aclFacetArray.swap(facetArray);
//         }
//         catch (std::exception&) {
//             // Special handling of std::length_error
//             throw Base::BadFormatError("Reading from stream failed");
//         }
//     }
//     else {
//         // The old formats
//         unsigned long uCtPts=magic, uCtFts=version;
//         MeshPointArray pointArray;
//         MeshFacetArray facetArray;

//         float ratio = 0;
//         if (uCtPts > 0) {
//             ratio = static_cast<float>(uCtFts) / static_cast<float>(uCtPts);
//         }

//         // without edge array
//         if (ratio < 2.5f) {
//             // the stored mesh kernel might be empty
//             if (uCtPts > 0) {
//                 pointArray.resize(uCtPts);
//                 rclIn.read((char*)&(pointArray[0]), uCtPts*sizeof(MeshPoint));
//             }
//             if (uCtFts > 0) {
//                 facetArray.resize(uCtFts);
//                 rclIn.read((char*)&(facetArray[0]), uCtFts*sizeof(MeshFacet));
//             }
//             rclIn.read((char*)&_clBoundBox, sizeof(Base::BoundBox3f));
//         }
//         else {
//             // with edge array
//             unsigned long uCtEdges=uCtFts;
//             str >> magic;
//             uCtFts = magic;
//             pointArray.resize(uCtPts);
//             for (MeshPointArray::_TIterator it = pointArray.begin(); it != pointArray.end(); ++it) {
//                 str >> it->x >> it->y >> it->z;
//             }
//             uint32_t dummy;
//             for (unsigned long i=0; i<uCtEdges; i++) {
//                 str >> dummy;
//             }
//             uint32_t v1, v2, v3;
//             facetArray.resize(uCtFts);
//             for (MeshFacetArray::_TIterator it = facetArray.begin(); it != facetArray.end(); ++it) {
//                 str >> v1 >> v2 >> v3;
//                 it->_aulNeighbours[0] = v1;
//                 it->_aulNeighbours[1] = v2;
//                 it->_aulNeighbours[2] = v3;
//                 str >> v1 >> v2 >> v3;
//                 it->_aulPoints[0] = v1;
//                 it->_aulPoints[1] = v2;
//                 it->_aulPoints[2] = v3;
//                 str >> it->_ucFlag;
//             }

//             str >> _clBoundBox.MinX
//                 >> _clBoundBox.MinY
//                 >> _clBoundBox.MinZ
//                 >> _clBoundBox.MaxX
//                 >> _clBoundBox.MaxY
//                 >> _clBoundBox.MaxZ;
//         }

//         for (auto it = facetArray.begin(); it != facetArray.end(); ++it) {
//             for (int i=0; i<3; i++) {
//                 if (it->_aulPoints[i] >= uCtPts)
//                     throw Base::BadFormatError("Invalid data structure");
//                 if (it->_aulNeighbours[i] < ULONG_MAX && it->_aulNeighbours[i] >= uCtFts)
//                     throw Base::BadFormatError("Invalid data structure");
//             }
//         }

//         _aclPointArray.swap(pointArray);
//         _aclFacetArray.swap(facetArray);
//     }
// }
