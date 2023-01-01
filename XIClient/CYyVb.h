#pragma once
#define WIN32_LEAN_AND_MEAN
#include "StorageListObject.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CYyVb : public StorageListObject {
		public:
			const static CYyClass CYyVbClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			~CYyVb();
			CYyVb();
			CYyVb(UINT, DWORD, DWORD, D3DPOOL, IDirect3DVertexBuffer8*);
			int field_4;
			CYyVb* field_8;
			int field_C;
			int field_10;
			int field_14;
			D3DPOOL field_18;
			IDirect3DVertexBuffer8* field_1C;
		};
	}
}