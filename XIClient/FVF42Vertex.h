#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class FVF42Vertex {
		public:
			const static int FVF = D3DFVF_DIFFUSE | D3DFVF_XYZ;
			float X;
			float Y;
			float Z;
			D3DCOLOR DiffuseColor;
		};
	}
}