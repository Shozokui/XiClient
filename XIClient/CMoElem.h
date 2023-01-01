#pragma once
#include "CMoOtTask.h"
#include "LittleStruct.h"
#include "WMatrix.h"
#include "BlendStruct.h"
namespace FFXI {
	namespace CYy {
		class CMoResource;
		class CYyGenerator;
		class CMoElem : public CMoOtTask {
		public:
			static const CYyClass CMoElemClass;
			static char* ElemAllocate(int, unsigned char);
			static D3DCOLOR SomeColor;
			static float Floats[4];
			virtual const CYyClass* GetRuntimeClass() override;
			CMoElem();
			virtual ~CMoElem();
			virtual void VObj2(int*) override;
			virtual char OnMove() override;
			virtual void VirtOt1() override;
			virtual bool VirtElem1(FFXI::Constants::Enums::ElemType);
			virtual void VirtElem2(float);
			virtual void VirtElem3(unsigned short);
			virtual int VirtElem4();
			virtual bool IsNever(); //Virt Elem 5
			virtual double VirtElem6();
			virtual void OnDraw(); //Virt Elem7
			virtual void OnCalc() final; //VirtElem8
			virtual void VirtElem9();
			void UnPrepDX(unsigned short);
			void PrepDX(unsigned short);
			void LinkToGenerator(CYyGenerator*);
			void DoBlend(bool);
			D3DXVECTOR3* GetFitPosScale();
			D3DXVECTOR3* GetFitModelScale();
			FFXI::Math::WMatrix* GetSomeMatrix();
			FFXI::Math::WMatrix* GetProcOrCModMatrix();
			void GetWorldPos(D3DXVECTOR3*);
			bool CheckSomething();
			void CheckSomethingWasTrue();
			void UpdateField34Stuff(float);
			bool ExecTag();
			void CalcTrans();
			void CalcMatrix();
			void CalcRotAhead(D3DXVECTOR3);
			void SomeMatrixCalc();
			D3DXVECTOR3 field_54;
			FFXI::Math::WMatrix field_60;
			FFXI::Math::WMatrix field_A0;
			D3DXVECTOR3 field_E0;
			D3DXVECTOR3 field_EC;
			FFXI::Util::BlendStruct field_F8;
			CYyGenerator** field_FC;
			CYyGenerator** field_100;
			CMoResource** res;
			FFXI::Math::WMatrix* field_108;
			unsigned int field_10C;
			float Life;
			float field_114;
			float field_118;
			float field_11C;
			float field_120;
			float field_124;
			float field_128;
			float field_12C;
			int field_130;
			float field_134;
			float field_138;
			float field_13C;
			float field_140;
			float field_144;
			float field_148;
			CMoElem* field_14C;
			CMoElem* field_150;
			int field_154;
			int field_158;
			int field_15C;
			int field_160;
			int field_164;
			float field_168;
			unsigned short field_16C;
			short field_16E;
			unsigned short field_170;
			short ModelType;
			short field_174;
			short field_176;
			char field_178;
			char MMBFlag;
			char field_17A;
			//This is out of order to preserve alignment
			char field_187;
			unsigned int field_17B;
			LittleStruct field_17F;
			D3DXVECTOR3 field_188;
		};
	}
}