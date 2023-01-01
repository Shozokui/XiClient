#pragma once
#define WIN32_LEAN_AND_MEAN
#include "CTkObject.h"
#include "FVF144Vertex.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace Math { class WMatrix; }
	namespace CYy { class CYyTex; }
	namespace CTk {
		class _49SubListNode : public CTkObject {
		public:
			_49SubListNode();
			void Fill(void**);
			void TexInit();
			void Draw(int, int, float, float, D3DCOLOR, int);
			void DrawRotate(int, int, FFXI::Math::WMatrix*, D3DCOLOR, int);
			void RenderTex(const void*, int, int, int, int);
			void DrawTile(int, int, int, int, unsigned int, unsigned int, float, float, unsigned int, unsigned int, char);
			int field_4;
			unsigned short field_8;
			unsigned short field_A;
			unsigned short field_C;
			unsigned short field_E;
			int field_10;
			int field_14;
			int field_18;
			char field_1C;
			unsigned char VertReds[4];
			unsigned char VertGreens[4];
			unsigned char VertBlues[4];
			unsigned char VertAlphas[4];
			unsigned char field_2D;
			unsigned char field_2E;
			unsigned char field_2F;
			unsigned char field_30;
			char field_31;
			char field_32[0x18];
			short field_4A;
			CYy::CYyTex* Tex;
			CYy::FVF144Vertex Verts[4];
			short VertX[4];
			short VertY[4];
		};
	}
}