#include "CTkMenuMng.h"
#include "CMoResource.h"
#include "CYyDb.h"
#include "Globals.h"
#include "CMoResourceMng.h"
#include "CTkMenuCtrlData.h"
#include "CYyTexMng.h"
#include "CTkMsbDataList.h"
#include "CTkMsbData.h"
#include "TkManager.h"
#include "CTkMenuData.h"
#include "PrimMng.h"
#include "CTkMenuPrimitive.h"
#include "CTkMouse.h"
#include "CTkDrawCtrlFrame.h"
#include "CDx.h"
#include "CTkEventMsg.h"
#include "CTkInputCtrl.h"
#include "PlaceHolders.h"
#include "TkRect.h"
#include "InputMng.h"
#include "CYyResourceFile.h"
#include "SoundMng.h"
#include "CFsConf6Win.h"

using namespace FFXI::CTk;
using namespace FFXI::Constants;

const char* LanguageDependentMenuTable[] = {
	"menu    frames  ",
	"menu    framesus",
	"menu    framesde",
	"menu    framesfr",
	"menu    comwin  ",
	"menu    comwin  ",
	"menu    comwinde",
	"menu    comwinfr",
	"menu    resyn   ",
	"menu    resynus ",
	"menu    resynde ",
	"menu    resynfr ",
	"menu    compass ",
	"menu    compass ",
	"menu    compassd",
	"menu    compassf",
	"menu    sc_item ",
	"menu    sc_item_",
	"menu    sc_item_",
	"menu    sc_item_",
	"menu    key1top ",
	"menu    key1topu",
	"menu    key1topu",
	"menu    key1topu",
	"menu    jobcsel ",
	"menu    jobcselu",
	"menu    jobcselu",
	"menu    jobcselu",
	"menu    prty5   ",
	"menu    prty5us ",
	"menu    prty5de ",
	"menu    prty5fr "


};

CTkMenuMng::~CTkMenuMng() {
	this->DeleteMenuAll();
}

const char* FFXI::CTk::CTkMenuMng::GetLanguageDependentMenu(int a1)
{
	if (a1 < 0 || a1 >= Constants::Enums::LanguageDependentMenus::LDM_LAST)
		return nullptr;

	int RegionCode = FFXI::GlobalStruct::g_GlobalStruct.GetRegionCode();
	return LanguageDependentMenuTable[4 * a1 + RegionCode];
}

void FFXI::CTk::CTkMenuMng::Init()
{
	this->UIXRes = 640;
	this->field_62 = 0;
	this->field_AE = 0;
	this->field_B0 = 0;
	this->field_7C = 0;
	this->field_7E = 0;
	this->field_84 = 0;
	this->field_88 = 0;
	this->field_AB = 0;
	this->HideMode = 0;
	this->field_B2 = 0;
	this->field_B3 = 0;
	this->UIYRes = 480;
	
	this->LogWindow = PrimMng::g_pTkDrawMessageWindow;
	this->PartyList = PrimMng::g_pYkWndPartyList;
	this->TargetWindow = PrimMng::g_pTkTarget;
	this->QueryWindow = PrimMng::g_pTkQueryWindow;
	this->LogWindow2 = PrimMng::g_pTkDrawMessageWindow2;

	this->field_AA = 1;
	this->field_B5 = -1;
	this->field_B6 = -1;
	this->ZoneInit();
}

void FFXI::CTk::CTkMenuMng::ZoneInit()
{
	this->field_A0 = 0;
	this->field_A4 = 0;
	this->field_A8 = 0;
	this->field_78 = 30;
	this->field_AD = 0;
}

void FFXI::CTk::CTkMenuMng::DeleteMenuAll()
{
	CTkNode* Head = this->MCDList.GetHeadPosition();
	while (Head) {
		CTkNode* v3 = Head;
		CTkMenuCtrlData* Next = (CTkMenuCtrlData*)this->MCDList.GetNext(&Head);
		this->MCDList.RemoveAt(v3);
		if (Next)
			delete Next;
	}
	this->MCDList.RemoveAll();
	
	Head = this->ResourceMenuList.GetHeadPosition();
	while (Head) {
		CTkNode* v6 = Head;
		CTkMenuData* Next = (CTkMenuData*)this->ResourceMenuList.GetNext(&Head);
		this->ResourceMenuList.RemoveAt(v6);
		if (Next)
			delete Next;
	}
	this->ResourceMenuList.RemoveAll();

	Head = this->ObList3.GetHeadPosition();
	if (Head != nullptr) {
		//sub //TODO
		exit(0x101575E4);
	}
	this->ObList3.RemoveAll();

	if (PrimMng::g_pTkQueryWindow) {
		throw "NOT IMPLEMENTED";
	}

	if (PrimMng::g_pTkGMTellWindow) {
		throw "NOT IMPLEMENTED";
	}
	this->CurMCD = nullptr;
}

void FFXI::CTk::CTkMenuMng::CreateDrawMenu(const char* resName, bool Open, int a4)
{
	char CleanedName[32];
	this->CleanResName(CleanedName, resName);

	bool resFound = false;
	PrimMng::FindPrimMenu(CleanedName, &resFound);

	if (resFound) {
		if (this->IsEnableCreateDrawMenu(CleanedName)) {

			CTkMenuData* v5 = this->GetResourceMenu(CleanedName);
			if (v5) {
				CTkMenuCtrlData* v6 = this->GetFromMCDList(CleanedName);
				if (!v6) {
					v6 = new CTkMenuCtrlData();
					
					if (!v6)
						return;

					PrimMng::MenuPrimitiveLink(v6, CleanedName);
					v6->SetResourceData(v5);
					v6->field_10 = 14;
					if ((v6->field_34 & 0x200000) != 0)
						throw "NOT IMPLEMENTED";
						
					v6->LoadCursolInfo();

					if (v6->MenuPrimitive)
						v6->MenuPrimitive->OnInitialUpdatePrimitive();
						
					if (a4 && CTkMenuMng::IsAreaWindow(v6->field_38, v6, false)) {
						v6->field_75 = 1;
						Open = false;
					}
					
					this->AddDrawMenu(v6);
				}
				else {
					PrimMng::MenuPrimitiveLink(v6, CleanedName);
					v6->LoadCursolInfo();
					if (v6->field_10 != 14 && !v6->field_6E) {
						if (v6->MenuPrimitive) {
							v6->MenuPrimitive->OnInitialUpdatePrimitive();
							v6->MenuPrimitive->OnUpdatePrimitive();
						}
						this->ActiveDrawMenu(v6, 0);
						return;
					}
					v6->ReInit();

					if (v6->MenuPrimitive)
						v6->MenuPrimitive->OnInitialUpdatePrimitive();

					v6->field_6E = 0;
					if (a4 && CTkMenuMng::IsAreaWindow(v6->field_38, v6, false)) {
						v6->field_75 = 1;
						v6->field_76 = 1;
						return;
					}
				}

				if (Open) {
					this->OpenDrawMenu(v6);
					this->ActiveDrawMenu(v6, 0);
				}

			}
		}
	}
}

void FFXI::CTk::CTkMenuMng::DestroyDrawMenu(const char* a2)
{
	char MenuName[32];
	this->CleanResName(MenuName, a2);
	CTkMenuCtrlData* v3 = this->GetFromMCDList(MenuName);
	if (v3)
		this->DestroyDrawMenu(v3, true);
}

void FFXI::CTk::CTkMenuMng::DestroyDrawMenu(CTkMenuCtrlData* a2, char a3)
{
	if (!a2) return;

	if (!a2->field_6E) {
		a2->field_6E = 1;
		a2->DestroyPrimitive();
	}
	if (a2->field_10 == 14)
		a2->field_70 = 1;
	else {
		a2->field_72 = 0;
		this->CloseDrawMenu(a2, a3, 1, 0, 0);
	}
}

void FFXI::CTk::CTkMenuMng::DeleteDrawMenu(CTkMenuCtrlData* a2)
{
	if (!a2) return;

	CTkNode* Tail = this->MCDList.GetTailPosition();
	//Check if mcd is in the list
	if (Tail) {
		while (a2 != (CTkMenuCtrlData*)this->MCDList.GetPrev(&Tail)) {
			if (!Tail)
				return;
		}
	}

	a2->RemoveResourceData();
	this->RemoveMCD(a2);
	a2->RemovePrimitive();
	if (this->PrevMCD == a2)
		this->PrevMCD = nullptr;

	delete a2;
}

void FFXI::CTk::CTkMenuMng::OpenDrawMenu(CTkMenuCtrlData* a2)
{
	if (!a2) return;
	if (a2->field_6E) return;
	if (!a2->MenuData) return;
	if (a2->field_10 != 14) return;

	Input::InputMng::KeyProcessed();
	bool v2{ false };
	if (a2->field_10 == 14 || a2->field_6E) {
		a2->ReInit();
		a2->field_10 = 0;
		a2->field_6E = 0;
		v2 = true;
	}

	CTkMenuPrimitive* PrimMenu = PrimMng::FindPrimMenu(a2->MenuData->field_46, nullptr);
	if (PrimMenu) {
		a2->AddPrimitive();
		a2->InitialSetMessage();
		PrimMenu->OnUpdatePrimitive();
		TkManager::g_pCTkMouse->RequestJumpWindow();
		//sub //TODO
		if ((a2->field_34 & 0x1000) != 0) {
			throw "NOTIMP";
		}
		if ((a2->field_34 & 0x4000) != 0) {
			throw "NOTIMP";
		}
		if ((a2->field_34 & 0x2000) != 0) {
			throw "NOTIMP";
		}
		if ((a2->field_34 & 0x10000000) != 0) {
			throw "NOTIMP";
		}
	} else{
		a2->MenuPrimitive = nullptr;
	}

	if (v2) {
		CTkDrawCtrlFrame* dcFrame = a2->GetDrawCtrlFrame();
		if ((a2->field_34 & 8) != 0) {
			a2->field_68 = 0;
			if (dcFrame)
				dcFrame->OffAnimFrame();
		}
		else {
			a2->field_68 = 1;
			if (dcFrame)
				dcFrame->OnAnimFrame();
		}
	}

	if ((a2->field_34 & 0x20000) != 0) {
		CTkNode* Tail = this->MCDList.GetTailPosition();
		while (Tail) {
			CTkMenuCtrlData* Prev = (CTkMenuCtrlData*)this->MCDList.GetPrev(&Tail);
			if ((Prev->field_34 & 0x40000) != 0) {
				this->DestroyDrawMenu(Prev, 1);
			}
		}
	}
}

void FFXI::CTk::CTkMenuMng::CloseDrawMenu(CTkMenuCtrlData* a2, char a3, char a4, char a5, char a6)
{
	if (!a2) return;

	a2->SaveCursolInfo();

	if (a2->field_10 == 14) {
		if (a6)
			a2->field_75 = 1;
		else
			a2->field_75 = 0;
	}
	else {
		Input::InputMng::KeyProcessed();
		a2->field_73 = a4;
		a2->field_72 = a5;
		a2->ReqWindowClose(a3);

		if ((a2->field_34 & 1) == 0 && !a2->field_72 && TkManager::g_pCTkInputCtrl) {
			TkManager::g_pCTkInputCtrl->field_6A -= a2->field_6F;
			a2->field_6F = 0;
		}

		if (a2 == this->PrevMCD)
			this->PrevMCD = nullptr;

		if (a6)
			a2->field_75 = 1;
		else
			a2->field_75 = 0;
		a2->CloseCallback();
	}
}

void FFXI::CTk::CTkMenuMng::AddDrawMenu(CTkMenuCtrlData* a2)
{
	if (!a2) return;


	if (a2->MaybeDrawLayer >= 5) {
		this->MCDList.AddTail(a2);
	}
	else {
		CTkNode* Tail = this->MCDList.GetTailPosition();
		CTkMenuCtrlData* Prev{ nullptr };
		CTkNode* v5 = Tail;
		while (Tail) {
			v5 = Tail;
			Prev = (CTkMenuCtrlData*)this->MCDList.GetPrev(&Tail);
			if (Prev->MaybeDrawLayer < 5)
				break;
		}
		this->MCDList.InsertAfter(v5, a2);
	}
	
}

void FFXI::CTk::CTkMenuMng::ActiveDrawMenu(CTkMenuCtrlData* a2, char a3)
{
	if (a2) {
		a2->field_71 = this->field_60;
		a2->field_98 = a3;
		++this->field_60;
	}
}

CTkMenuCtrlData* FFXI::CTk::CTkMenuMng::GetDrawNext(CTkNode** a2)
{
	CTkMenuCtrlData* Next = (CTkMenuCtrlData*)this->MCDList.GetNext(a2);
	
	if (!Next || Next->field_70)
		return nullptr;

	return Next;
}

CTkMenuCtrlData* FFXI::CTk::CTkMenuMng::GetDrawPrev(CTkNode** a2)
{
	CTkMenuCtrlData* result = (CTkMenuCtrlData*)this->MCDList.GetPrev(a2);
	if (result)
		return result->field_70 == 0 ? result : nullptr;

	return nullptr;
}

bool FFXI::CTk::CTkMenuMng::IsEnableCreateDrawMenu(char* a1)
{
	int v2 = FFXI::GlobalStruct::g_GlobalStruct.field_0;
	int v13 = v2;

	bool v3 = false;

	char v10{ 0 };
	bool v9{ false };

	if (!PrimMng::PrimTable[0].MenuName[0])
		return v3;

	PrimTableEntry* v5 = PrimMng::PrimTable;

	while (true) {
		if (!strncmp(v5->MenuName, a1, 0x10u))
		{
			switch (v2) {
			case 0:
			case 48:
			case 64:
				if ((v5->SomeByte1 & 8) != 0)
					v3 = 1;
				break;
			case 16:
				if ((v5->SomeByte1 & 1) != 0)
					v3 = 1;
				break;
			case 32:
			case 80:
			case 112:
			case 128:
			case 144:
			case 160:
			case 176:
			case 192:
			case 224:
				v10 = v5->SomeByte1;
				v9 = v10 == -1;
				if (v9)
					v3 = 1;
				break;
			case 96:
				if (!Placeholder::pGlobalNowZone)
					break;

				throw "NOT IMPLEMENTED";

				v3 = 1;
				break;
			case 208:
				v10 = v5->SomeByte1;
				if (this->field_AE) {
					v9 = v10 == -1;
					if (v9)
						v3 = 1;
				}
				else if ((v10 & 0x40) != 0)
					v3 = 1;
				break;
			default:
				break;
			}
		}

		v5 += 1;
		if (!v5->MenuName[0])
			return v3;
		v2 = v13;
	}
	return v3;
}

bool FFXI::CTk::CTkMenuMng::HaveActiveWindow()
{
	return this->CurMCD &&
		this->CurMCD->field_10 != 14 &&
		!this->CurMCD->field_70;
}

void FFXI::CTk::CTkMenuMng::SetHideMode(char a2)
{
	if (!this->field_B2 && !this->field_B3) {
		if (a2 >= 2)
			this->HideMode = 0;
		else
			this->HideMode = a2;
	}
	
}

int FFXI::CTk::CTkMenuMng::GetHideMode()
{
	return this->HideMode;
}

bool FFXI::CTk::CTkMenuMng::IsHideMode()
{
	return this->HideMode == 1;
}

bool FFXI::CTk::CTkMenuMng::IsNoWindow()
{
	return PrimMng::g_pTkEventMsg->field_48 || this->IsHideMode();
}

bool FFXI::CTk::CTkMenuMng::IsCurrentWindow(CTkMenuCtrlData* a2)
{
	if (!a2 || this->CurMCD != a2 || a2->field_10 == 14 || a2->field_70)
		return false;

	return true;
}

bool FFXI::CTk::CTkMenuMng::IsActiveBlink(CTkMenuCtrlData* a2)
{
	if ((a2->field_34 & 4) != 0)
		return false;

	if (a2->field_6B)
		return true;

	CTkMenuCtrlData* v3 = this->CurMCD;
	if (!v3)
		v3 = this->field_5C;

	return v3 == a2;
}

CTkMenuData* FFXI::CTk::CTkMenuMng::GetResourceMenu(char* a1)
{
	CTkNode* Head = this->ResourceMenuList.GetHeadPosition();

	while (true) {
		if (!Head)
			return nullptr;

		CTkMenuData* Next = (CTkMenuData*)this->ResourceMenuList.GetNext(&Head);
		if (Next) {
			if (!strncmp(a1, Next->field_46, sizeof(Next->field_46)))
				return Next;
		}
	}
}

CTkMenuCtrlData* FFXI::CTk::CTkMenuMng::GetFromMCDList(char* a2)
{
	CTkNode* Head = this->MCDList.GetHeadPosition();
	CTkMenuCtrlData* Next{ nullptr };
	while (true) {
		if (!Head)
			return nullptr;

		Next = (CTkMenuCtrlData*)this->MCDList.GetNext(&Head);
		if (Next->MenuData) {
			if (!strncmp(a2, Next->MenuData->field_46, sizeof(Next->MenuData->field_46)))
				break;
		}
	}
	return Next;
}

CTkMenuCtrlData* FFXI::CTk::CTkMenuMng::SearchCurMCD()
{
	CTkNode* Tail = this->MCDList.GetTailPosition();
	if (!Tail) return nullptr;

	CTkMenuCtrlData* Prev{ nullptr };

	while (true) {
		Prev = (CTkMenuCtrlData*)this->MCDList.GetPrev(&Tail);
		if (((Prev->field_34 & 1) == 0 || Prev->field_98) && !Prev->field_6E && !Prev->field_70 && Prev->field_10 != 14)
			break;
		if (!Tail)
			return nullptr;
	}

	return Prev;
}

void FFXI::CTk::CTkMenuMng::SetMenuPhase(int a1, int a2)
{
	if (a2 || this->field_6C <= a1)
		this->field_6C = a1;
}

float FFXI::CTk::CTkMenuMng::Get84(bool a2)
{
	if (a2 || FFXI::GlobalStruct::g_GlobalStruct.field_0 != 0x40)
		return this->field_84;

	return 0.0f;
}

float FFXI::CTk::CTkMenuMng::Get88(bool a2)
{
	if (a2 || FFXI::GlobalStruct::g_GlobalStruct.field_0 != 0x40)
		return this->field_88;

	return 0.0f;
}

void FFXI::CTk::CTkMenuMng::GetSomePosition(tagPOINT* a2, char a3, bool a4)
{
	double xRes = this->UIXRes;
	double yRes = this->UIYRes;
	float xScale = this->Get84(a4) * 0.5;
	float yScale = this->Get88(a4) * 0.5;

	if (a3 == 1) {
		a2->x += (1.0 - xScale) * xRes - 512;
		a2->y += yRes * yScale;
	}
	else if (a3 == 2) {
		a2->x += xRes * xScale;
		a2->y += (1.0 - yScale) * yRes - 448.0;
	}
	else if (a3 == 3) {
		a2->x = (1.0 - xScale) * xRes - 512.0;
		a2->y = (1.0 - yScale) * yRes - 448.0;
	}
	else {
		a2->x += xRes * xScale;
		a2->y += yRes * yScale;
	}
}

void FFXI::CTk::CTkMenuMng::CleanResName(char* out, const char* resName)
{
	while (*resName == ' ')
		++resName;

	memset(out, ' ', 16);
	int resNamelen = strlen(resName);
	if (resNamelen <= 8)
		sprintf_s(out, 32, "menu    %s", resName);
	else if (resNamelen < 32)
		strncpy_s(out, 32, resName, resNamelen);

	memset(out + 16, 0, 16);

	if (this->field_B5)
		return;

	//sub //TODO
	throw "NOT IMPLEMENTED";
}

void FFXI::CTk::CTkMenuMng::FrameLoad(CYy::CYyResourceFile*** a1, int a2)
{
	if (this->field_B8) {
		(*this->field_B8)->SomeClean3();
		CYy::CMoResource::UnlinkFromManager((CYy::CMoResource***)&this->field_B8);
		this->field_B8 = nullptr;
	}

	if (CYyDb::pCYyTexMng)
		CYyDb::pCYyTexMng->field_10 = 1;

	this->field_BC = CYyDb::g_pCYyDb->pCMoResourceMng->LoadNumFileGet49(a2 + 14, &this->field_B8);
	if (CYyDb::pCYyTexMng)
		CYyDb::pCYyTexMng->field_10 = 0;

	{//InlinedThis2
		CTkMsbDataList* lists = &TkManager::g_CTkMenuDataList;
		int v2 = lists->CTkObList::field_10;
		CTkNode* a2 = lists->CTkObList::GetHeadPosition();
		while (v2) {
			CTkMsbData* Next = (CTkMsbData*)lists->CTkObList::GetNext(&a2);
			Next->DoSomething();
			--v2;
		}
	}

	*a1 = this->field_B8;
}

void FFXI::CTk::CTkMenuMng::GetWinCoefficient(float* a2, float* a3)
{
	*a2 = (double)this->UIXRes / 640.0;
	*a3 = (double)this->UIYRes / 480.0;
}

void FFXI::CTk::CTkMenuMng::InitDraw()
{
	CYy::CDx* CDx = FFXI::CYy::CDx::instance;
	CDx->DXDevice->SetRenderState(D3DRS_FOGENABLE, false);
	CDx->DXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	CDx->DXDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_POINT);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)(D3DTSS_ADDRESSU), D3DTADDRESS_WRAP);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)(D3DTSS_ADDRESSV), D3DTADDRESS_WRAP);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	CDx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CDx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CDx->DXDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
}

void FFXI::CTk::CTkMenuMng::EndDraw()
{
	CYy::CDx* CDx = FFXI::CYy::CDx::instance;
	CDx->DXDevice->SetRenderState(D3DRS_FOGENABLE, false);
	CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

void FFXI::CTk::CTkMenuMng::OnCalc()
{
	int v8{ 0 };
	//Check Left Mouse Button down
	if (FFXI::Input::InputMng::GetKey(62, Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON, Enums::TRGTYPE::TRGTYPE_4, -1))
		this->OnLButtonDown();
	if (FFXI::Input::InputMng::GetKey(62, Enums::KEYTYPE::KEYTYPE_MOUSE_LBUTTON, Enums::TRGTYPE::TRGTYPE_5, -1))
		this->OnLButtonUp();
	if (FFXI::Input::InputMng::GetKey(62, Enums::KEYTYPE::KEYTYPE_MOUSE_RBUTTON, Enums::TRGTYPE::TRGTYPE_1, -1))
		this->OnRButtonDown();
	if (FFXI::Input::InputMng::GetKey(62, Enums::KEYTYPE::KEYTYPE_MOUSE_RBUTTON, Enums::TRGTYPE::TRGTYPE_5, -1))
		this->OnRButtonUp();
	FFXI::Input::InputMng::GetKey(62, Enums::KEYTYPE::KEYTYPE_MOUSE_MBUTTON, Enums::TRGTYPE::TRGTYPE_1, -1);
	if (FFXI::Input::InputMng::GetKey(62, Enums::KEYTYPE::KEYTYPE_MOUSE_MBUTTON, Enums::TRGTYPE::TRGTYPE_5, -1))
		this->OnMButtonUp();

	this->InitDraw();
	this->OnPhaseCtrl();
	
	CTkNode* Head = this->MCDList.GetHeadPosition();
	while (Head) {
		CTkMenuCtrlData* Next = (CTkMenuCtrlData*)this->MCDList.GetNext(&Head);
		if (Next && Next->field_70 && Next != this->CurMCD)
		{
			if (Next->field_6F > 0)
			{
				TkManager::g_pCTkInputCtrl->field_6A -= Next->field_6F;
				Next->field_6F = 0;
			}
			this->DeleteDrawMenu(Next);
		}
	}

	Head = this->MCDList.GetHeadPosition();
	while (Head) {
		CTkMenuCtrlData* Next = (CTkMenuCtrlData*)this->MCDList.GetNext(&Head);
		if (!Next->field_6E && Next->field_75 && Next->field_76) {
			this->RemoveMCD(Next);
			CTkNode* HeadPosition = this->MCDList.GetHeadPosition();
			Head = HeadPosition;
			if (HeadPosition) {
				while (true) {
					CTkNode* v7 = HeadPosition;
					CTkMenuCtrlData* mcd = (CTkMenuCtrlData*)this->MCDList.GetNext(&Head);
					if ((mcd->field_38 & Next->field_38) != 0) {
						this->MCDList.InsertBefore(v7, Next);
						break;
					}
					HeadPosition = Head;
					if (!Head) {
						this->MCDList.AddTail(Next);
						break;
					}
				}
			}
			Next->field_76 = 0;
		}
	}
	v8 = 1;
	while (v8 < this->field_60) {
		Head = this->MCDList.GetHeadPosition();
		while (Head) {
			CTkMenuCtrlData* Next = (CTkMenuCtrlData*)this->MCDList.GetNext(&Head);
			if (Next) {
				if (Next->field_71 == v8 && !Next->field_6E) {
					this->RemoveMCD(Next);
					this->AddDrawMenu(Next);
					Next->field_71 = 0;
					if ((Next->field_34 & 1) == 0 || Next->field_98) {
						this->SetCurMCD(Next);
						tagPOINT v38{};
						if (Next->GetTargetButtonPoint(&v38))
							TkManager::g_pCTkMouse->SetMousePoint(&v38, 0.0);
					}
					break;
				}
			}
		}
		++v8;
	}

	this->field_60 = 1;
	CTkMenuCtrlData* v12 = this->SearchCurMCD();
	this->SetCurMCD(v12);

	if (FFXI::GlobalStruct::g_GlobalStruct.field_0 == 96) {
		throw "NOT IMPLEMENTED";
	}

	TkManager::g_pCTkMouse->OnCalc();
	Head = this->MCDList.GetHeadPosition();
	while (Head) {
		CTkMenuCtrlData* DrawNext = this->GetDrawNext(&Head);
		if (DrawNext) {
			if (!DrawNext->field_6E && !DrawNext->field_70) {
				if (DrawNext->MenuPrimitive) {
					if (DrawNext->MenuPrimitive->MenuCtrlData) {
						if (DrawNext == this->CurMCD)
							DrawNext->MenuPrimitive->OnDrawCalc(true);
						else
							DrawNext->MenuPrimitive->OnDrawCalc(false);
					}
				}
			}
		}
	}

	unsigned short v21{};
	CTkNode* Tail = this->MCDList.GetTailPosition();
	while (Tail) {
		CTkMenuCtrlData* DrawPrev = this->GetDrawPrev(&Tail);
		if (DrawPrev) {
			if (DrawPrev->field_38) {
				if ((v21 & DrawPrev->field_38) != 0) {
					if (DrawPrev->field_10 != 14)
						this->CloseDrawMenu(DrawPrev, 1, 1, 0, 1);
				}
				else {
					if (DrawPrev->field_10 != 14)
						v21 |= DrawPrev->field_38;
					else {
						if (!DrawPrev->field_6E && DrawPrev->field_73 == 1 && DrawPrev->field_75) {
							this->OpenDrawMenu(DrawPrev);
							v21 |= DrawPrev->field_38;
						}
						else if (DrawPrev->field_72 == 1)
							v21 |= DrawPrev->field_38;
					}
				}
			}
		}
	}

	//sub //TODO
}

void FFXI::CTk::CTkMenuMng::OnDraw()
{
	this->InitDraw();
	//sub //TODO

	if (this->CurMCD) {
		if (this->CurMCD->ScrollBar) {
			if (!this->IsNoWindow())
				;//sub //TODO
		}
	}
	for (int i = 0; i < Constants::Values::COUNT_MENU_LAYERS; ++i) {
		CTkNode* Head = TkManager::g_CTkMenuMng.MCDList.GetHeadPosition();
		while (Head) {
			CTkMenuCtrlData* Next = TkManager::g_CTkMenuMng.GetDrawNext(&Head);
			if (Next && Next->MaybeDrawLayer == i && (!this->IsHideMode() || (Next->field_34 & 0x40) != 0))
				Next->OnDraw();
		}
	}

	TkManager::g_pCTkMouse->DrawCursor();
	this->EndDraw();
}

void FFXI::CTk::CTkMenuMng::OnLButtonDown()
{
	if (!this->HaveActiveWindow())	return;
	if (!this->CurMCD) return;

	CTkMouse* mouse = TkManager::g_pCTkMouse;
	if (!mouse || mouse->field_4D != 2) return;

	if (!this->CurMCD->IsOnWindow(mouse->field_4)) return;
	short buttonID = this->CurMCD->IsOnButton(mouse->field_4, nullptr);
	if (buttonID <= 0) return;

	if (this->CurMCD->field_4C == buttonID) {
		if (this->field_B0 <= 30) {
			this->field_B0 += CYyDb::g_pCYyDb->CheckTick();
		}
		else
			this->OnKeyDown(29);
	}
	else {
		this->CurMCD->SetCursol(buttonID, 0);
		this->OnKeyDown(9);
	}
}

void FFXI::CTk::CTkMenuMng::OnLButtonUp()
{
	CTkMouse* mouse = TkManager::g_pCTkMouse;
	if (!mouse) return;

	if (this->CurMCD && mouse->field_4D == 2 && !mouse->field_86) {
		short buttonID = this->CurMCD->IsOnButton(mouse->field_4, nullptr);
		if (buttonID <= 0) {
			if (!mouse->field_87)
				SoundMng::CYySePlayBeep();
		}
		else if (buttonID == this->CurMCD->field_4C) {
			this->OnKeyDown(5);
			this->field_B0 = 0;
			return;
		}
	}
	this->field_B0 = 0;
}

void FFXI::CTk::CTkMenuMng::OnRButtonDown()
{
	this->HaveActiveWindow();
}

void FFXI::CTk::CTkMenuMng::OnRButtonUp()
{
	if (!this->CurMCD) return;
	if (!TkManager::g_pCTkMouse) return;

	if (TkManager::g_pCTkMouse->field_4D == 2)
		this->OnKeyDown(6);
}

void FFXI::CTk::CTkMenuMng::OnMButtonUp()
{
	if (!CFsConf6Win::Check() && Input::InputMng::CanIGetKey(4))
		this->OnKeyDown(7);
}

void FFXI::CTk::CTkMenuMng::OnKeyDown(int a1)
{
	if (this->IsNoWindow()) return;

	if (!this->HaveActiveWindow() || this->IsHideMode()) {
		if (a1 == 7)
			;//RequestOpenMainMenu //sub //todo
	}
	else 
		this->CurMCD->OnKeyDown(a1);
}

void FFXI::CTk::CTkMenuMng::OnPhaseCtrl()
{
	int v2 = FFXI::GlobalStruct::g_GlobalStruct.field_0;
	int v12 = 0;
	if (v2 != this->field_64) {
		this->field_AE = 0;
		switch (v2) {
		case 0:
		case 16:
		case 32:
		case 48:
		case 160:
			break;
		case 64:
			v12 = 1;
			this->OnMenuSwitchCtrl(512);
			break;
		case 80:
			this->OnMenuSwitchCtrl(128);
			break;
		case 96:
			this->field_AC = 1;
			v12 = 1;
			break;
		case 112:
		case 224:
			this->OnMenuSwitchCtrl(8);
			//sub
			this->OnMenuSwitchCtrl(0x4000);
			break;
		case 128:
			this->OnMenuSwitchCtrl(8);
			break;
		case 176:
			break;
		case 192:
			//
			break;
		case 208:
			v12 = 1;
			this->OnMenuSwitchCtrl(0x2000);
			break;
		default:
			v12 = 1;
			break;
		}

		if (this->field_64 == 64)
			this->OnMenuSwitchCtrl(1024);
		else if (v2 == 64)
			this->OnMenuSwitchCtrl(512);
	}

	if (this->field_AC && v2 == 96 && Placeholder::pGlobalNowZone) {
		throw "NOT IMPLEMENTED";
		//if dword_1046C200
		//sub //TODO
	}

	this->field_64 = v2;

	//sub //TODO
	if (v12)
		this->SetHideMode(0);

	if (v2 == 96) {
		//sub //todo getkey
	}
	else {
		if (PrimMng::g_pYkWndYesno) {
			throw "NOT IMPLEMENTED";
		}
	}
}

void FFXI::CTk::CTkMenuMng::OnMenuSwitchCtrl(int a2)
{
	PrimTableEntry* entry = PrimMng::PrimTable;
	
	while (entry->MenuName[0]) {
		CTkMenuPrimitive* prim{ nullptr };

		if (entry->PrimMenuPtr) {
			prim = *entry->PrimMenuPtr;
			if (prim) {
				prim->PrimVirt16(a2);
			}
		}

		if (a2 & 0x800) {
			if ((entry->SomeByte1 & 4) == 0 ||
				(entry->SomeInt & 0x400000) != 0 ||
				(entry->SomeInt & 0x2000000) != 0 &&
				true) {//sub //todo //Event Flag check
				if (!prim || prim->MenuCtrlData)
					this->DestroyDrawMenu(entry->MenuName);
			}
		}

		if ((a2 & 0x1000) != 0 &&
			(entry->SomeInt & 0x1000000) != 0 &&
			(!prim || prim->MenuCtrlData))
			this->DestroyDrawMenu(entry->MenuName);

		if ((a2 & 0x200) != 0 &&
			(entry->SomeByte1 & 8) != 0 &&
			(!prim || prim->MenuCtrlData))
			this->DestroyDrawMenu(entry->MenuName);

		if ((a2 & 0x20) != 0) {
			TkManager::g_pCTkInputCtrl->ResetAllFlag();
			if (((entry->SomeByte1 & 4) == 0 ||
				(entry->SomeInt & 0x400000) != 0) &&
				(!prim || prim->MenuCtrlData)) {
				this->DestroyDrawMenu(entry->MenuName);
			}
			if ((entry->SomeInt & 0x400) != 0) {
				if ((entry->SomeInt & 0x4000000) != 0) {
					if (Placeholder::CheckSomething())
						this->CreateDrawMenu(entry->MenuName, true, 1);
				}
				else if ((entry->SomeInt & 0x8000000) == 0 || !Placeholder::CheckSomething()) {
					this->CreateDrawMenu(entry->MenuName, true, 1);
				}
			}
		}

		if ((a2 & 0x40) != 0) {
			if ((entry->SomeInt & 0x800) != 0) {
				if ((entry->SomeInt & 0x4000000) != 0) {
					if (Placeholder::CheckSomething())
						this->DestroyDrawMenu(entry->MenuName);
				}
				else if ((entry->SomeInt & 0x8000000) == 0 || !Placeholder::CheckSomething()) {
					this->DestroyDrawMenu(entry->MenuName);
				}
			}
		}

		if ((a2 & 0x10) != 0 && (entry->SomeInt & 0x200) != 0)
			this->DestroyDrawMenu(entry->MenuName);
		if ((a2 & 8) != 0 && (entry->SomeInt & 0x100) == 0)
			this->DestroyDrawMenu(entry->MenuName);
		if ((a2 & 4) != 0 && *(char*)entry->SomeInt < 0)
			this->CreateDrawMenu(entry->MenuName, true, 0);
		++entry;
	}

	if ((a2 & 0xA28) != 0)
	{
		if (TkManager::g_pCTkInputCtrl)
			TkManager::g_pCTkInputCtrl->ResetCallback();

		if (PrimMng::g_pYkWndPartyList)
			throw "NOT IMPLEMENTED";
	}
	if ((a2 & 0x800) != 0) {
		if (TkManager::g_pCTkInputCtrl && TkManager::g_pCTkInputCtrl->field_68)
			TkManager::g_pCTkInputCtrl->ResetAllFlag();
	}
	if ((a2 & 0x200) != 0)
		this->field_AA = 1;
	if ((a2 & 0x1204) != 0)
		Placeholder::StartAutoOffline();
	if ((a2 & 0xC08) != 0)
		Placeholder::PauseAutoOffline();
}

CTkMenuCtrlData* FFXI::CTk::CTkMenuMng::GetCurMCD()
{
	return this->CurMCD;
}

void FFXI::CTk::CTkMenuMng::SetCurMCD(CTkMenuCtrlData* a2)
{
	if (this->CurMCD == a2) return;

	if (this->CurMCD) {
		this->CurMCD->OnActive(0);
		this->CurMCD->field_98 = 0;
	}

	this->PrevMCD = this->CurMCD;
	this->CurMCD = a2;
	
	if (a2) {
		a2->OnActive(1);
		TkManager::g_pCTkMouse->RequestJumpWindow();
	}
}

void FFXI::CTk::CTkMenuMng::RemoveMCD(CTkMenuCtrlData* a2)
{
	CTkNode* Head = this->MCDList.GetHeadPosition();
	while (Head) {
		CTkNode* v4 = Head;
		if ((CTkMenuCtrlData*)this->MCDList.GetNext(&Head) == a2)
			this->MCDList.RemoveAt(v4);
	}
}

bool FFXI::CTk::CTkMenuMng::IsAreaWindow(unsigned short a1, CTkMenuCtrlData* a2, bool a3)
{
	CTkNode* Head = TkManager::g_CTkMenuMng.MCDList.GetHeadPosition();

	while (true) {
		if (!Head)
			return false;

		CTkMenuCtrlData* DrawNext = TkManager::g_CTkMenuMng.GetDrawNext(&Head);
		if (DrawNext && (a1 & DrawNext->field_38) != 0 &&
			(a3 || DrawNext->field_10 != 14) &&
			(!a2 || DrawNext != a2) )
			break;
	}
	return true;
}

int FFXI::CTk::CTkMenuMng::SomeMapping(int a1)
{
	switch (a1) {
	case 0:
		return 0;
	case 1:
		return 1;
	case 3:
		return 3;
	case 5:
		return 4;
	case 7:
		return 2;
	case 9:
		return 5;
	default:
		return 6;
	}
}

void FFXI::CTk::CTkMenuMng::MarginComp(TKRECT* a1)
{
	if (a1->Left < 16)
		a1->Left = 16;

	if (a1->Top < 16)
		a1->Top = 16;

	short upperX = TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C - 16;
	short upperY = TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E - 16;

	if (a1->Right >= upperX)
		a1->Right = upperX;

	if (a1->Bottom >= upperY)
		a1->Bottom = upperY;
}
