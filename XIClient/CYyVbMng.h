#pragma once
#define WIN32_LEAN_AND_MEAN
#include "StorageListObject.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CYyVbMng : public StorageListObject {
		public:
			const static CYyClass CYyVbMngClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			~CYyVbMng();
			CYyVbMng();
			static void DoSomething(IDirect3DVertexBuffer8**);
			int InitBuffer(int, int, int, D3DPOOL, IDirect3DVertexBuffer8**, int);
			int field_4;
			int field_8;
		};
	}
}