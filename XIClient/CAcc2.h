#pragma once
#define WIN32_LEAN_AND_MEAN
#include "StorageListObject.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CAcc2 : public StorageListObject {
		public:
			const static CYyClass CAcc2Class;
			virtual const CYyClass* GetRuntimeClass() override final;
			~CAcc2();
			CAcc2();
			IDirect3DTexture8* Texture[2];
			IDirect3DTexture8* Texture2;
			IDirect3DTexture8* Texture3{ 0 };
			IDirect3DSurface8* Surface{ 0 };
			IDirect3DSurface8* Surface2{ 0 };	
		};
	}
}