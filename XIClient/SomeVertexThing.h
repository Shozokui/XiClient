#pragma once
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class SomeVertexThingSub {
		public:
			IDirect3DVertexBuffer8* field_0;
			int field_4;
			int field_8;
			int field_C;
			int field_10;
			unsigned int FVF;
			int field_18;
			int Stride;
		};
		class SomeVertexThing {
		public:
			SomeVertexThing();
			virtual ~SomeVertexThing();
			void Init(int, unsigned int);
			void Clean();
			void UpdateSubValues(int);
			BYTE* LockBuffer();
			void UnlockBuffer();
			void Render(int, int, D3DPRIMITIVETYPE, int, int);
			void Toggle45();
			//field 0 is vtable
			SomeVertexThingSub field_4[2];
			char field_44;
			char field_45;
			char field_46;
			char field_47;
		};
	}
}