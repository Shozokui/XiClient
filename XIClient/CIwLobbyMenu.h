#pragma once
#include "CIwMenu.h"

namespace FFXI {
	namespace CTk {
		class CIwLobbyMenu : public CIwMenu {
		public:
			CIwLobbyMenu();
			virtual void OnInitialUpdatePrimitive() override final;
			virtual void OnUpdatePrimitive() override final;
			virtual void OnDrawPrimitive() override final;
			virtual void OnKeyDown(short, short) override final;
			void SetHelpText();
			int field_14;
			int ActiveButtonIndex;
			int field_1C;
			int field_20[8];
		};
	}
}