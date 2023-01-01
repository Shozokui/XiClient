#pragma once
#include "ReferenceReadBase.h"
namespace FFXI {
	namespace CYy { class CXiActor;	class CYyResourceFile; }
	namespace File {
		class EquipReferenceRead : public ReferenceReadBase {
		public:
			static void ReadCallback(CYy::CYyResourceFile**, ReferenceReadBase*);
			CYy::CXiActor* field_10;
			int field_14;
			int EquipModelID;
			int DATIndex;
		};
	}
}