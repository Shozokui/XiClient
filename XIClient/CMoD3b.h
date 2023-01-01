#pragma once
#include "CMoResource.h"
#include "TexHelper.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CMoD3bElem;
		class CMoD3b : public CMoResource {
		public:
			CMoD3b() = default;
			virtual ~CMoD3b() = default;
			virtual void Open() override final;
			virtual void Close() override final;
			void Draw(CMoD3bElem*, D3DCOLOR*, float*, float*, float*, bool);
			float* MakeObj(float*, float*);
			unsigned int field_30;
			short field_34;
			short field_36;
			unsigned short field_38;
			unsigned short field_3A;
			unsigned short field_3C;
			unsigned short field_3E;
			TexHelper field_40;
		};
	}
}