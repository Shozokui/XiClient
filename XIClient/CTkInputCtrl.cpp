#include "CTkInputCtrl.h"
#include "TkManager.h"
#include "CTkMenuMng.h"

using namespace FFXI::CTk;

FFXI::CTk::CTkInputCtrl::CTkInputCtrl()
{
	Init();
	this->field_84 = 0;
	this->field_88 = 0;
}

void FFXI::CTk::CTkInputCtrl::Init()
{
	this->field_68 = 0;
	this->field_74 = 0;

	this->field_6C = 0;
	this->field_70 = 0;

	this->field_75 = 0;
	this->field_76 = 0;
	this->field_77 = 0;
	this->field_7C = 0;
	this->field_80 = 0;
	this->field_6A = 0;
	this->field_51 = 0;
	this->field_50 = 0;
	this->field_5C &= 0xFFFFFFF0;
	this->field_52 = 0;
	this->field_53 = 0;
	this->field_54 = 0;
	this->field_60 = -1;
	this->field_64 = 0;
	this->field_57 = 0;
	this->field_58 = 0;
	this->field_59 = 0;
	this->field_55 = 0;
	this->field_56 = 0;
	this->field_8C = 0;
	this->field_90 = 0;

	this->field_0.ClearTarget();
	this->field_28.ClearTarget();

	this->field_C4 = 0;
	this->CallbackReset = 0;
	this->field_CC = 0;
	this->field_D0 = 0;
	this->field_9C = 0;
	this->field_B8 = 0;
	this->field_BC = 0;
	this->field_C0 = 0;
}

void FFXI::CTk::CTkInputCtrl::ResetAllFlag()
{
	this->field_68 = 0;
	TkManager::g_CTkMenuMng.SetCurMCD(nullptr);
}

void FFXI::CTk::CTkInputCtrl::ResetCallback()
{
	if (this->CallbackReset)
		this->CallbackReset(0, 0);

	this->CallbackReset = 0;
	this->field_C4 = 0;
	this->field_D0 = 0;

	//sub //TODO
}

void FFXI::CTk::CTkInputCtrl::SetSomeCoords(int x, int y)
{
	double XRes = ((double)TkManager::g_CTkMenuMng.UIXRes) * 0.5;
	double YRes = ((double)TkManager::g_CTkMenuMng.UIYRes) * 0.5;

	this->field_8C = ((double)x - XRes) / XRes;
	this->field_90 = ((double)y - YRes) / YRes;
	this->field_55 = 1;
}