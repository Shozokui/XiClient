#include "CTkDrawCtrlFrame.h"
#include "CTkMenuFrame.h"
#include "CTkResourceID.h"
#include "KaWindow.h"
#include "TkManager.h"
#include "Globals.h"
#include "CYyDb.h"

using namespace FFXI::CTk;

int CTkDrawCtrlFrame::AnimOnValue{ 25 };

FFXI::CTk::CTkDrawCtrlFrame::CTkDrawCtrlFrame()
{
	this->field_1C = 0;
	this->field_1A = 0;
	this->field_8 = 0;
	this->field_44 = 0;
	this->field_48 = 0;
	this->Window = nullptr;
	this->field_18 = 0;
	this->field_22 = 0;
	this->field_24 = 0;
	this->field_26 = 0;
	this->field_28 = 0;
	this->field_2C = 0;
	this->MenuFrame = nullptr;
	this->field_34 = 0;
	this->field_38 = 0;
	this->field_3C = nullptr;
	this->field_40 = 0;
	this->field_4C = 0;
	this->field_C = 1.0;
	this->field_10 = 1.0;
	this->field_14 = 1.0;
}

FFXI::CTk::CTkDrawCtrlFrame::~CTkDrawCtrlFrame()
{
	if (this->Window != nullptr) {
		delete this->Window;
		this->Window = nullptr;
	}
}

void FFXI::CTk::CTkDrawCtrlFrame::SetResourceData(CTkMenuFrame* a2)
{
	unsigned short* shortData = (unsigned short*)a2->Buffer;
	this->MenuFrame = a2;
	this->field_22 = shortData[5];
	this->field_24 = shortData[6];
}

int FFXI::CTk::CTkDrawCtrlFrame::LocateCalc(short a2, short a3, char a4)
{
	if (!this->MenuFrame)
		return 0;

	float XCoeff = 1.0, YCoeff = 1.0;
	
	if (a4)
		TkManager::g_CTkMenuMng.GetWinCoefficient(&XCoeff, &YCoeff);

	short* shortData = (short*)this->MenuFrame->Buffer;
	unsigned char* byteData = (unsigned char*)this->MenuFrame->Buffer;

	this->field_1A = a2 + ((double)shortData[1] * XCoeff);
	this->field_1C = a3 + ((double)shortData[2] * YCoeff);

	this->field_1E = (double)shortData[1] * XCoeff;
	this->field_20 = (double)shortData[2] * YCoeff;

	switch (byteData[19]) {
	case 1:
		this->field_1A += TkManager::g_CTkMenuMng.UIXRes - 512;
		this->field_1E += TkManager::g_CTkMenuMng.UIXRes - 512;
		break;
	case 2:
		this->field_1C += TkManager::g_CTkMenuMng.UIYRes - 448;
		this->field_20 += TkManager::g_CTkMenuMng.UIYRes - 448;
		break;
	case 3:
		this->field_1A += TkManager::g_CTkMenuMng.UIXRes - 512;
		this->field_1C += TkManager::g_CTkMenuMng.UIYRes - 448;
		this->field_1E += TkManager::g_CTkMenuMng.UIXRes - 512;
		this->field_20 += TkManager::g_CTkMenuMng.UIYRes - 448;
		break;
	}

	this->field_28 = 0;
	if (this->MenuFrame->field_8)
		this->field_28 = this->MenuFrame->field_8;

	this->field_2C = 0;

	if (this->MenuFrame->field_C)
		this->field_2C = this->MenuFrame->field_C;
	return 0;
}

void FFXI::CTk::CTkDrawCtrlFrame::OffAnimFrame()
{
	this->field_26 = 0;
}

void FFXI::CTk::CTkDrawCtrlFrame::OnAnimFrame()
{
	this->field_26 = CTkDrawCtrlFrame::AnimOnValue;
}

void FFXI::CTk::CTkDrawCtrlFrame::ShapeCalc(char a2)
{
	if (!this->MenuFrame) return;

	short* shortData = (short*)this->MenuFrame->Buffer;
	float x{}, y{};

	if (this->Window) {
		if (a2) {
			TkManager::g_CTkMenuMng.GetWinCoefficient(&x, &y);
			this->field_22 = (double)shortData[5] * x;
			this->field_24 = (double)shortData[6] * y;
		}
		this->Window->field_0 = this->field_22;
		this->Window->field_4 = this->field_24;
	}
	else {
		this->field_22 = shortData[5];
		this->field_24 = shortData[6];
		if (a2) {
			TkManager::g_CTkMenuMng.GetWinCoefficient(&x, &y);
			this->field_22 = (double)shortData[5] * x;
			this->field_24 = (double)shortData[6] * y;
		}
		this->Window = new KaWindow();
		if (!this->Window)
			return;

		this->Window->Create(this->field_22, this->field_24);
	}

	if (this->field_26 <= 0) {
		this->field_10 = 1.0;
		this->field_14 = 1.0;
		if (a2) {
			TkManager::g_CTkMenuMng.GetWinCoefficient(&x, &y);
			this->field_10 *= x;
			this->field_14 *= y;
		}
		if (this->field_48 > 0) {
			this->field_48 -= CYyDb::g_pCYyDb->CheckTick();
			if (this->field_48 <= 0) {
				this->field_48 = 0;
				if (this->field_44 == 2) {
					this->field_44 = 1;
				}
				else if (this->field_44 == 3) {
					this->field_44 = 0;
				}
			}
		}
		if (this->field_44 == 1) {
			this->field_C = 0.0f;
		}
		else if (this->field_44 == 2) {
			this->field_C = (double)this->field_48 * 0.05f;
		}
		else if (this->field_44 == 3) {
			this->field_C = 1.0 - (double)this->field_48 / 0.05f;
			if (this->field_C > 1.0)
				this->field_C = 0.9f;
		}
		else {
			if (this->field_4C)
				this->field_C = 0.58f;
			else
				this->field_C = 1.0f;
		}
	}
	else {
		this->field_26 -= CYyDb::g_pCYyDb->CheckTick();
		if (this->field_26 < 0)
			this->field_26 = 0;

		int v29 = CTkDrawCtrlFrame::AnimOnValue - this->field_26;
		double v8 = (double)v29 / (double)CTkDrawCtrlFrame::AnimOnValue;
		this->field_C = v8;
		this->field_14 = v8;
		this->field_10 = v8;

		if (this->field_C > 1.0) {
			this->field_C = 1.0;
			this->field_14 = 1.0;
			this->field_10 = 1.0;
		}
	}

	if (this->field_38 != this->field_34 || !this->field_8) {
		this->field_38 = this->field_34;
		int someCount = this->MenuFrame->Buffer[20];
		int v22 = 0;
		CTkResourceID* resID{ nullptr };
		while (v22 < someCount) {
			resID = this->MenuFrame->ResourceIDs + v22;
			if (resID) {
				if (resID->field_0 == 0 ||
					resID->field_0 == CTkMenuMng::SomeMapping(this->field_34))
				{
					if (!resID->field_18)
						resID->field_18 = TkManager::g_CTkMenuDataList.GetSubLists(resID->field_4);
					if (!resID->field_18)
						return;
					this->field_8 = (*resID->field_18)[resID->field_2];
				}
			}
			++v22;
		}
	}
}
