#pragma once
#define WIN32_LEAN_AND_MEAN
#include "CTkObject.h"
#include "CTkObList.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace Math { class WMatrix;	}
	namespace CTk {
		class _49SubList : public CTkObject {
		public:
			~_49SubList();
			bool DoSomething(void**);
			void DoSomething2();
			void Draw(int, int, D3DCOLOR, int, int);
			void DrawRotate(int, int, FFXI::Math::WMatrix*, D3DCOLOR, int);
			void ExtraDraw(int, int, float, float, unsigned int, char, unsigned int, int);
			void DrawTile(int, int, int, int, unsigned int, unsigned int, float, float, unsigned int, unsigned int, char);
			CTkObList ObList;
			short field_20;
			short field_22;
			short field_24;
			short field_26;
		};
	}
}