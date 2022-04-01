// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2020 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Renderer.hpp"
#include "Graphic/Service.hpp"
#include <algorithm>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Renderer
{
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	Batch::Batch(Ref<Core::Subsystem::Context> Context)
        : Subsystem(Context),
          mBuffer { 0 },
		  mBufferLocation { 0 },
		  mSampler { 0 },
		  mQueue { 0 },
		  mQueueTemp { 0 },
		  mQueueQuantity { 0 },
		  mSubmission { 0 }
	{
        mSubmission = new Graphic::Submission[kMaxBatch];
        Service = Context.GetSubsystem<Graphic::Service>();
        
		InitResources();
	}

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	Batch::~Batch()
	{
        Service->DeleteBuffer(mBuffer[0]);
        Service->DeleteBuffer(mBuffer[1]);
        Service->DeleteBuffer(mBuffer[2]);
        Service->DeleteSampler(mSampler);
	}

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	void Batch::SetData(Real32 X, Real32 Y, Real32 Width, Real32 Height, Real32 Time)
	{
        Real32 kNear = 0.0f;
        Real32 kFar  = (1 << 24) - 1;

        struct Data
        {
            Real32 Projection[4 * 4];
            Real32 View[4 * 4];
            Real32 Time;
        };

        // @formatter:off
        Data Uniform{
            .Projection = {
                2.0f / Width,  0.0f,            0.0f,                   -1.0f,
                0.0f,         -2.0f / Height,   0.0f,                    1.0f,
                0.0f,          0.0f,            1.0f / (kFar - kNear),   0.0f,
                0.0f,          0.0f,            kNear / (kNear - kFar),  1.0f,
            },
            .View = {
                1.0f,   0.0f,   0.0f,   X,
                0.0f,   1.0f,   0.0f,   Y,
                0.0f,   0.0f,   1.0f,   0.0f,
                0.0f,   0.0f,   0.0f,   1.0f,
            },
            .Time = Time
        };
         // @formatter:on

        Service->UpdateBuffer(mBuffer[2], true, 0, { (UInt08 *) & Uniform, sizeof(Data) });
	}

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	void Batch::Draw(
			const Rectangle & Destination,
			const Rectangle & Source,
			Bool   Alpha,
            Real32 Depth,
			UInt32 Angle,
			UInt32 Color,
			const SPtr<Graphic::Pipeline> & Pipeline,
			const SPtr<Graphic::Texture> & Texture)
	{
		if (!Pipeline->HasLoaded()) {
			return;
		}

		if (mQueueQuantity == kMaxBatch)
		{
			Flush();
		}

		Key ID;

		if (Alpha)
		{
			ID.Transparent.bType     = 0;
			ID.Transparent.bTechnique = Pipeline->GetID();
			ID.Transparent.bMaterial = Texture->GetID();
			ID.Transparent.bDepth    = 16777215 - Depth;
		}
		else
		{
			ID.Opaque.bType     = 1;
			ID.Opaque.bTechnique = Pipeline->GetID();
			ID.Opaque.bMaterial = Texture->GetID();
			ID.Opaque.bDepth    = Depth;
		}

		Drawable * Drawable = (mQueueTemp[mQueueQuantity] = & mQueue[mQueueQuantity]);
		++mQueueQuantity;

		Drawable->ID          = ID;
		Drawable->Destination = Destination;
		Drawable->Source      = Source;
		Drawable->Depth       = Depth;
		Drawable->Rotation    = Angle;
		Drawable->Color       = Color;
		Drawable->Texture     = Texture->GetID();
		Drawable->Pipeline    = Pipeline->GetID();
	}

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	void Batch::Flush()
	{
		if (mQueueQuantity == 0u)
		{
			return;
		}

		std::sort(std::begin(mQueueTemp), std::begin(mQueueTemp) + mQueueQuantity, [] (Drawable * p_1st, Drawable * p_2nd)
		{
			return p_1st->ID.Value > p_2nd->ID.Value;
		});

		static Layout sLayout[kMaxBatch * 4u];
		static UInt32 sQueue[kMaxBatch], sQueueCapacity = 0u;

		for (auto zBegin = 0u, zEnd = 1u; zBegin < mQueueQuantity; zBegin = zEnd, ++sQueueCapacity)
		{
			const auto Texture  = mQueueTemp[zBegin]->Texture;
			const auto Pipeline = mQueueTemp[zBegin]->Pipeline;

			for (; zEnd < mQueueQuantity && mQueueTemp[zEnd]->Texture == Texture && mQueueTemp[zEnd]->Pipeline == Pipeline; ++zEnd);

			WriteBatch(& sLayout[zBegin * 4], & mQueueTemp[zBegin], (sQueue[sQueueCapacity] = zEnd - zBegin));
		}

		UInt32 Offset = UpdateBatch(sLayout, mQueueQuantity); // Update Buffer

		for (UInt32 ID = 0u, Address = 0u; ID < sQueueCapacity; Address += sQueue[ID], ++ID)
		{
			DrawBatch(& mSubmission[ID], Offset + Address, sQueue[ID],
					mQueueTemp[Address]->Texture, mQueueTemp[Address]->Pipeline);
		}

		if (sQueueCapacity > 0)
		{
            Service->Submit({ mSubmission, sQueueCapacity });   // Draw Calls
		}

		mQueueQuantity = sQueueCapacity = 0u;
	}

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	void Batch::InitResources()
	{
		mBuffer[0] = Service->CreateBuffer(true, sizeof(Layout)  * 4 * kMaxBatch, {});
		mBuffer[2] = Service->CreateBuffer(false, sizeof(Real32) * 64 , {});

		UPtr<UInt16[]> Memory = eastl::make_unique<UInt16[]>(kMaxBatch * 6u);

		for (UInt32 Position = 0u, Index = 0u, max = kMaxBatch * 6u; Position < max; Index += 4u)
		{
			Memory[Position++] = static_cast<UInt16>(Index);
			Memory[Position++] = static_cast<UInt16>(Index + 1);
			Memory[Position++] = static_cast<UInt16>(Index + 2);
			Memory[Position++] = static_cast<UInt16>(Index + 0);
			Memory[Position++] = static_cast<UInt16>(Index + 2);
			Memory[Position++] = static_cast<UInt16>(Index + 3);
		}

		mBuffer[1] = Service->CreateBuffer(true, 6 * kMaxBatch * sizeof(UInt16), {
				reinterpret_cast<UInt08 *>(Memory.get()), 6 * kMaxBatch * sizeof(UInt16)
		});

		mSampler = Service->CreateSampler(
				Graphic::TextureEdge::Repeat, Graphic::TextureEdge::Repeat, Graphic::TextureFilter::Trilinear);
	}

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	void Batch::DrawBatch(Graphic::Submission * Call, UInt32 Offset, UInt32 Count, UInt Texture, UInt Pipeline)
	{
        memset(Call, 0, sizeof(Graphic::Submission));

		Call->Vertices.Buffer    = mBuffer[0];
		Call->Vertices.Offset    = 0;
		Call->Vertices.Length    = 0;
        Call->Vertices.Stride    = 24;
		Call->Indices.Buffer     = mBuffer[1];
		Call->Indices.Offset     = Offset * 6;
		Call->Indices.Length     = 6 * Count;
        Call->Indices.Stride     = sizeof(UInt16);
		Call->Uniforms[0].Buffer  = mBuffer[2];
		Call->Uniforms[0].Offset  = 0;
		Call->Uniforms[0].Length  = sizeof(Real32) * (4 * 4);
		//Call->Uniforms[1]       = mData;
		Call->Scissor             = { 0, 0, UINT16_MAX, UINT16_MAX };
		Call->Pipeline            = Pipeline;
		Call->Samplers[0]         = mSampler;
		Call->Textures[0]         = Texture;
	}

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	void Batch::WriteBatch(Layout * Layout, Drawable ** List, UInt32 Length)
	{
		for (UInt32 zID = 0u; zID < Length; ++zID)
		{
			const Drawable * Sprite = List[zID];

			//
			// Calculate Depth (We're using 24-bit depth)
			//
			Real32 Depth = static_cast<Real32>(Sprite ->Depth);

			//
			// Calculate \ref Drawable's vertices (rotation)
			//
			Real32 DestinationX1 = Sprite->Destination.Left;
			Real32 DestinationX2 = Sprite->Destination.Left;
			Real32 DestinationX3 = Sprite->Destination.Right;
			Real32 DestinationX4 = Sprite->Destination.Right;

			Real32 DestinationY1 = Sprite->Destination.Top;
			Real32 DestinationY2 = Sprite->Destination.Bottom;
			Real32 DestinationY3 = Sprite->Destination.Bottom;
			Real32 DestinationY4 = Sprite->Destination.Top;

			if (Sprite->Rotation != 0.0f && Sprite->Rotation != 360)
			{
				const Real32 Angles = (Sprite->Rotation * 3.141592654f) / 180.0f;
				const Real32 Cos    = cosf(Angles);
				const Real32 Sin    = sinf(Angles);

				const Real32 DimX   = (Sprite->Destination.Right  - Sprite->Destination.Left);
				const Real32 DimY   = (Sprite->Destination.Bottom - Sprite->Destination.Top);
				const Real32 WorldX = (DimX * 0.5f);
				const Real32 WorldY = (DimY * 0.5f);

				const Real32 LocalX1 = -WorldX;
				const Real32 LocalY1 = -WorldY;
				const Real32 LocalX2 = LocalX1 + DimX;
				const Real32 LocalY2 =  LocalY1 + DimY;

				const Real32 LocalXCos  = LocalX1 * Cos;
				const Real32 LocalXSin  = LocalX1 * Sin;
				const Real32 LocalYCos  = LocalY1 * Cos;
				const Real32 LocalYSin  = LocalY1 * Sin;
				const Real32 LocalX2Cos = LocalX2 * Cos;
				const Real32 LocalY2Cos = LocalY2 * Cos;
				const Real32 LocalY2Sin = LocalY2 * Sin;

				const auto X1  = LocalXCos - LocalYSin;
				const auto Y1  = LocalYCos + LocalXSin;
				DestinationX1 = X1 + WorldX;
				DestinationY1 = Y1 + WorldY;

				const auto X4  = LocalXCos - LocalY2Sin;
				const auto Y4  = LocalY2Cos + LocalXSin;
				DestinationX4 = X4 + WorldX;
				DestinationY4 = Y4 + WorldY;

				const auto X3  = LocalX2Cos - LocalY2Sin;
				const auto Y3  = LocalY2Cos + LocalY2Sin;
				DestinationX3 = X3 + WorldX;
				DestinationY3 = Y3 + WorldY;

				DestinationX2 = DestinationX1 + (DestinationX3 - DestinationX4);
				DestinationY2 = DestinationY3 - (DestinationY4 - DestinationY1);
			}

			Layout->X     = DestinationX1;
			Layout->Y     = DestinationY1;
			Layout->Z     = Depth;
			Layout->Color = Sprite->Color;
			Layout->U     = Sprite->Source.Left;
			Layout->V     = Sprite->Source.Top;
			++Layout;

			Layout->X     = DestinationX2;
			Layout->Y     = DestinationY2;
			Layout->Z     = Depth;
			Layout->Color = Sprite->Color;
			Layout->U     = Sprite->Source.Left;
			Layout->V     = Sprite->Source.Bottom;
			++Layout;

			Layout->X     = DestinationX3;
			Layout->Y     = DestinationY3;
			Layout->Z     = Depth;
			Layout->Color = Sprite->Color;
			Layout->U     = Sprite->Source.Right;
			Layout->V     = Sprite->Source.Bottom;
			++Layout;

			Layout->X     = DestinationX4;
			Layout->Y     = DestinationY4;
			Layout->Z     = Depth;
			Layout->Color = Sprite->Color;
			Layout->U     = Sprite->Source.Right;
			Layout->V     = Sprite->Source.Top;
			++Layout;
		}
	}

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	UInt32 Batch::UpdateBatch(Layout * Layout, UInt32 Length)
	{
		const UInt32 Position = mBufferLocation + Length > kMaxBatch ? 0 : mBufferLocation;
		const UInt32 Count    = Length * sizeof(struct Layout) * 4;

		Service->UpdateBuffer(mBuffer[0], (Position == 0), Position * sizeof(struct Layout) * 4, {
				reinterpret_cast<UInt08 *>(Layout), Count
		});

		mBufferLocation = Position + Length;
		return Position;
	}
}