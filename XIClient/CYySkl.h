#pragma once
#include "StorageListObject.h"

namespace FFXI {
	namespace Math { class WMatrix; }
	namespace CYy {
		class CMoSk2;
		class CYySkl : public StorageListObject {
		public:
			static const CYyClass CYySklClass;
			static CMoSk2* g_sk2;
			static FFXI::Math::WMatrix* g_mat;
			static unsigned char* g_obj;
			static int g_count;
			static int g_arr[256];
			virtual const CYyClass* GetRuntimeClass() override final;
			CYySkl();
			~CYySkl();
			void Init();
			bool ResetFlagArray();
			short* GetSomeShortPtr(unsigned int);
			float* GetSomeFloatPtr();
			short* GetShortPtrBase();
			void RecurseSetFlags(CMoSk2*, int, int);
			void DoingSomething(FFXI::Math::WMatrix*, int, int, char*);
			bool SetStatics();
			void SetStatics2();
			CMoSk2** Resource;
			void* field_8;
			FFXI::Math::WMatrix* field_C;
			char field_10;
			char field_11;
			__int16 field_12;
			unsigned char* field_14;
		};
	}
}