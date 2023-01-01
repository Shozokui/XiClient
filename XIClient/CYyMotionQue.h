#pragma once
#include "CYyQue.h"
namespace FFXI {
	namespace CYy {
		class CMoMo2;
		class CYyMotionQue : public CYyQue {
		public:
			static const CYyClass CYyMotionQueClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			virtual void VirtQue1() override final;
			virtual int GetResID() override final;
			virtual void* GetMod() override final;
			virtual void SetMod(void*) override final;
			virtual void SetSpeed(float) override final;
			virtual float GetFrame() override final;
			virtual void UpdateFrame() override final;
			virtual void CalcMotion(int, int, void*, CYyQue*) override final;
			virtual bool IsZombi() override final;
			CYyMotionQue();
			~CYyMotionQue();
			float Frame;
			float Speed;
			int field_2C;
			int field_30;
			CMoMo2** field_34;
			float field_38;
			int field_3C;
		};
	}
}