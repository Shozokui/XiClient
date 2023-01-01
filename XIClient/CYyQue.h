#pragma once
#include "StorageListObject.h"
namespace FFXI {
	namespace CYy {
		class CYyQue : public StorageListObject {
		public:
			static const CYyClass CYyQueClass;
			virtual const CYyClass* GetRuntimeClass() override;
			CYyQue();
			virtual ~CYyQue();
			void Kill(float);
			CYyQue* Previous;
			char field_8;
			char field_9;
			char field_A;
			char field_B;
			float field_C;
			int field_10;
			float field_14;
			float field_18;
			float field_1C;
			float field_20;
			virtual void VirtQue1() = 0;
			virtual int GetResID() = 0;
			virtual void* GetMod() = 0;
			virtual void SetMod(void*) = 0;
			virtual void SetSpeed(float) = 0;
			virtual float GetFrame() = 0;
			virtual void UpdateFrame() = 0;
			virtual void CalcMotion(int, int, void*, CYyQue*) = 0;
			virtual bool IsZombi() = 0; //VirtQue9
			virtual void ControlBlendWeight();
		};
	}
}