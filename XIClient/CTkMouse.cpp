#include "CTkMouse.h"
#include "_49SubList.h"
#include "TkManager.h"
#include "Globals.h"
#include "CTkMenuMng.h"
#include "CYyDb.h"
#include "CTkMenuMng.h"
#include "CTkMenuCtrlData.h"
#include "CDx.h"
#include "RegistryConfig.h"
#include "CApp.h"
#include "CXiOpening.h"
#include "InputMng.h"
#include "PlaceHolders.h"
#include "CTkInputCtrl.h"
#include "CFsConf6Win.h"
#include "Values.h"
#include <iostream>
#include "RuntimeConfig.h"
using namespace FFXI::CTk;
using namespace FFXI::Constants;

int CTkMouse::SomeFlag{ 0 };

class MouseCursolEntry {
public:
	char ResourceName[32];
	D3DCOLOR SomeColor;
	_49SubList*** ResourceData;
	int field_28;
	float SomeAngle;
	unsigned short field_30;
	short field_32;
	int field_34;
	int field_38;
	int field_3C;
};

MouseCursolEntry mouseCursolTBL[] = {
	{"anc     anc_m1  ", 0x80808080, nullptr, 0, 0.0, 0, 1, 0, 0, 0},
	{"anc     anc_m2  ", 0x80808080, nullptr, 0, 0.0, 0, 1, 0, 0, 0},
	{"anc     anc_m2  ", 0x80808080, nullptr, 0, 0.0, 0, 1, 0, 0, 0},
	{"anc     anc_scro", 0x80808080, nullptr, 0, 0.0, 0, 1, 0, 0, 0},
	{"anc     kaipage ", 0x80808080, nullptr, 0, 0.0, 0, 2, 0, 0, 0},
	{"anc     kaipage ", 0x80202080, nullptr, 0, 0.0, 0, 2, 0, 0, 0},
	{"anc     anc_m1  ", 0x80808080, nullptr, 0, 0.0, 0, 1, 0, 0, 0},
	{"", 0, nullptr, 0, 0, 0, 0, 0, 0, 0}
};

FFXI::CTk::CTkMouse::CTkMouse()
{
	this->field_58 = 0;
	this->field_5C = 0;
	this->field_60 = 0;
	this->field_64 = 0;
	this->field_68 = 0;
	this->field_70 = -999.0;
	this->field_4D = 0;
	this->field_88 = 1;
	this->CamPointX = 0;
	this->CamPointY = 0;
	this->field_94 = 0.0;
	this->field_98 = 0.0;
	this->field_9C = 0;
	this->field_A2 = 0;
	this->field_A4 = 0;
	this->field_A0 = 0;

	int v2 = TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C;
	this->field_4.x = v2 / 2;
	this->field_C = v2 / 2;

	v2 = TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E;
	this->field_4.y = v2 / 2;
	this->field_10 = v2 / 2;

	this->field_40 = TkManager::g_CTkMenuDataList.GetSubLists("anc     anc_scro");
	this->field_4C = 0;
	this->field_44 = 0;
	this->field_80 = 1.0;
	this->field_84 = 0;

	for (int i = 0; i < sizeof(mouseCursolTBL) / sizeof(mouseCursolTBL[0]); ++i) {
		MouseCursolEntry* mce = mouseCursolTBL + i;
		if (mce->ResourceData == nullptr) {
			mce->ResourceData = TkManager::g_CTkMenuDataList.GetSubLists(mce->ResourceName);
		}
	}

	this->field_4E = 0;
	this->ResetMouseMoveCenter();
	this->field_86 = 0;
	this->field_87 = 0;
	this->field_A6 = 0;

	if (FFXI::CYy::CDx::instance)
		FFXI::CYy::CDx::instance->ShowCursor(false);
	
	this->SetMousePoint(&this->field_4, 0.0);

	for (int i = 0; i < sizeof(this->field_34) / sizeof(this->field_34[0]); ++i) {
		this->field_1C[i] = { 0, 0 };
		this->field_34[i] = 0;
	}

	this->field_A8 = 0;
	this->field_AC = 0;
}

FFXI::CTk::CTkMouse::~CTkMouse()
{
}

void FFXI::CTk::CTkMouse::ResetMouseMoveCenter()
{
	this->field_38 = (TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C) / 2;
	this->field_3C = (TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E) / 2;
}

void FFXI::CTk::CTkMouse::UpdateState()
{
	this->field_5C = 0;
	this->field_60 = 0;
	this->field_64 = 0;
	if (!TkManager::g_CTkMenuMng.HideMode) {
		if ((this->field_58 & this->field_68) != 0) {
			if (this->field_70 == -999.0) {
				this->field_70 = 24.0;
			}
			else {
				this->field_70 -= CYyDb::g_pCYyDb->CheckTick();
				if (this->field_70 < 0.0) {
					this->field_70 = 12.0;
					this->field_5C |= this->field_68;
				}
			}
		}
		else {
			this->field_68 = 0;
			this->field_70 = -999.0;
		}
	}
}

void FFXI::CTk::CTkMouse::OnLButtonDown()
{
	int v2 = this->field_60;
	if ((v2 & 1) == 0) {
		this->field_70 = -999.0;
		this->field_58 |= 1;
		this->field_60 |= 1;
		this->field_68 = 1;
		this->field_5C |= 1;
		this->field_1C[0] = this->field_4;
		this->field_34[0] = 1;
		bool v7 = (this->field_58 & 2) != 0;
		if (CFsConf6Win::Check()) {
			if (!v7)
				v7 = (this->field_58 & 4) != 0;
			else return;
		}
		if (!v7) {
			if (false) {//sub
				this->ResetMouseMoveCenter();
			}
			else {
				this->field_38 = this->field_4.x;
				this->field_3C = this->field_4.y;
			}
			if (CFsConf6Win::Check()) {
				if (this->field_4D != 5)
					CTkMouse::SomeFlag = 0;
			}
		}
	}
}

void FFXI::CTk::CTkMouse::OnRButtonDown()
{
	int v2 = this->field_60;
	if ((v2 & 2) != 0)
		return;
	this->field_60 |= 2;
	this->field_68 = 2;
	this->field_58 |= 2;
	this->field_5C |= 2;
	this->field_70 = -999.0;
	this->field_1C[2] = this->field_4;
	this->field_34[1] = 1;
	bool v7 = (this->field_58 & 1) != 0;

	if (CFsConf6Win::Check()) {
		if (!v7) 
			v7 = (this->field_58 & 4) != 0;
	}
	if (!v7) {
		if (false) { //sub //TODO
			TkManager::g_pCTkMouse->ResetMouseMoveCenter();
		}
		else {
			this->field_38 = this->field_4.x;
			this->field_3C = this->field_4.y;
		}
	}
	
	if (CFsConf6Win::Check()) {
		if (this->field_4D != 5)
			CTkMouse::SomeFlag = 1;
	}
}

void FFXI::CTk::CTkMouse::OnLButtonUp()
{
	if ((this->field_64 & 1) == 0) {
		this->field_58 ^= 1;
		this->field_64 |= 1;
	}
}

void FFXI::CTk::CTkMouse::OnRButtonUp()
{
	if ((this->field_64 & 2) == 0) {
		this->field_58 ^= 2;
		this->field_64 |= 2;
	}
}

void FFXI::CTk::CTkMouse::OnMButtonDown()
{
	if ((this->field_60 & 4) != 0)	return;

	this->field_60 |= 4;
	this->field_68 = 4;
	this->field_58 |= 4;
	this->field_5C |= 4;
	this->field_70 = -999.0;
	this->field_1C[3] = this->field_4;
	this->field_34[2] = 1;
	bool v7 = (this->field_58 & 1) != 0;

	if (!CFsConf6Win::Check()) return;
	if ((this->field_58 & 3) != 0) return;

	if (false) {//sub //todo //GetConfig144
		this->ResetMouseMoveCenter();
	}
	else {
		this->field_38 = this->field_4.x;
		this->field_3C = this->field_4.y;
	}

	this->field_4D = 4;
}

void FFXI::CTk::CTkMouse::OnMButtonUp()
{
	if ((this->field_64 & 4) == 0) {
		this->field_58 ^= 4;
		this->field_64 |= 4;
	}
}

void FFXI::CTk::CTkMouse::OnTrgButtons(bool a1)
{
	if (a1) {
		this->SetDrawStatus(1);
	}
	else {
		switch (this->field_4D) {
		case 3:
		case 6:
			this->CheckActiveWindow();
			break;
		case 4:
		case 5:
			this->SetDrawStatus(1);
			return;
		default:
			break;
		}
		this->SetDrawStatus(0);
	}
}

void FFXI::CTk::CTkMouse::RequestJumpWindow()
{
	tagPOINT point{};
	this->field_48 = 0x41900000;
	if (TkManager::g_CTkMenuMng.HaveActiveWindow()) {
		if (TkManager::g_CTkMenuMng.CurMCD->GetTargetButtonPoint(&point))
			this->SetMousePoint(&point, 0.0);
		
	}
}

void FFXI::CTk::CTkMouse::CheckActiveWindow()
{
	if (TkManager::g_CTkMenuMng.HaveActiveWindow())
		this->field_4D = 2;
	else
		this->field_4D = 1;
}

void FFXI::CTk::CTkMouse::SetMousePoint(tagPOINT* a2, float a3)
{
	if (!this) return;

	if (this->field_4D < 3u || this->field_4D > 6u) {
		this->field_4 = *a2;
		if (a3 != 0.0)
			this->field_48 = a3;
		if ((FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::Fullscreen ||
			FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::BorderlessFullscreen) &&
			FFXI::CYy::CDx::instance &&
			Config::MainRegistryConfig::HardwareMouse &&
			FFXI::CYy::CApp::g_pCApp &&
			FFXI::CYy::CApp::g_pCApp->field_1C == 1)
		{
			double x = this->field_4.x;
			double y = this->field_4.y;
			RECT Rect{};
			if (FFXI::Config::RuntimeConfig::instance.window_mode != FFXI::Config::RuntimeConfig::WindowMode::Fullscreen) {
				GetWindowRect(FFXI::CYy::CApp::g_pCApp->hWnd, &Rect);
				int width = Rect.right - Rect.left;
				int height = Rect.bottom - Rect.top;

				double v10 = x / (double)(TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C);
				double v11 = y / (double)(TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E);

				int cxframe = GetSystemMetrics(SM_CXFRAME);
				int cyframe = GetSystemMetrics(SM_CYFRAME);
				int cycap = GetSystemMetrics(SM_CYCAPTION);
				if (FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::BorderlessFullscreen) {
					cxframe = 0;
					cyframe = 0;
					cycap = 0;
				}

				x = (double)(width - 2 * cxframe) * v10 + (double)Rect.left + cxframe;
				y = (double)(height - 2 * cyframe - cycap) * v11 + (double)Rect.top + cycap + cyframe;
			}
			SetCursorPos(x, y);
		}
	}
}

void FFXI::CTk::CTkMouse::DrawCursor()
{
	if (!this->field_4E) return;

	MouseCursolEntry* mce = mouseCursolTBL + this->field_4C;
	_49SubList*** resdata = mce->ResourceData;
	_49SubList* sublist{ nullptr };
	if (resdata) {
		sublist = (*resdata)[mce->field_30];
		if (sublist)
			mce->field_30 += 1;
		else {
			mce->field_30 = 0;
			sublist = **resdata;
		}
	}

	if (Config::MainRegistryConfig::HardwareMouse) {
		if (FFXI::CYy::CDx::instance) {
			FFXI::CYy::CDx::instance->SetFromCursors(this->field_4C);
			FFXI::CYy::CDx::instance->ShowCursor(true);
		}
	}
	else {
		if (FFXI::CYy::CDx::instance)
			FFXI::CYy::CDx::instance->ShowCursor(false);
		FFXI::Math::WMatrix v11{};
		v11.Identity();
		v11.RotateZ(mouseCursolTBL[this->field_4C].SomeAngle);
		//this not used?
		if (sublist)
			sublist->DrawRotate(this->field_C, this->field_10, &v11, mouseCursolTBL[this->field_4C].SomeColor, 0);
	}

	if (this->field_4D == 6)
		(*this->field_40)[this->field_44]->ExtraDraw(this->field_1C[3].x, this->field_1C[3].y, 1.0, 1.0, 0x80808080, 0, 0, 0);
}

void FFXI::CTk::CTkMouse::SetDrawStatus(unsigned char a2)
{
	if (FFXI::Config::MainRegistryConfig::HardwareMouse) {
		if (FFXI::CYy::CDx::instance)
FFXI::CYy::CDx::instance->ShowCursor(a2);

if (this->field_4E != a2 && a2 == 1) {
	if (FFXI::CYy::CDx::instance)
		FFXI::CYy::CDx::instance->SetFromCursors(this->field_4C);
}

	}
	else {
	if (FFXI::CYy::CDx::instance)
		FFXI::CYy::CDx::instance->ShowCursor(0);
	}

	this->field_4E = a2;
}

void SetA6Helper() {
	float angle = mouseCursolTBL[5].SomeAngle;
	CTkMouse* mouse = TkManager::g_pCTkMouse;

	if (angle < -Values::ANGLE_7PI_OVER_8)
		mouse->field_A6 = 10;
	else if (angle < -Values::ANGLE_5PI_OVER_8)
		mouse->field_A6 = 11;
	else if (angle < -Values::ANGLE_3PI_OVER_8)
		mouse->field_A6 = 12;
	else if (angle < -Values::ANGLE_PI_OVER_8)
		mouse->field_A6 = 13;
	else if (angle < Values::ANGLE_PI_OVER_8)
		mouse->field_A6 = 14;
	else if (angle < Values::ANGLE_3PI_OVER_8)
		mouse->field_A6 = 15;
	else if (angle < Values::ANGLE_5PI_OVER_8)
		mouse->field_A6 = 16;
	else if (angle < Values::ANGLE_7PI_OVER_8)
		mouse->field_A6 = 17;
	else
		mouse->field_A6 = 10;
}

void FFXI::CTk::CTkMouse::OnCalc()
{
	if (TkManager::g_CTkMenuMng.GetHideMode() || FFXI::CYy::CXiOpening::XiMovie)
	{
		this->field_4D = 0;
		this->SetDrawStatus(0);
		return;
	}

	this->OnCalcHelper1();
	this->OnCalcHelper2();

	if (TkManager::g_CTkMenuMng.IsNoWindow()) {
		if (this->field_4D == 3 || this->field_4D == 6)
			this->CheckActiveWindow();
	}

	CTkMenuCtrlData* mcd = TkManager::g_CTkMenuMng.GetCurMCD();
	void* ScrollBar{ nullptr };
	if (mcd)
		ScrollBar = mcd->ScrollBar;

	int x{}, y{};
	if (this) {
		x = this->field_4.x;
		y = this->field_4.y;
	}

	TkManager::g_pCTkInputCtrl->SetSomeCoords(x, y);
	if (this->field_48 <= 0.0) {
		this->field_C = this->field_4.x;
		this->field_10 = this->field_4.y;
	}
	else {
		this->field_48 -= CYyDb::g_pCYyDb->CheckTick();
		if (this->field_48 < 0)
			this->field_48 = 0;
		else {
			this->field_C += (this->field_4.x - this->field_C) / 2;
			this->field_10 += (this->field_4.y - this->field_10) / 2;
		}
	}

	if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON, Enums::TRGTYPE::TRGTYPE_1)
		&& TkManager::g_CTkMenuMng.HaveActiveWindow()
		&& mcd
		&& mcd->IsOnWindow(this->field_4)
		&& !TkManager::g_CTkMenuMng.IsNoWindow())
		this->field_54 = 1;

	if (!Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON, Enums::TRGTYPE::TRGTYPE_4)) {
		this->field_54 = 0;
		this->field_55 = 0;
	}
	
	bool v114 = false;
	switch (this->field_4D) {
	case 0:
		if (this->field_14 || this->field_18)
			this->CheckActiveWindow();
		break;
	case 1:
	case 2:
		this->CheckActiveWindow();
		if (this->field_4D == 2) {
			if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON, Enums::TRGTYPE::TRGTYPE_1)) {
				if (ScrollBar)
					throw "DO THIS";
				else {
					this->field_74 = 25.0;
					this->field_86 = 0;
				}
			}
			if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON, Enums::TRGTYPE::TRGTYPE_5))
				this->field_86 = 0;
			if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_MBUTTON, Enums::TRGTYPE::TRGTYPE_1)) {
				if (mcd) {
					if (ScrollBar)
						throw "DO THIS";
				}
			}
		}
		else {
			if (!Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_RBUTTON, Enums::TRGTYPE::TRGTYPE_5)
				|| TkManager::g_pCTkInputCtrl && false
				|| true || true) {
				//sub //TODO controlactor
			}
			else {
				//sub //TODO autorun
			}
		}
		break;
	case 3:
		if (!Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON, Enums::TRGTYPE::TRGTYPE_4))
			this->CheckActiveWindow();
		break;
	case 4: {
		Enums::KEYTYPE v25 = Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON;
		if (CFsConf6Win::Check())
			v25 = Enums::KEYTYPE::KEYTYPE_MOUSE_MBUTTON;
		if (!Input::InputMng::GetKey(v25, Enums::TRGTYPE::TRGTYPE_4)) {
			this->CheckActiveWindow();
		}
		else {
			if (CFsConf6Win::Check() && this->field_AC) {
				mouseCursolTBL[4].SomeAngle = -this->field_A8 - Values::ANGLE_PI_OVER_2;
			}
			else {
				double xf = (int)this->GetMoveH();
				double yf = (int)this->GetMoveV();

				if (this->field_98 || this->field_94)
					mouseCursolTBL[4].SomeAngle = atan2((int)xf, (int)yf) - Values::ANGLE_PI_OVER_2;
				else
					mouseCursolTBL[4].SomeAngle = atan2(xf, yf) - Values::ANGLE_PI_OVER_2;
			}
		
			
			if (mouseCursolTBL[4].SomeAngle < -Values::ANGLE_PI)
				mouseCursolTBL[4].SomeAngle += Values::ANGLE_2PI;
			else if (mouseCursolTBL[4].SomeAngle > Values::ANGLE_PI)
				mouseCursolTBL[4].SomeAngle -= Values::ANGLE_2PI;

			if (Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_RBUTTON, Enums::TRGTYPE::TRGTYPE_1)) {
				//sub //todo //control actor // autorunning
			}
		}
	}
		break;
	case 5:
	{
		bool Key = Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_RBUTTON, Enums::TRGTYPE::TRGTYPE_4);
		if (!Key && CFsConf6Win::Check())
			Key = Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON, Enums::TRGTYPE::TRGTYPE_4);

		if (!Key) {
			this->CheckActiveWindow();
		}
		else {
			int v45 = this->field_4.y - this->field_3C;
			int v46 = this->field_4.x - this->field_38;
			mouseCursolTBL[5].SomeAngle = atan2(v45, v46) - Values::ANGLE_PI_OVER_2;
			if (!v46 && !v45) {
				mouseCursolTBL[5].SomeAngle = -Values::ANGLE_PI_OVER_2;
				this->field_A6 = 12;
			}
			else {
				if (mouseCursolTBL[5].SomeAngle < -Values::ANGLE_PI)
					mouseCursolTBL[5].SomeAngle += Values::ANGLE_2PI;
				else if (mouseCursolTBL[5].SomeAngle > Values::ANGLE_PI)
					mouseCursolTBL[5].SomeAngle -= Values::ANGLE_2PI;
				SetA6Helper();
			}
		}
	}
	break;
	case 6: {
		float v88 = abs(this->field_80);
		int v87 = (int)v88;
		if (this->field_7C <= 0.0 && v87 != 0) {
			this->field_44 += 1;
			this->field_7C = v88 * 3.0;
			if (!(*this->field_40)[this->field_44])
				this->field_44 = 0;
			if (v87 == 8) {
				if (this->field_80 <= 0.0) {
					if (this->field_80 < 0.0)
						TkManager::g_CTkMenuMng.OnKeyDown(24);
				}
				else
					TkManager::g_CTkMenuMng.OnKeyDown(25);
			}
			else if (this->field_80 <= 0.0) {
				if (this->field_80 < 0.0)
					TkManager::g_CTkMenuMng.OnKeyDown(22);
			}
			else
				TkManager::g_CTkMenuMng.OnKeyDown(23);
		}
		else {
			int v96 = CYyDb::g_pCYyDb->CheckTick();
			this->field_7C -= (double)v96;
			int v100 = this->field_4.y - this->field_1C[3].y;
			int v103 = abs(v100);
			int v99 = 1;
			int v102 = TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E;
			if (v100 < 0)
				v99 = -1;
			this->field_80 = 0.0;
			double v104 = 0.0;
			if (v103 > v102 / 4)
				this->field_80 = (double)(8 * v99);
			else if (v103 > v102 / 10)
				this->field_80 = (double)(3 * v99);
			else if (v103 > v102 / 20)
				this->field_80 = (double)(5 * v99);
		}
		if (this->field_0 != TkManager::g_CTkMenuMng.GetCurMCD()
			|| Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON, Enums::TRGTYPE::TRGTYPE_1)
			|| Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_RBUTTON, Enums::TRGTYPE::TRGTYPE_1)
			|| Input::InputMng::GetKey(Enums::KEYTYPE::KEYTYPE_MOUSE_MBUTTON, Enums::TRGTYPE::TRGTYPE_1)) {
			this->CheckActiveWindow();
		}
	}

	}

	if (this->field_4D == 2 && this->field_4E) {
		int v105 = this->field_50;
		short v106 = this->field_52;
		this->field_50 = 0;
		this->field_52 = -1;
		if (TkManager::g_CTkMenuMng.HaveActiveWindow() && !TkManager::g_CTkMenuMng.IsNoWindow() && mcd) {
			if (ScrollBar)
				throw "DO THIS";
			this->field_52 = mcd->IsOnButton(this->field_4, &v114);
			if (this->field_52 > 0 && this->field_52 != v106)
				mcd->SetCursol(this->field_52, 0);
			int v110 = 1;
			if (v105 > 0)
				v110 = -1;
			while (v105) {
				v105 += v110;
				if (v110 <= 0)
					mcd->OnKeyDown(22);
				else
					mcd->OnKeyDown(23);
			}
		}
	}
	
	if (this->field_4D == 3 && this->field_4E) {
		if (TkManager::g_CTkMenuMng.HaveActiveWindow()
			&& this->field_0 == mcd
			&& ScrollBar) {
			throw "DO THIS";
		}
		else {
			this->CheckActiveWindow();
			this->field_0 = nullptr;
		}
	}
	
	this->field_4C = 0;
	switch (this->field_4D) {
	case 0:
		this->field_4C = 0;
		break;
	case 1:
		if (!TkManager::g_CTkMenuMng.IsNoWindow() && this->field_4E && false)//sub //todo
			this->field_4C = 2;
		break;
	case 2:
		if (!TkManager::g_CTkMenuMng.IsNoWindow() && v114)
			this->field_4C = 1;
		break;
	case 3:
		if (!TkManager::g_CTkMenuMng.IsNoWindow())
			this->field_4C = 3;
		break;
	case 4:
		this->field_4C = 4;
		break;
	case 5:
		this->field_4C = 5;
		break;
	case 6:
		if (!TkManager::g_CTkMenuMng.IsNoWindow())
			this->field_4C = 6;
		break;
	}
	if (this->field_4E == 0) {
		if (FFXI::CYy::CDx::instance) {
			if (Config::MainRegistryConfig::HardwareMouse)
				SetCursor(0);
		}
	}
}

void FFXI::CTk::CTkMouse::OnCalcHelper1()
{
	if (this->field_4D == 0) {
		this->field_78 = 25.0f;
		return;
	}
	else if (this->field_4D == 5) {
		if (!this->field_88)
			this->field_4D = 1;
		return;
	}
	else if (this->field_4D > 2) {
		this->field_78 = 25.0;
		return;
	}

	int v3 = 1;
	bool Key = FFXI::Input::InputMng::GetKey(Constants::Enums::KEYTYPE::KEYTYPE_MOUSE_RBUTTON, Constants::Enums::TRGTYPE::TRGTYPE_4);
	if (CFsConf6Win::Check()) {
		if (!Key)
			Key = FFXI::Input::InputMng::GetKey(Constants::Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON, Constants::Enums::TRGTYPE::TRGTYPE_4);
		v3 = CTkMouse::SomeFlag;
	}

	if (this->field_78 <= 0.0f || !Key)
		this->field_78 = 25.0;
	else
		this->field_78 -= CYyDb::g_pCYyDb->CheckTick();

	if (Key) {
		if (this->field_34[v3]) {
			int v10 = this->field_4.x - this->field_1C[v3].x;
			int v11 = this->field_4.y - this->field_1C[v3].y;
			if (((v10 * v10 + v11 * v11) > 64 ||
				this->field_78 <= 0.0) &&
			!this->field_54 && !this->field_55 &&
			(Placeholder::g_pStDancer || FFXI::GlobalStruct::g_GlobalStruct.field_0 == 96 || FFXI::GlobalStruct::g_GlobalStruct.field_0 == 208)) {
				if (Placeholder::g_pStDancer)
					throw "CHANGE THIS";
				if (this->field_88)
					this->field_4D = 5;
			}
		}
	}
}

void FFXI::CTk::CTkMouse::OnCalcHelper2()
{
	if (this->field_4D == 0) {
		this->field_74 = 25.0;
		return;
	}
	else if (this->field_4D == 4) {
		if (!this->field_88)
			this->field_4D = 1;
		this->field_74 = 25.0;
		return;
	}
	else if (this->field_4D > 2) {
		this->field_74 = 25.0;
		return;
	}

	Constants::Enums::KEYTYPE v3 = Constants::Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON;
	int v4 = 0;
	if (CFsConf6Win::Check()) {
		v3 = Constants::Enums::KEYTYPE::KEYTYPE_MOUSE_MBUTTON;
		v4 = 2;
	}

	bool Key = Input::InputMng::GetKey(v3, Constants::Enums::TRGTYPE::TRGTYPE_4);

	if (this->field_74 <= 0.0 || !Key)
		this->field_74 = 25.0;
	else
		this->field_74 -= CYyDb::g_pCYyDb->CheckTick();

	if (Key) {
		if (this->field_34[v4]) {
			int v10 = this->field_4.x - this->field_1C[v4].x;
			int v11 = this->field_4.y - this->field_1C[v4].y;
			if (((v10 * v10 + v11 * v11) > 64 || this->field_74 <= 0.0) &&
				!this->field_54 && !this->field_55 && 
				FFXI::GlobalStruct::g_GlobalStruct.field_0 == 96) {
				if (this->field_88) 
					this->field_4D = 4;
			}
		}
	}
}

void FFXI::CTk::CTkMouse::OnMouseMove(tagPOINT* a2, short a3)
{
	if (CFsConf6Win::Check() && !TkManager::g_CTkMenuMng.HaveActiveWindow())
		throw "DO THIS";

	this->UpdatePos(a2, a3);
}

void FFXI::CTk::CTkMouse::UpdatePos(tagPOINT* a2, short a3)
{
	this->field_50 = a3;
	if (!TkManager::g_CTkMenuMng.GetHideMode() && (a2->x || a2->y)) {
		if (FFXI::CYy::CDx::instance)
			FFXI::CYy::CDx::instance->ShowCursor(true);
		this->field_48 = 0.0;
		this->SetDrawStatus(1);
	}
	this->field_4.x += a2->x;
	this->field_4.y += a2->y;
	this->field_14 = a2->x;
	this->field_18 = a2->y;
	if (this->field_4.x < 0)
		this->field_4.x = 0;
	if (this->field_4.y < 0)
		this->field_4.y = 0;
	if (this->field_4.x > TkManager::g_CTkMenuMng.UIXRes)
		this->field_4.x = TkManager::g_CTkMenuMng.UIXRes - 1;
	if (this->field_4.y > TkManager::g_CTkMenuMng.UIYRes)
		this->field_4.y = TkManager::g_CTkMenuMng.UIYRes - 1;
}

float FFXI::CTk::CTkMouse::GetMoveH()
{
	double result = (double)(this->field_4.x - this->field_38) / ((double)(short)(TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C) * 0.047619049);
	if (result >= 1.0)
		return 1.0;

	if (result <= -1.0)
		return -1.0;

	return 0.0;
}

float FFXI::CTk::CTkMouse::GetMoveV()
{
	double result = (double)(this->field_4.y - this->field_3C) / ((double)(short)(TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E) * 0.047619049);
	if (result >= 1.0)
		result = 1.0;
	else if (result <= -1.0)
		result = -1.0;
	else
		result = 0.0;

	//sub //todo
	if (CFsConf6Win::Check() && result == 0.0 &&
		(this->field_58 & 4) != 0 &&
		this->GetMoveH() == 0.0)
		return -1.0;

	return result;
}

bool FFXI::CTk::CTkMouse::CheckMouse()
{
	return this->field_4D == 2 && !Placeholder::IsEvent(0);
}
