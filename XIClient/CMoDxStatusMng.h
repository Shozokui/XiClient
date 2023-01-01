#pragma once
#include "StorageListObject.h"
#include "StatusNode.h"
namespace FFXI {
	namespace CYy {
		class CMoDxStatusMng : public StorageListObject {
		public:
			static const CYyClass CMoDxStatusMngClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CMoDxStatusMng();
			~CMoDxStatusMng();
			StatusNode field_4;
		};
	}
}