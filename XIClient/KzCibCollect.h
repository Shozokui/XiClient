#pragma once
#include "KzCib.h"
namespace FFXI {
	namespace CYy {
		class KzCibCollect : public KzCib {
		public:
			KzCibCollect();
			int GetSomething(int);
			double GetSomeDouble();
			char field_10;
			char field_11;
			char field_12;
			char field_13;
			char field_14[8];
			char field_1C[8];
			unsigned char field_24;
			char field_25;
			char field_26;
			char field_27;
		};
	}
}