#pragma once
#include "CMoResource.h"
#include "d3d8to9/d3d8types.hpp"
namespace FFXI {
	namespace CYy {
		class CMoD3mElem;
		class CMoD3m : public CMoResource {
		public:
			CMoD3m() = default;
			virtual ~CMoD3m() = default;
			virtual void Open() override final;
			virtual void Close() override final;
			unsigned short* GetShortPointer();
			unsigned short* GetAnotherShortPointer();
			float* GetFloatPointer();
			float GetSomeMagnitude();
			void Draw(D3DCOLOR*, CMoD3mElem*);
		};
	}
}