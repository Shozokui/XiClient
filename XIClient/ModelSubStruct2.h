#pragma once
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class ModelSubStruct2 {
		public:
			static bool some_flag;
			static D3DCOLOR some_color;
			static void SetStatics(D3DCOLOR, bool);
			ModelSubStruct2();
			~ModelSubStruct2();
			void PrepareViewport();
			void CleanViewport();
			void DrawSomething();
			bool field_4;
			IDirect3DTexture8* field_8[4];
		};
	}
}