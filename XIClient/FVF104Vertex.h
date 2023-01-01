#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class FVF104Vertex {
		public:
			static const int FVF = D3DFVF_TEX1 | D3DFVF_XYZRHW;
			float X;
			float Y;
			float Z;
			float RHW;
			float TexVertX;
			float TexVertY;
		};
	}
}