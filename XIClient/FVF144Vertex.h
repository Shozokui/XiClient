#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class FVF144Vertex {
		public:
			static const int FVF = D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW;
			float X;
			float Y;
			float Z;
			float RHW;
			D3DCOLOR DiffuseColor;
			float TexVertX;
			float TexVertY;
		};
	}
}