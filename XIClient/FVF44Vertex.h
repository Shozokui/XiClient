#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class FVF44Vertex {
		public:
			const static int FVF = D3DFVF_DIFFUSE | D3DFVF_XYZRHW;
			float X;
			float Y;
			float Z;
			float RHW;
			D3DCOLOR DiffuseColor;
		};
	}
}