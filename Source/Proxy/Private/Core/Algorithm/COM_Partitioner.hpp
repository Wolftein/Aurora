// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Core/COM_Common.hpp"
#include <LooseQuadtree.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    [export, uuid("207F73B7-28C7-11EE-ADCE-1418C3A8EDB8")]
    typedef struct Partitioner_Item
    {
        vbInt16  ID;
        vbInt16  Type;
        vbInt32  X;
        vbInt32  Y;

        vbReal32 RectX1;
        vbReal32 RectY1;
        vbReal32 RectX2;
        vbReal32 RectY2;
    } Partitioner_Item;

    // -=(Undocumented)=-
    [object, uuid("8603F791-28C7-11EE-ADCE-1418C3A8EDB8"), pointer_default(unique)]
    __interface Partitioner_
    {
        HRESULT Insert([in] Partitioner_Item * Object);

        HRESULT Remove([in] Partitioner_Item * Object);

        HRESULT Update([in] Partitioner_Item * Object);

        HRESULT Clear();

        HRESULT Query([in] vbInt32 X1, [in] vbInt32 Y1, [in] vbInt32 X2, [in] vbInt32 Y2, [in, satype(Partitioner_Item)] SAFEARRAY ** Result);

        HRESULT Overlaps([in] vbInt32 X,[in] vbInt32 Y,[in] vbInt32 Radius, [in] Partitioner_Item * Object, [out, retval] vbBool * Result);
    };

    // -=(Undocumented)=-
    [coclass, uuid("88FED143-28C7-11EE-ADCE-1418C3A8EDB8")]
    class ATL_NO_VTABLE Partitioner : public Partitioner_
    {
    public:

        // \see Partitioner_::Insert
        HRESULT Insert(Partitioner_Item * Object) override;

        // \see Partitioner_::Remove
        HRESULT Remove(Partitioner_Item * Object) override;

        // \see Partitioner_::Update
        HRESULT Update(Partitioner_Item * Object) override;

        // \see Partitioner_::Clear
        HRESULT Clear() override;

        // \see Partitioner_::Query
        HRESULT Query(vbInt32 X1, vbInt32 Y1, vbInt32 X2, vbInt32 Y2, SAFEARRAY ** Result) override;

        // \see Partitioner_::Overlaps
        HRESULT Overlaps(vbInt32 X, vbInt32 Y, vbInt32 Radius, Partitioner_Item * Object, vbBool * Result);

    private:

        // -=(Undocumented)=-
        struct LooseQuadtreeExtractor
        {
            static void ExtractBoundingBox(
                Ptr<const Partitioner_Item> Object, Ptr<loose_quadtree::BoundingBox<Real32>> Boundaries)
            {
                Boundaries->left   = Object->RectX1;
                Boundaries->top    = Object->RectY1;
                Boundaries->width  = (Object->RectX2 - Object->RectX1);
                Boundaries->height = (Object->RectY2 - Object->RectY1);
            }
        };

    private:

        // -=(Undocumented)=-
        using LooseQuadtree = loose_quadtree::LooseQuadtree<Real32, Partitioner_Item, LooseQuadtreeExtractor>;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        LooseQuadtree            mQuadtree;
        Bool                     mQuadtreeDirty;
        Vector<Partitioner_Item> mQuadtreeDatabase;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<Partitioner_Item> mLastQueryList;
        SInt32                   mLastQueryX1;
        SInt32                   mLastQueryY1;
        SInt32                   mLastQueryX2;
        SInt32                   mLastQueryY2;
    };
}