#pragma once
#include "CIwMenu.h"
#include "XIString.h"
namespace FFXI {
	namespace CTk {
		class CIwOkMenu : public CIwMenu {
		public:
			CIwOkMenu();
			virtual void OnInitialUpdatePrimitive() override final;
			virtual void OnDrawPrimitive() override final;
			virtual void OnKeyDown(short, short) override final;
			void SetErrorString(const char*);
			void(__cdecl* field_14)(int);
			int field_18;
			char field_1C[0x230];
			FFXI::Text::XIString field_24C;
			char field_258;
			char field_259;
			char field_25A;
			char field_25B;
			int field_25C;
		};
	}
}