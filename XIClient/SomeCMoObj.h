#pragma once
#define WIN32_LEAN_AND_MEAN
#include "CYyObject.h"
#include "WMatrix.h"
namespace FFXI {
	namespace CYy {
		class SomeCMoObj : public CYyObject {
		public:
			void Update();
			int field_4;
			int field_8;
			int field_C;
			FFXI::Math::WMatrix field_10;
			FFXI::Math::WMatrix field_50;
			FFXI::Math::WMatrix field_90;
		};
	}
}