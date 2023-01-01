#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3d8to9/d3d8types.hpp"
#include "FVF144Vertex.h"

namespace FFXI {
	namespace CTk { class _49SubList; }
	namespace CYy {
		class CYyTex;
		class CYyIcon;
	}
	namespace Text {
		class MojiDraw {
		public:
			MojiDraw();
			~MojiDraw();
			static tagPOINT TextPos;
			static CTk::_49SubList*** g_pFontUsGaiji;
			void YmDrawString(int, int, const char*, int, D3DCOLOR, int, float, float);
			void YmDrawChar(short, D3DCOLOR, int, float, float);
			void moji_print2(int, tagPOINT*, float, float);
			void SetMojiTexture();
			void SetIndent(int, int*);
			void InitHQTex();
			bool UseHQTex();
			CYy::FVF144Vertex Vertices[4];
			int field_70;
			CYy::CYyTex* BasicMojiMap;
			CYy::CYyIcon* HQTex;
			char* HQMapData;
			char TryUseHQTex;
			char field_81;
			char field_82;
			char field_83;
		};
	}
}