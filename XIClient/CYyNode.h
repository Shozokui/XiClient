#pragma once
#include "StorageListObject.h"
namespace FFXI {
	namespace CYy {
		class CYyNode : public StorageListObject {
		public:
			static const CYyClass CYyNodeClass;
			virtual const CYyClass* GetRuntimeClass() override;
			CYyNode();
			~CYyNode();
			void Append(CYyNode**);
			CYyNode* GetNextFreeSpot();
			CYyNode* field_4;
			CYyNode* field_8;
		};
	}
}