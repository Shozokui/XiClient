#pragma once
#include "StorageListObject.h"
#include "d3d8to9/d3d8types.hpp"
namespace FFXI {
	class OTStruct;
	namespace CYy {
		class CMoOT : public StorageListObject {
		public:
			const static CYyClass CMoOTClass;
			static D3DMATERIAL8 Material;
			static float FogStart, FogEnd;
			static D3DCOLOR FogColor;
			static D3DCOLOR Ambient;
			virtual const CYyClass* GetRuntimeClass() override final;
			CMoOT();
			~CMoOT();
			void Init(int, float, float);
			void InitOtStructs();
			void Insert(OTStruct*);
			OTStruct* field_4;
			int field_8;
			float field_C;
			float field_10;
			float field_14;
		};
	}
}