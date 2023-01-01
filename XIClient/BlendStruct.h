#pragma once
#include "d3d8to9/d3d8types.hpp"
namespace FFXI {
	namespace Util {
		class BlendStruct {
		public:
			void MixSet(unsigned int);
			void Scale(float*);
			void Scale(BlendStruct*, float);
			void Offset(BlendStruct*);
			void GetWithScaledAlpha(D3DCOLOR*, float);
			unsigned char field_0;
			unsigned char field_1;
			unsigned char field_2;
			unsigned char field_3;
		};
	}
}