#include "FsMenuDraw.h"
#include "TkManager.h"
#include "CTkInputCtrl.h"
#include "InputMng.h"
#include "Enums.h"
#include "CTkMenuMng.h"

using namespace FFXI::CTk;
using namespace FFXI::Constants;
void FFXI::CTk::FsMenuDraw::onMenuKeyDispatch()
{
	if (!this->field_0) return;

	if (TkManager::g_pCTkInputCtrl->field_C4 && false) return; //sub //TODO //FsTextInput

	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_L_UP, Enums::TRGTYPE::TRGTYPE_0))
		TkManager::g_CTkMenuMng.OnKeyDown(2);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_L_DOWN, Enums::TRGTYPE::TRGTYPE_0))
		TkManager::g_CTkMenuMng.OnKeyDown(1);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_L_LEFT, Enums::TRGTYPE::TRGTYPE_0))
		TkManager::g_CTkMenuMng.OnKeyDown(4);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_L_RIGHT, Enums::TRGTYPE::TRGTYPE_0))
		TkManager::g_CTkMenuMng.OnKeyDown(3);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_R_RIGHT, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(5);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_R_DOWN, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(6);
	if (Input::InputMng::GetKey(62, Enums::KEYTYPE::KEYTYPE_PAD_R_DOWN, Enums::TRGTYPE::TRGTYPE_1, 0))
		TkManager::g_CTkMenuMng.OnKeyDown(30);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_RBUTTON, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(30);
	if (Input::InputMng::GetKey(62, Enums::KEYTYPE::KEYTYPE_PAD_R_DOWN, Enums::TRGTYPE::TRGTYPE_1, 1))
		TkManager::g_CTkMenuMng.OnKeyDown(31);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_R2, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(7);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_R_UP, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(8);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_R1, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(17);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_L1, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(21);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_L1_COPY1, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(18);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_L2, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(10);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_R2_COPY1, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(11);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_R_UP_COPY1, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(12);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_R1_COPY1, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(14);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_R_LEFT, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(15);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_SELECT, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(16);
	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_PAD_SELECT_COPY1, Enums::TRGTYPE::TRGTYPE_1))
		TkManager::g_CTkMenuMng.OnKeyDown(19);

}
