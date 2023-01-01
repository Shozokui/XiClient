#pragma once
#define WIN32_LEAN_AND_MEAN
#include "streams.h"
#include "d3d8to9/d3d8to9.hpp"
namespace FFXI {
	namespace CYy {
		class VideoRenderer : public CBaseVideoRenderer {
		public:
			VideoRenderer(HRESULT* a);
			virtual HRESULT DoRenderSample(IMediaSample* pMediaSample) override final;
			virtual HRESULT CheckMediaType(const CMediaType*) override final;
			virtual HRESULT SetMediaType(const CMediaType*) override final;
			static const IID thisCLSID;
			static int FrameCopyLock;
			int field_160;
			int field_164;
			int field_168;
			D3DFORMAT field_16C;
			int TextureIndex;
			Direct3DTexture8* TextureArray[4];
			int field_184;
		};
	}
}