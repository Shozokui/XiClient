#pragma once
#include "CMoResource.h"
#include "CMoAttachments.h"
namespace FFXI {
	namespace CYy {
		class CYyGeneratorClone;
		class CXiActor;
		class GeneratorTask;
		class CMoElem;
		class CYyGenerator : public CMoResource , public CMoAttachments {
		public:
			static bool some_generator_scalar_flag;
			static float GetSomeGeneratorScalar();
			CYyGenerator();
			virtual ~CYyGenerator();
			virtual void ConstructFromData(char*) override final;
			virtual void Open() override final;
			virtual void Close() override final;
			virtual FFXI::Math::WMatrix* VRes4() override final;
			virtual bool VirtAttach3() override;
			virtual bool VirtAttach4() override;
			void* GetModel();
			unsigned char GetModelType();
			unsigned char* FindFirst(int);
			unsigned char* GetElemStatus();
			bool IsNever();
			int GetLife();
			void Unlink(bool);
			void Preset();
			bool AttachCalc();
			void Activate();
			void MaybeDeactivate();
			void Tracking(FFXI::Math::WMatrix*);
			void Tracking(unsigned int*, FFXI::Math::WMatrix*);
			bool Idle(CMoElem*);
			unsigned int* IdleFindFirst(unsigned int);
			void ElemGenerate(CMoElem*);
			void ElemIdle(CMoElem*);
			void ElemDie(CMoElem*);
			void KillAll(CYyGenerator**);
			void DeleteClone(CXiActor*, CXiActor*, int, CYyGenerator**);
			CYyGeneratorClone* Clone(CXiActor*, CXiActor*, int);
			void Copy(CYyGenerator*);
			void GetPos(D3DXVECTOR3*);
			bool CheckFlag29();
			void GenUnk1();
			float field_70;
			float field_74;
			D3DXVECTOR3 PosScale;
			D3DXVECTOR3 ModelScale;
			float field_90;
			int field_94;
			unsigned char* field_98;
			int field_9C;
			unsigned int field_A0;
			FFXI::Math::WMatrix* field_A4;
			GeneratorTask* field_A8;
			int field_AC;
			CMoElem* field_B0;
			int field_B4;
			int field_B8;
			int field_BC;
			int field_C0;
			int field_C4;
			float field_C8;
			float field_CC;
			float field_D0;
			short field_D4;
			short field_D6;
			unsigned int flags;
			short field_DC;
			char field_DE;
			char field_DF;
			unsigned int* Script1;
			unsigned int* Script2;
			unsigned int* Script3;
			unsigned int* Script4;
		};
	}
}