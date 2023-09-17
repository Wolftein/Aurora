// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
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
// [  NAMESPACE  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

using namespace loose_quadtree;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    [export, uuid("207F73B7-28C7-11EE-ADCE-1418C3A8EDB8")]
    typedef struct Scene_Partitioner_Item
    {
        vbInt16  ID;
        vbInt16  Type;
        vbInt32  X;
        vbInt32  Y;

        vbReal32 RectX1;
        vbReal32 RectY1;
        vbReal32 RectX2;
        vbReal32 RectY2;
    } Scene_Partitioner_Item;

    // -=(Undocumented)=-
    [object, uuid("8603F791-28C7-11EE-ADCE-1418C3A8EDB8"), pointer_default(unique)]
    __interface Scene_Partitioner_
    {
        HRESULT Insert([in] Scene_Partitioner_Item * Object);

        HRESULT Remove([in] Scene_Partitioner_Item * Object);

        HRESULT Update([in] Scene_Partitioner_Item * Object);

        HRESULT Clear();

        HRESULT Query([in] vbInt32 X1, [in] vbInt32 Y1, [in] vbInt32 X2, [in] vbInt32 Y2, [in, satype(Scene_Partitioner_Item)] SAFEARRAY ** Result);

        HRESULT Overlaps([in] vbInt32 X,[in] vbInt32 Y,[in] vbInt32 Radius, [in] Scene_Partitioner_Item * Object, [out, retval] vbBool * Result);
    };

    // -=(Undocumented)=-
    [coclass, uuid("88FED143-28C7-11EE-ADCE-1418C3A8EDB8")]
    class ATL_NO_VTABLE Scene_Partitioner : public Scene_Partitioner_
    {
    public:

        // \see Scene_Partitioner_::Insert
        HRESULT Insert(Scene_Partitioner_Item * Object) override;

        // \see Scene_Partitioner_::Remove
        HRESULT Remove(Scene_Partitioner_Item * Object) override;

        // \see Scene_Partitioner_::Update
        HRESULT Update(Scene_Partitioner_Item * Object) override;

        // \see Scene_Partitioner_::Clear
        HRESULT Clear() override;

        // \see Scene_Partitioner_::Query
        HRESULT Query(vbInt32 X1, vbInt32 Y1, vbInt32 X2, vbInt32 Y2, SAFEARRAY ** Result) override;

        // \see Scene_Partitioner_::Overlaps
        HRESULT Overlaps(vbInt32 X, vbInt32 Y, vbInt32 Radius, Scene_Partitioner_Item * Object, vbBool * Result);

    private:

        // -=(Undocumented)=-
        struct LooseQuadtreeExtractor
        {
            static void ExtractBoundingBox(Ptr<const Scene_Partitioner_Item> Object, Ptr<BoundingBox<Real32>> Boundaries)
            {
                Boundaries->left   = Object->RectX1;
                Boundaries->top    = Object->RectY1;
                Boundaries->width  = (Object->RectX2 - Object->RectX1);
                Boundaries->height = (Object->RectY2 - Object->RectY1);
            }
        };

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        LooseQuadtree<Real32, Scene_Partitioner_Item, LooseQuadtreeExtractor> mQuadtree;
        Bool                                                                  mQuadtreeDirty;
        Vector<Scene_Partitioner_Item>                                        mQuadtreeDatabase;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<Scene_Partitioner_Item>                                        mLastQueryList;
        SInt32                                                                mLastQueryX1;
        SInt32                                                                mLastQueryY1;
        SInt32                                                                mLastQueryX2;
        SInt32                                                                mLastQueryY2;
    };
}