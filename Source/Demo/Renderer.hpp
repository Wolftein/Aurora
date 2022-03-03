// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2020 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include <Graphic/Service.hpp>
#include <Graphic/Pipeline.hpp>
#include <Graphic/Texture.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Renderer
{
	class Batch : public Subsystem
	{
	public:

		static constexpr UInt32 kMaxBatch = 0x00004000;

	public:

		struct Rectangle
		{
			Real32 Left, Top, Right, Bottom;
		};

	public:

		// (Missing Documentation)
		explicit Batch(Ref<Core::Subsystem::Context> Context);

		// (Missing Documentation)
		~Batch();

		// (Missing Documentation)
		void SetData(UInt32 Width, UInt32 Height, Real32 Time);

		// (Missing Documentation)
		void Draw(
				const Rectangle & Destination,
				const Rectangle & Source,
				Bool   Alpha,
				Real32 Depth,
				UInt32 Angle,
				UInt32 Color,
				const SPtr<Graphic::Pipeline> & Pipeline,
				const SPtr<Graphic::Texture> & Texture);

		// (Missing Documentation)
		void Flush();

	private:

		union  Key
		{
			UInt64 Value;

			struct
			{
				UInt64 bDepth     : 32;
				UInt64 bTechnique : 8;
				UInt64 bMaterial  : 16;
				UInt64 bType      : 8;
			} Opaque;

			struct
			{
				UInt64 bTechnique : 8;
				UInt64 bMaterial  : 16;
				UInt64 bDepth     : 32;
				UInt64 bType      : 8;
			} Transparent;
		};

		struct Layout
		{
			Real32 X, Y, Z;
			UInt32 Color;
			Real32 U, V;
		};

		struct Drawable
		{
			Key	      ID;
			Rectangle Destination;
			Rectangle Source;
			UInt32    Depth;
			UInt32 	  Rotation;
			UInt32    Color;
			UInt      Texture;
            UInt      Pipeline;
		};

	private:

		// (Missing Documentation)
		void InitResources();

		// (Missing Documentation)
		void DrawBatch(Graphic::Submission * Call, UInt32 Offset, UInt32 Count, UInt Texture, UInt Pipeline);

		// (Missing Documentation)
		void WriteBatch(Layout * Layout, Drawable ** List, UInt32 Length);

		// (Missing Documentation)
		UInt32 UpdateBatch(Layout * Layout, UInt32 Length);

	private:

        SPtr<Graphic::Service> Service;

		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt       mBuffer[3];
		UInt32     mBufferLocation;
        UInt       mSampler;
		Graphic::Binding mData;

		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

		Drawable   mQueue[kMaxBatch];
		Drawable * mQueueTemp[kMaxBatch];
		UInt32     mQueueQuantity;

		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

		Graphic::Submission mSubmission[kMaxBatch];
	};
}