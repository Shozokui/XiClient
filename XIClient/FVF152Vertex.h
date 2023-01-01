#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class FVF152Vertex {
		public:
			static const int FVF = D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ;
			float X;
			float Y;
			float Z;
			float N_X;
			float N_Y;
			float N_Z;
			D3DCOLOR Diffuse;
			float TexVertX;
			float TexVertY;
		};
	}
}