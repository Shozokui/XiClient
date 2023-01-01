#pragma once
#include "StorageListObject.h"
namespace FFXI {
	namespace CYy {
		class CYyMotionQue;
		class MotionManStaticStruct;
		class CMoMo2;
		class CYyMotionQueList : public StorageListObject {
		public:
			static const CYyClass CYyMotionQueListClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CYyMotionQueList();
			~CYyMotionQueList();
			void AppendSync(CMoMo2**, float, float, int, float, float, float, float);
			void Append(CMoMo2**, float, float, int, float, float, float, float);
			void Remove(CYyMotionQue*);
			void DeleteAll();
			int GetNbQue();
			void UpdateFloats(int, MotionManStaticStruct*);
			bool IsExistZombiQue();
			CYyMotionQue* GetParent(CYyMotionQue*);
			CYyMotionQue* GetTail();
			CYyMotionQue* Head;
			float field_8;
			float field_C;
			float field_10;
		};
	}
}