// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "COM_Partitioner.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Partitioner::Insert(Partitioner_Item * Object)
    {
        mQuadtree.Insert(Object);
        mQuadtreeDirty = true;
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Partitioner::Remove(Partitioner_Item * Object)
    {
        mQuadtree.Remove(Object);
        mQuadtreeDirty = true;
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Partitioner::Update(Partitioner_Item * Object)
    {
        mQuadtree.Update(Object);
        mQuadtreeDirty = true;
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Partitioner::Clear()
    {
        mQuadtree.Clear();
        mQuadtreeDirty = true;

        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Partitioner::Query(vbInt32 X1, vbInt32 Y1, vbInt32 X2, vbInt32 Y2, SAFEARRAY ** Result)
    {
        mQuadtreeDirty |= (mLastQueryX1 != X1 || mLastQueryY1 != Y1 || mLastQueryX2 != X2 || mLastQueryY2 != Y2);

        if (mQuadtreeDirty)
        {
            mQuadtreeDirty = false;

            mLastQueryX1   = X1;
            mLastQueryY1   = Y1;
            mLastQueryX2   = X2;
            mLastQueryY2   = Y2;

            LooseQuadtree::Query Intersection
                = mQuadtree.QueryIntersectsRegion(loose_quadtree::BoundingBox<Real32>(X1, Y1, X2 - X1, Y2 - Y1));

            mLastQueryList.clear();

            while (!Intersection.EndOfQuery())
            {
                mLastQueryList.push_back(* Intersection.GetCurrent());

                Intersection.Next();
            }
        }

        CPtr<Partitioner_Item> Chunk(mLastQueryList.data(), mLastQueryList.size());
        VBSpanToSafeArrayTemp(Chunk, Result);

        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Partitioner::Overlaps(vbInt32 X, vbInt32 Y, vbInt32 Radius, Partitioner_Item * Object, vbBool * Result)
    {
        const UInt X1 = X - Radius / 2.0f;
        const UInt Y1 = Y - Radius;
        const UInt X2 = X + Radius / 2.0f;
        const UInt Y2 = Y;

        const bool noOverlap = X1 > Object->RectX2 || Object->RectX1 > X2 || Y1 > Object->RectY2 || Object->RectY1 > Y2;

        (* Result) = (noOverlap ? VBFalse : VBTrue);
        return S_OK;
    }
}