#pragma once
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CMoResource;
		class ModelSubStruct5 {
		public:
			static D3DCOLOR const_color;
			CMoResource** GetResource();
			ModelSubStruct5();
			~ModelSubStruct5();
			void SetConstColor();
			void DrawSomething();
			void DrawAnotherSomething();
			CMoResource** field_4;
			int field_8;
		};
	}
}