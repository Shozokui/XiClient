#pragma once
#define WIN32_LEAN_AND_MEAN
#include "StorageListObject.h"
#include "DirectShow/streams.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy  {
		class VideoRenderer;
		class CXiMovie : public StorageListObject {
		public:
			CXiMovie();
			bool InitFilterGraph(char*);
			HRESULT CreateFilterGraph(char*);
			IDirect3DTexture8* GetTexture();
			int field_4;
			REFTIME MediaPosBackup;
			int field_C;
			IGraphBuilder* GraphBuilder;
			IMediaControl* MediaControl;
			IMediaPosition* MediaPosition;
			IMediaEvent* MediaEvent;
			VideoRenderer* Renderer;	
		};
	}
}