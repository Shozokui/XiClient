#pragma once
#define WIN32_LEAN_AND_MEAN
#include "StorageListObject.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CAcc : public StorageListObject {
		public:
			const static CYyClass CAccClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CAcc();
			~CAcc();
			IDirect3DTexture8* SomeTexture;
			IDirect3DTexture8* Texture2;
			IDirect3DSurface8* Surface;
		};
	}
}