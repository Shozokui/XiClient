#pragma once
#define WIN32_LEAN_AND_MEAN
#include "CYyTexBase.h"
#include "d3d8to9/d3d8to9.hpp"
namespace FFXI {
	namespace CYy {
		class CYyBmp2;
		class CYyTex : public CYyTexBase {
		public:
			const static CYyClass CYyTexClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CYyTex();
			~CYyTex();
			void DoThing(char*, char, char);
			void Init();
			bool CreateTextures();
			bool SurfaceStuff();
			void Clean();
			bool LoadFromNumFile(int);
			static int SomeFlag;
			int field_30;
			int field_34;
			CYyTex* field_38;
			int field_3C;
			Direct3DTexture8* RegTex;
			Direct3DCubeTexture8* CubeTex;
		};
	}
}
