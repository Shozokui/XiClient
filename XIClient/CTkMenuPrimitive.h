#pragma once
#include "CTkObject.h"

namespace FFXI {
	namespace CTk {
		class CTkMenuCtrlData;
		class CTkMenuPrimitive : public CTkObject {
		public:
			virtual ~CTkMenuPrimitive() = default;
			virtual void OnInitialUpdatePrimitive();
			virtual void OnDestroyPrimitive();
			virtual void OnRemovePrimitive();
			virtual void OnDrawPrimitive();
			virtual void OnUpdatePrimitive();
			virtual void OnKeyDown(short, short);
			virtual void PrimVirt7();
			virtual void PrimVirt8();
			virtual void OnActive(bool);
			virtual void OnDrawCalc(bool);
			virtual void PrimVirt11();
			virtual void PrimVirt12();
			virtual void PrimVirt13();
			virtual void PrimVirt14();
			virtual void PrimVirt15();
			virtual void PrimVirt16(int);
			CTkMenuCtrlData* MenuCtrlData;
			char field_C;
			char field_D;
			short field_E;
			char field_10;
			char field_11;
			char field_12;
			char field_13;
		};
	}
}