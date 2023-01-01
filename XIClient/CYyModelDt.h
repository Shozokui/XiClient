#pragma once
#include "StorageListObject.h"
#include "VbList.h"

namespace FFXI {
	namespace CYy {
		class CMoOs2;
		class CYyOs2VtxBuffer;
		class CXiActor;
		class CYyModel;
		class CYyModelBase;
		class CYyModelDt : public StorageListObject {
		public:
			static const CYyClass CYyModelDtClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CYyModelDt();
			virtual ~CYyModelDt();
			virtual void VirtModelDt1(CXiActor*, CYyModel*, CYyModelBase*, float, int*, int*) = 0;
			virtual void VirtModelDt2() = 0;
			virtual void VirtModelDt3() = 0;
			virtual void VirtModelDt4(unsigned char*) = 0;
			virtual CYyOs2VtxBuffer* VirtModelDt5() = 0;
			int GetResId();
			CYyModelDt* field_4;
			CMoOs2** field_8;
			int field_C;
			VbList field_10;
		};
	}
 }