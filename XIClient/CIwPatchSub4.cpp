#include "Globals.h"
#include "CIwPatchSub4.h"
#include "CTkMenuCtrlData.h"
#include "TkRect.h"
#include "TkManager.h"
#include "CTkMenuMng.h"
#include "TextRenderer.h"
#include "StringTables.h"
#include <string>

using namespace FFXI::CTk;

struct LobbyHelpString {
	int Page;
	int Entry;
};

LobbyHelpString LobbyHelpTable[] = {
	{-1, -1},
	{-1, -1},
	{-1, -1},
	{-1, -1},
	{-1, -1},
	{-1, -1},
	{-1, -1},
	{8, 0x5F},
	{8, 0x62},
	{8, 0x63},
	{-1, -1},
	{-1, -1},
	{8, 0x64},
	{8, 0x65},
	{8, 0x66},
	{8, 0x67},
	{8, 0x68},
	{8, 0x69},
	{8, 0x6A},
	{8, 0x6B},
	{8, 0x6D},
	{8, 0x6E},
	{8, 0x6F},
	{27, 4},
	{8, 0x70},
	{8, 0x72},
	{8, 0x73},
	{8, 0x74},
	{8, 0x75},
	{8, 0x76},
	{8, 0x77},
	{8, 0x78},
	{8, 0x79},
	{8, 0x71},
	{8, 0xF4},
	{8, 0xF5},
	{8, 0xF6},
	{8, 0xF7},
	{8, 0xF8},
	{8, 0xF9},
	{8, 0xFA},
	{8, 0xFB},
	{8, 0xFC},
	{8, 0xFD},
	{8, 0xFE},
	{8, 0xFF},
	{8, 0x100},
	{8, 0x101},
	{8, 0x102},
	{8, 0x103},
	{8, 0x104},
	{8, 0x105},
	{8, 0x106},
	{8, 0x107},
	{8, 0x108},
	{8, 0x109},
	{8, 0x10A},
	{8, 0x10B},
	{8, 0x10C},
	{8, 0x10D},
	{-1, -1},
};

void FFXI::CTk::CIwPatchSub4::Init()
{
	this->field_14 = 0;
	memset(this->field_30, 0, sizeof(this->field_30));
	this->field_18 = 0;
	this->field_1C = 0;
	this->field_20 = 0;
	this->field_24 = 0;
	this->field_2C = 0;
	this->field_130 = 0;
	this->field_134 = 30;
}

void FFXI::CTk::CIwPatchSub4::OnInitialUpdatePrimitive()
{
	this->MenuCtrlData->field_85 = 0;
	this->MenuCtrlData->field_86 = 0;
	this->MenuCtrlData->field_84 = 0;

	TKRECT v1{};
	v1.Left = 0;
	v1.Top = 0;
	v1.Right = 0;
	v1.Bottom = 0;
	this->MenuCtrlData->GetWindowLocate(&v1);
	this->MenuCtrlData->ResizeWindow(
		v1.Left,
		v1.Top,
		TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C,
		v1.Bottom - v1.Top,
		1,
		0,
		0
	);
}

void FFXI::CTk::CIwPatchSub4::OnDrawPrimitive()
{
	if (this->field_14 != -1) {
		TKRECT v11{};
		this->MenuCtrlData->GetWindowLocate(&v11);

		if (this->field_2C != this->field_130) {
			this->field_134 = 30;
			this->field_130 = this->field_2C;
			this->field_30[this->field_2C] = 0;
		}

		if (this->field_14 == 1)
			throw "NOT IMPLEMENTED";//sub

		const char* text = &Globals::NullString;
		LobbyHelpString* entry = &LobbyHelpTable[this->field_2C];
		if (entry->Page >= 0)
			text = Text::XiStrGet(entry->Page, entry->Entry);

		int Str = Text::TextRenderer::SjisFontWidthGetStr(text);

		short Diff = TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C;
		if (Str < (Diff - 16)) {
			Text::TextRenderer::mojishadow(
				((Diff) - Str) / 2,
				v11.Top + 160,
				text,
				0,
				0x80808080);
		}
		else {
			char v13[0x400];
			sprintf_s(v13, 0x400, "\x81@\x81@\x81@\x81@%s", text);
			int v6 = Text::TextRenderer::SjisFontWidthGetStr(v13);
			if (this->field_134)
				this->field_134 -= 1;
			else {
				this->field_30[this->field_2C] += 2;
				if (this->field_30[this->field_2C] >= v6)
					this->field_30[this->field_2C] = 0;
			}

			int v9 = this->field_30[this->field_2C];
			if (v9 < v6)
				Text::TextRenderer::mojishadow(-v9, v11.Top + 160, v13, 0, 0x80808080);
			int v10 = v6 - v9;
			if (v10 < Diff)
				Text::TextRenderer::mojishadow(v10, v11.Top + 160, v13, 0, 0x80808080);
		}
	}
}
