// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Text.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Text ConvertUTF16ToUTF8(std::wstring_view Value)
    {
        Text Result;
        Result.reserve(Value.size() * 4);

        for (auto Iterator = Value.begin(); Iterator != Value.end(); ++Iterator)
        {
            const UInt16 Character = (* Iterator);
            UInt32 Codepoint = 0;

            if (Character >= 0xD800 && Character <= 0xDBFF)
            {
                const UInt16 Surrogate = * (Iterator++);
                Codepoint = ((Character - 0xD800) << 10) | (Surrogate - 0xDC00);
                Codepoint += 0x10000;
            }
            else
            {
                Codepoint = Character;
            }

            if (Codepoint <= 0x7F)
            {
                Result.push_back(static_cast<char>(Codepoint));
            }
            else if (Codepoint <= 0x7FF)
            {
                Result.push_back(0xC0 | (Codepoint >> 6));
                Result.push_back(0x80 | (Codepoint & 0x3F));
            }
            else if (Codepoint <= 0xFFFF)
            {
                Result.push_back(0xE0 | (Codepoint >> 12));
                Result.push_back(0x80 | ((Codepoint >> 6) & 0x3F));
                Result.push_back(0x80 | (Codepoint & 0x3F));
            }
            else if (Codepoint <= 0x10FFFF)
            {
                Result.push_back(0xF0 | (Codepoint >> 18));
                Result.push_back(0x80 | ((Codepoint >> 12) & 0x3F));
                Result.push_back(0x80 | ((Codepoint >> 6) & 0x3F));
                Result.push_back(0x80 | (Codepoint & 0x3F));
            }
        }
        return Result;
    }
}