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

namespace Content
{
	// -=(Undocumented)=-
	class Uri final
	{
	public:

        // -=(Undocumented)=-
        Uri() = default;

        // -=(Undocumented)=-
        Uri(ConstPtr<Char> Url)
            : mUrl { Url }
        {
        }

        // -=(Undocumented)=-
        Uri(CStr Url)
            : mUrl { Url }
        {
        }

		// -=(Undocumented)=-
		Bool HasSchema() const
		{
			return !GetSchema().empty();
		}

		// -=(Undocumented)=-
		Bool HasFolder() const
		{
			return !GetFolder().empty();
		}

		// -=(Undocumented)=-
		Bool HasExtension() const
		{
			return !GetExtension().empty();
		}

		// -=(Undocumented)=-
        CStr GetUrl() const
		{
			return mUrl;
		}

        // -=(Undocumented)=-
        CStr GetUrlWithoutExtension() const
        {
            const UInt Offset = mUrl.rfind('.');
            return (Offset != CStr::npos ? GetUrl().substr(0, Offset) : GetUrl());
        }

		// -=(Undocumented)=-
        CStr GetSchema() const
		{
			const UInt Offset = mUrl.find("://");
			return (Offset != CStr::npos ? GetUrl().substr(0, Offset) : "");
		}

		// -=(Undocumented)=-
        CStr GetPath() const
		{
			const UInt Offset = mUrl.find("://");
			return (Offset != CStr::npos ? GetUrl().substr(Offset + 3, mUrl.length()) : mUrl);
		}

		// -=(Undocumented)=-
        CStr GetFolder() const
		{
			const CStr Path   = GetPath();
			const UInt Offset = mUrl.find('/');
			return (Offset != CStr::npos ? Path.substr(0, Offset) : "");
		}

		// -=(Undocumented)=-
        CStr GetFilename() const
		{
			const UInt Offset = mUrl.rfind('/');
			return (Offset != CStr::npos ? GetUrl().substr(Offset + 1, mUrl.length()) : GetPath());
		}

		// -=(Undocumented)=-
        CStr GetExtension() const
		{
			const UInt Offset = mUrl.rfind('.');
			return (Offset != CStr::npos ? GetUrl().substr(Offset + 1, mUrl.length()) : "");
		}

    public:

        // -=(Undocumented)=-
        static Uri Merge(ConstRef<Uri> Parent, CStr Subresource)
        {
            return Uri(Format("{}#{}", Parent.GetUrlWithoutExtension(), Subresource));
        }

	private:

		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

		const SStr mUrl;
	};
}