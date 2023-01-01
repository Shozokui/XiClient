#pragma once
#include "CYyNode.h"
namespace FFXI {
	namespace CYy {
		class CYyResourceFile;
		class CYyResfList : public CYyNode {
		public:
			static const CYyClass CYyResfListClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CYyResfList();
			~CYyResfList();
			int field_C;
			int field_10;
			int field_14;
			int field_18;
			CYyResourceFile** field_1C;
			bool field_20;
		};
	}
}