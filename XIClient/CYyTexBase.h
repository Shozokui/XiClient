#pragma once
#include "StorageListObject.h"

namespace FFXI {
	namespace CYy {
		class CYyBmp2;
		class CYyTexBase : public StorageListObject {
		public:
			const static CYyClass CYyTexBaseClass;
			virtual const CYyClass* GetRuntimeClass() override;
			virtual ~CYyTexBase() = default;
			CYyTexBase();
			static int Counter;
			int ReferenceCount;
			CYyBmp2* field_8;
			char Unique[16];
			void* field_1C;
			void* field_20;
			unsigned short ImageWidth;
			unsigned short ImageHeight;
			char field_28;
			char BitDepth;
			unsigned char field_2A;
			char field_2B;
			unsigned int field_2C;
		};
	}
}