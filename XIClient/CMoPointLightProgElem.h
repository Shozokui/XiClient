#pragma once
#include "CMoElem.h"

namespace FFXI {
	namespace CYy {
		class CMoPointLightProgElem : public CMoElem {
		public:
			static const CYyClass CMoPointLightProgElemClass;
			const CYyClass* GetRuntimeClass() override final;
			CMoPointLightProgElem(int, int);
			virtual ~CMoPointLightProgElem();
			virtual bool VirtElem1(FFXI::Constants::Enums::ElemType) override final;
			virtual double VirtElem6() override final;
			virtual void OnDraw() override final;
			int field_194;
			int field_198;
			float field_19C;
			float field_1A0;
			float field_1A4;
			float field_1A8;
			int field_1AC;
			char field_1B0;
			char field_1B1;
			char field_1B2;
			char field_1B3;
		};
	}
}