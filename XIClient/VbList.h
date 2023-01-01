#pragma once
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class VbList {
		public:
			VbList();
			~VbList();
			int field_4;
			int field_8;
			IDirect3DVertexBuffer8* field_C[8];
		};
	}
}