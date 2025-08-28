// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Aurora.Base/Base.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    /// \brief Represents a color with red, green, blue, and alpha components.
    class Color final
    {
    public:

        /// \brief Constructs an uninitialized color with all components set to 0.
        AURORA_INLINE constexpr Color()
            : mComponents { 0 }
        {
        }

        /// \brief Constructs a color with specified red, green, blue, and alpha components.
        /// 
        /// \param Red   Value of the red channel (0.0 to 1.0).
        /// \param Green Value of the green channel (0.0 to 1.0).
        /// \param Blue  Value of the blue channel (0.0 to 1.0).
        /// \param Alpha Value of the alpha channel (0.0 to 1.0).
        AURORA_INLINE constexpr Color(Real32 Red, Real32 Green, Real32 Blue, Real32 Alpha = 1.0f)
            : mComponents { Red, Green, Blue, Alpha }
        {
        }

        /// \brief Returns the red component of the color.
        /// 
        /// \return Red component.
        AURORA_INLINE constexpr Real32 GetRed() const
        {
            return mComponents[0];
        }

        /// \brief Returns the green component of the color.
        /// 
        /// \return Green component.
        AURORA_INLINE constexpr Real32 GetGreen() const
        {
            return mComponents[1];
        }

        /// \brief Returns the blue component of the color.
        /// 
        /// \return Blue component.
        AURORA_INLINE constexpr Real32 GetBlue() const
        {
            return mComponents[2];
        }

        /// \brief Returns the alpha component of the color.
        /// 
        /// \return Alpha component.
        AURORA_INLINE constexpr Real32 GetAlpha() const
        {
            return mComponents[3];
        }

        /// \brief Serializes the state of the object to or from the specified archive.
        /// 
        /// \param Archive The archive to serialize the object with.
        template<typename Serializer>
        AURORA_INLINE void OnSerialize(Serializer Archive)
        {
            Archive.SerializeObject(mComponents[0]);
            Archive.SerializeObject(mComponents[1]);
            Archive.SerializeObject(mComponents[2]);
            Archive.SerializeObject(mComponents[3]);
        }

    public:

        /// \brief Packs 8-bit RGBA components into a 32-bit unsigned integer.
        ///
        /// Layout: [R | G | B | A], with each channel in the range [0–255].
        ///
        /// \param Red   Red channel (0–255).
        /// \param Green Green channel (0–255).
        /// \param Blue  Blue channel (0–255).
        /// \param Alpha Alpha channel (0–255).
        /// \return A 32-bit packed RGBA value.
        AURORA_INLINE constexpr static UInt32 PackRGBA8(UInt8 Red, UInt8 Green, UInt8 Blue, UInt8 Alpha = 255)
        {
            if constexpr (AURORA_LITTLE_ENDIAN)
            {
                return   (static_cast<UInt32>(Alpha) << 24)
                       | (static_cast<UInt32>(Blue)  << 16)
                       | (static_cast<UInt32>(Green) << 8)
                       | (static_cast<UInt32>(Red));
            }
            else
            {
                return   (static_cast<UInt32>(Red)   << 24)
                       | (static_cast<UInt32>(Green) << 16)
                       | (static_cast<UInt32>(Blue)  << 8)
                       | (static_cast<UInt32>(Alpha));
            }
        }

        /// \brief Packs normalized float RGBA components into a 32-bit unsigned integer.
        ///
        /// Layout: [R | G | B | A], with each channel in the range [0–1].
        ///
        /// \param Red   Red channel (0.0–1.0).
        /// \param Green Green channel (0.0–1.0).
        /// \param Blue  Blue channel (0.0–1.0).
        /// \param Alpha Alpha channel (0.0–1.0).
        /// \return A 32-bit packed RGBA value.
        AURORA_INLINE constexpr static UInt32 PackRGBA32F(Real32 Red, Real32 Green, Real32 Blue, Real32 Alpha = 1.0f)
        {
            if constexpr (AURORA_LITTLE_ENDIAN)
            {
                return   (static_cast<UInt32>(Alpha * 0xFF) << 24)
                       | (static_cast<UInt32>(Blue  * 0xFF) << 16)
                       | (static_cast<UInt32>(Green * 0xFF) << 8)
                       | (static_cast<UInt32>(Red   * 0xFF));
            }
            else
            {
                return   (static_cast<UInt32>(Red   * 0xFF) << 24)
                       | (static_cast<UInt32>(Green * 0xFF) << 16)
                       | (static_cast<UInt32>(Blue  * 0xFF) << 8)
                       | (static_cast<UInt32>(Alpha * 0xFF));
            }
        }

        /// \brief Linearly interpolates between two colors.
        /// 
        /// \param Start      The starting color.
        /// \param End        The ending color.
        /// \param Percentage The interpolation percentage (range between 0 and 1).
        /// \return A color interpolated between the start and end colors.
        AURORA_INLINE constexpr static Color Lerp(ConstRef<Color> Start, ConstRef<Color> End, Real32 Percentage)
        {
            const Real32 Red   = Base::Lerp(Start.GetRed(),   End.GetRed(),   Percentage);
            const Real32 Green = Base::Lerp(Start.GetGreen(), End.GetGreen(), Percentage);
            const Real32 Blue  = Base::Lerp(Start.GetBlue(),  End.GetBlue(),  Percentage);
            const Real32 Alpha = Base::Lerp(Start.GetAlpha(), End.GetAlpha(), Percentage);
            return Color(Red, Green, Blue, Alpha);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<Real32, 4> mComponents;
    };
}