#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CYyTex;
		class MmbStruct2 {
		public:
			int field_0;
			CYyTex* field_4;
			int VerticeCount;
			float* field_C;
			int IndexCount;
			unsigned short* IndexData;
			int PrimitiveCount;
			int field_1C;
			short field_20;
			short field_22;
			short field_24;
			short field_26;
			IDirect3DVertexBuffer8* field_28;
			IDirect3DDevice8* field_2C;
			IDirect3DVertexBuffer8* Buffers[0x20];
			IDirect3DVertexBuffer8* VertexBuffer;
			IDirect3DVertexBuffer8* field_B4;
			IDirect3DIndexBuffer8* IndexBuffer;
			int field_BC;
		};
	}
}