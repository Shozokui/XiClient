#pragma once
#include "StorageListObject.h"
#include "Enums.h"
namespace FFXI {
	namespace CYy {
		class CEnv : public StorageListObject {
		public:
			const static CYyClass CEnvClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			~CEnv();
			CEnv();
			void Init();
			void CheckCPU();
			void DoSomething(float);
			bool CheckField18();
			FFXI::Constants::Enums::CPUVendorID CPUType;
			char field_8;
			char field_9;
			char field_A;
			char field_B;
			char field_C;
			char field_D;
			char field_E;
			char field_F;
			int field_10;
			char field_14;
			char field_15;
			char field_16;
			char field_17;
			int field_18;
			short field_1C;
			char field_1E;
			char field_1F;
			char field_20;
			char field_21;
			char field_22;
			char field_23;
		};
	}
}