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

#include "Packer.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Packer::Packer(UInt Width, UInt Height)
    {
        Reset(Width, Height);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Packer::Reset(UInt Width, UInt Height)
    {
        mDimension.Set(Width, Height);

        mSpaces.clear();
        mSpaces.emplace_back(0, 0, Width, Height);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Recti Packer::Add(UInt Width, UInt Height)
    {
        const auto Iterator = std::find_if(mSpaces.begin(), mSpaces.end(), [Width, Height](Ref<const Rectangle> Space)
        {
            return Width <= Space.Width && Height <= Space.Height;
        });

        if (Iterator != mSpaces.end())
        {
            const Rectangle Space = (* Iterator);

            // Remove the old space from the available list
            mSpaces.erase(Iterator);

            // Split space
            if (Space.Width > Width && Space.Height > Height)
            {
                mSpaces.emplace_back(Space.X + Width, Space.Y, Space.Width - Width, Height);
                mSpaces.emplace_back(Space.X, Space.Y + Height, Space.Width, Space.Height - Height);
            }
            else if (Space.Width > Width)
            {
                mSpaces.emplace_back(Space.X + Width, Space.Y, Space.Width - Width, Space.Height);
            }
            else if (Space.Height > Height)
            {
                mSpaces.emplace_back(Space.X, Space.Y + Height, Space.Width, Space.Height - Height);
            }

            // Return the new position of the item
            return Recti(Space.X, Space.Y, Space.X + Width, Space.Y + Height);
        }

        return Recti(0, 0, 0, 0);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Packer::Remove(Ref<const Recti> Rect)
    {
        // Track back the space that got freed
        Ref<Rectangle> Space = mSpaces.emplace_back(
            Rectangle {
                static_cast<UInt>(Rect.GetX()),
                static_cast<UInt>(Rect.GetY()),
                static_cast<UInt>(Rect.GetWidth()),
                static_cast<UInt>(Rect.GetHeight())
            });

        // Merge with the next space if possible
        auto OtherSpaceIterator = std::find_if(mSpaces.begin(), mSpaces.end(), [&](Ref<const Rectangle> Other)
        {
            return Other.X == Space.X + Space.Width
                && Other.Y == Space.Y
                && Other.Height == Space.Height;
        });

        if (OtherSpaceIterator != mSpaces.end())
        {
            Space.Width += OtherSpaceIterator->Width;
            mSpaces.erase(OtherSpaceIterator);
        }

        // Merge with the previous space if possible
        OtherSpaceIterator = std::find_if(mSpaces.begin(), mSpaces.end(), [&](Ref<const Rectangle> Other)
        {
            return Other.X == Space.X
                && Other.Y == Space.Y + Space.Height
                && Other.Width == Space.Width;
        });

        if (OtherSpaceIterator != mSpaces.end())
        {
            Space.Height += OtherSpaceIterator->Height;
            mSpaces.erase(OtherSpaceIterator);
        }
    }
}