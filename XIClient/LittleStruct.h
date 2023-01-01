#pragma once
#include "d3d8to9/d3d8.hpp"
#include "d3dx9math.h"
namespace FFXI {
	class LittleStruct {
	public:
		LittleStruct();
		~LittleStruct();
		void Init();
		int CopyRects(bool);
		int Update(D3DXVECTOR4*, float, float, float, IDirect3DTexture8*, IDirect3DTexture8*, int, int, int);
		int PrepRender(D3DXVECTOR4*, float, float, float, IDirect3DTexture8*, IDirect3DTexture8*, int, int, int);
		void Render(RECT*, float, float, float, float, float, IDirect3DTexture8*, IDirect3DTexture8*, int);
		short field_0;
		short field_2;
		IDirect3DSurface8* field_4;
	};
}