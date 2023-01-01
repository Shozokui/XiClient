#pragma once
#define WIN32_LEAN_AND_MEAN
#include "StorageListObject.h"
#include "CAcc.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CMoOcclusionMng : public StorageListObject {
		public:
			static const CYyClass CMoOcclusionMngClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CMoOcclusionMng();
			~CMoOcclusionMng();
			void InitField8();
			int field_4;
			CAcc field_8;
			IDirect3DSurface8* field_18;
		};
	}
}