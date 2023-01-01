#include "CTkMenuCtrlData.h"
#include "CTkDrawCtrlFrame.h"
#include "CTkDrawCtrlButton.h"
#include "CTkDrawCtrlCursol.h"
#include "CTkMenuPrimitive.h"
#include "CTkMenuCursol.h"
#include "CTkMenuButton.h"
#include "CTkMenuData.h"
#include "CTkMenuFrame.h"
#include "TkManager.h"
#include "KaWindow.h"
#include "PrimMng.h"
#include "CTkEventMsg.h"
#include "Enums.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CDx.h"
#include "_49SubList.h"
#include "CTkMenuMng.h"
#include "CTkInputCtrl.h"
#include "CTkMouse.h"
#include "SoundMng.h"
using namespace FFXI::CTk;

D3DVIEWPORT8 CTkMenuCtrlData::MenuViewport{};

FFXI::CTk::CTkMenuCtrlData::CTkMenuCtrlData()
{
	this->field_3A.Left = 0;
	this->field_3A.Top = 0;
	this->field_3A.Right = 0;
	this->field_3A.Bottom = 0;
	this->field_42.Left = 0;
	this->field_42.Top = 0;
	this->field_42.Right = 0;
	this->field_42.Bottom = 0;
	this->field_9C = 0;
	this->field_9E = 0;
	this->field_A0 = 0;
	this->field_A2 = 0;
	this->field_BC = 0;
	this->field_BE = 0;
	this->field_C0 = 0;
	this->field_C2 = 0;
	this->field_C4 = 0;
	this->field_C8 = this;
	this->MenuData = nullptr;
	this->field_4C = -1;
	this->field_4A = -1;
	this->field_4E = 0;
	this->field_50 = 0;
	this->field_54 = 0;
	this->field_52 = 0;
	this->field_8 = 0;
	this->field_56 = 0;
	this->field_10 = 0;
	this->field_30 = -1;
	this->field_34 = 0xFFFF;
	this->field_38 = 0;
	this->DrawCtrlButtonCount = 0;
	this->field_5C = 1;
	this->field_5E = 0;
	this->CallbackClose = nullptr;
	this->field_80 = 0;
	this->field_5D = 0;
	this->MaybeDrawLayer = 0;
	this->field_61 = 1;
	this->field_62 = 1;
	this->field_68 = 1;
	this->field_69 = 1;
	this->field_6A = 1;
	this->field_63 = 1;
	this->field_84 = 1;
	this->field_85 = 1;
	this->field_86 = 1;
	this->field_6B = 0;
	this->field_6D = 0;
	this->field_6C = 0;
	this->field_6E = 0;
	this->field_6F = 0;
	this->ScrollBar = nullptr;
	this->MenuPrimitive = nullptr;
	this->field_70 = 0;
	this->field_71 = 0;
	this->field_73 = 1;
	this->field_72 = 0;
	this->field_94 = -999;
	this->field_75 = 0;
	this->field_76 = 0;
	this->field_9B = 0;
	this->field_A4 = 0;
	this->field_A8 = 0;
	this->field_AC = 0;
	this->field_B0 = 0;
	this->field_B4 = 0;
	this->field_B8 = 0;
	this->field_98 = 0;
	this->field_74 = 1;
	this->field_77 = 1;
	this->field_78 = 1;
	this->field_99 = 0;
	this->field_9A = 0;
	this->field_BC = 0;
	this->field_BE = 0;
	this->field_C0 = 0;
	this->field_C2 = 0;
	this->field_CC = 0x80808080;
}

FFXI::CTk::CTkMenuCtrlData::~CTkMenuCtrlData()
{
	CTkNode* head = this->DrawCtrlList.GetHeadPosition();
	while (head != nullptr) {
		CTkNode* headbackup = head;
		CTkObject* next = this->DrawCtrlList.GetNext(&head);
		this->DrawCtrlList.RemoveAt(headbackup);
		if (next->DrawType == FFXI::Constants::Enums::TkDrawType::TKDraw_Frame
			|| next->DrawType == FFXI::Constants::Enums::TkDrawType::TKDraw_Button
			|| next->DrawType == FFXI::Constants::Enums::TkDrawType::TKDraw_Cursol) {
			delete next;
		}
	}
}

void FFXI::CTk::CTkMenuCtrlData::SetResourceData(CTkMenuData* a1)
{
	this->MenuData = a1;
	CTkMenuFrame* Frame = a1->GetFrame();
	if (Frame) {
		CTkDrawCtrlFrame* newDCF = new CTkDrawCtrlFrame();
		newDCF->SetResourceData(Frame);
		int Calc = newDCF->LocateCalc(0, 0, this->field_99);

		this->field_52 = newDCF->field_1A;
		this->field_54 = newDCF->field_1C;
		newDCF->ShapeCalc(Calc);

		this->field_3A.Bottom = newDCF->field_1C + newDCF->field_24;
		this->field_3A.Right = newDCF->field_1A + newDCF->field_22;
		this->field_3A.Top = newDCF->field_1C;
		this->field_3A.Left = newDCF->field_1A;

		this->field_42.Left = this->field_3A.Left;
		this->field_42.Top = this->field_3A.Top;

		this->field_42.Right = this->field_3A.Right;
		this->field_42.Bottom = this->field_3A.Bottom;

		this->DrawCtrlList.AddTail(newDCF);
	}

	for (int i = 0; i < this->MenuData->ButtonCount; ++i) {
		CTkMenuButton* Button = this->MenuData->GetButton(i + 1);
		if (!Button)
			break;
		CTkDrawCtrlButton* newDCB = new CTkDrawCtrlButton(this);
		if (!newDCB)
			return;

		++this->DrawCtrlButtonCount;

		newDCB->SetResourceData(Button);
		newDCB->LocateCalc(this->field_52, this->field_54, this->field_99);
		newDCB->ShapeCalc(this->field_99);
		this->DrawCtrlList.AddTail(newDCB);
		if (this->field_4C == newDCB->field_3A)
			this->field_8 = newDCB;
	}

	CTkMenuCursol* Cursol = this->MenuData->GetCursol();
	if (Cursol) {

		CTkDrawCtrlCursol* newDCC = new CTkDrawCtrlCursol();
		if (!newDCC)
			return;

		newDCC->SetResourceData(Cursol);
		newDCC->LocateCalc(this->field_8);
		newDCC->ShapeCalc();
		this->DrawCtrlList.AddTail(newDCC);
	}

	this->SetCursolNoSelect(1);
}

void FFXI::CTk::CTkMenuCtrlData::RemoveResourceData()
{
	CTkNode* HeadPosition = this->DrawCtrlList.GetHeadPosition();
	CTkNode* a2 = HeadPosition;
	if (HeadPosition)
	{
		do
		{
			CTkNode* v4 = HeadPosition;
			CTkObject* Next = this->DrawCtrlList.GetNext(&a2);
			if (!Next)
				break;
			this->DrawCtrlList.RemoveAt(v4);
			if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Frame) {
				CTkDrawCtrlFrame* frame = (CTkDrawCtrlFrame*)Next;
				delete frame;
			}
			else if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Button) {
				CTkDrawCtrlButton* button = (CTkDrawCtrlButton*)Next;
				delete Next;
			}
			else if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Cursol) {
				CTkDrawCtrlCursol* cursol = (CTkDrawCtrlCursol*)Next;
				delete cursol;
			}

			HeadPosition = (CTkNode*)a2;
		} while (a2);
	}
	this->DrawCtrlList.RemoveAll();
	this->MenuData = 0;
	
}

void FFXI::CTk::CTkMenuCtrlData::SaveCursolInfo()
{
	if (this->MenuData) {
		CTkDrawCtrlButton* button = this->GetDrawCtrlButton(this->field_4C);

		if (button) {
			if (button->field_10 != 16 && button->field_18 != 15)
				this->MenuData->SetLastCursol(this->field_4C, this->field_4E);
		}
	}
}

void FFXI::CTk::CTkMenuCtrlData::LoadCursolInfo()
{
	int v2 = 1;
	if ((this->field_34 & 0x10000) == 0) {
		if (this->MenuData) {
			short v5{};
			this->MenuData->GetLastCursol(&v5, &this->field_4E);
			if (v5 < 1)
				v2 = 1;
			else
				v2 = v5;
		}
	}

	this->SetCursolNoSelect(v2);
}

void FFXI::CTk::CTkMenuCtrlData::SetCursolNoSelect(int a2)
{
	int v3{};
	if (a2 <= 0)
		v3 = 1;
	else
		v3 = a2;

	if (v3 > this->DrawCtrlButtonCount)
		v3 = this->DrawCtrlButtonCount;

	if (this->field_4C != v3) {
		CTkDrawCtrlButton* DrawCtrlButton = this->GetDrawCtrlButton(this->field_4C);
		if (DrawCtrlButton) {
			DrawCtrlButton->field_24 = 0;
			if (DrawCtrlButton->field_10 != 5)
				DrawCtrlButton->SetMenuCtrlID(0);
		}

		DrawCtrlButton = this->GetDrawCtrlButton(v3);
		if (DrawCtrlButton) {
			DrawCtrlButton->field_24 = 1;
			if (DrawCtrlButton->field_10 != 5)
				DrawCtrlButton->SetMenuCtrlID(3);
			this->field_4C = v3;
			this->RepositionCursol(v3);
		}
	}
	
	this->SetButtonHelp();
}

void FFXI::CTk::CTkMenuCtrlData::SetCursol(int a2, int a3)
{
	int v4 = a2;
	if ((short)a2 <= 0)
		v4 = 1;

	if (v4 > this->DrawCtrlButtonCount)
		v4 = this->DrawCtrlButtonCount;

	if (this->field_4C != v4) {
		CTkDrawCtrlButton* button = this->GetDrawCtrlButton(this->field_4C);
		if (button) {
			button->field_24 = 0;
			if (button->field_10 != 5)
				button->SetMenuCtrlID(0);
		}
		button = this->GetDrawCtrlButton(v4);
		if (button) {
			button->field_24 = 1;
			if (button->field_10 != 5)
				button->SetMenuCtrlID(3);
			this->field_4C = v4;
			this->RepositionCursol(v4);
			if (TkManager::g_pCTkMouse->field_48 <= 0.0 && TkManager::g_pCTkMouse->field_4E)
				this->OnKeyDown(9);
		}
		if (a3) {
			tagPOINT pt{};
			this->GetTargetButtonPoint(&pt);
			TkManager::g_pCTkMouse->SetMousePoint(&pt, 18.0);
		}
	}
	this->SetButtonHelp();
}

void FFXI::CTk::CTkMenuCtrlData::RepositionCursol(int a2)
{
	CTkDrawCtrlButton* DrawCtrlButton = this->GetDrawCtrlButton(a2);
	if (DrawCtrlButton) {
		CTkNode* Head = this->DrawCtrlList.GetHeadPosition();
		while (Head) {
			CTkDrawCtrlCursol* Next = (CTkDrawCtrlCursol*)this->DrawCtrlList.GetNext(&Head);
			if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Cursol) {
				Next->LocateCalc(DrawCtrlButton);
				this->field_8 = DrawCtrlButton;
			}
		}
	}
}

void FFXI::CTk::CTkMenuCtrlData::SetButtonHelp()
{
	if (TkManager::g_CTkMenuMng.CurMCD != this)
		return;

	CTkDrawCtrlButton* DrawCtrlButton = this->GetDrawCtrlButton(this->field_4C);
	if (!DrawCtrlButton || !DrawCtrlButton->field_4C)
		return;

	char* text = { nullptr };
	
	if (!DrawCtrlButton->field_4C) {
		if (!DrawCtrlButton->field_40) {
			text = this->GetSomeHelpText();
		}
		else
			text = DrawCtrlButton->field_40;
	}
	else
		text = DrawCtrlButton->field_4C;

	if (text && strlen(text)) {
		if (PrimMng::g_pTkHelp) {
			throw "NOT IMPLEMENTED";
			//sub //TODO
			//sethelpstring
			this->field_6D = 1;
		}
	}

	if (DrawCtrlButton->field_44 && strlen(DrawCtrlButton->field_44)) {
		if (DrawCtrlButton->field_50)
			text = DrawCtrlButton->field_50;
		else
			text = DrawCtrlButton->field_44;
	}
	else
		text = this->GetSomeTitleText();

	if (PrimMng::g_pTkTitle) {
		throw "NOT IMPLEMENTED";
		//sub //TODO
		this->field_6C = 1;
	}
}

void FFXI::CTk::CTkMenuCtrlData::AddPrimitive()
{
this->MenuPrimitive->MenuCtrlData = this;
}

void FFXI::CTk::CTkMenuCtrlData::RemovePrimitive()
{
	if (this->MenuPrimitive) {
		this->MenuPrimitive->OnRemovePrimitive();
		this->MenuPrimitive->MenuCtrlData = nullptr;
		this->MenuPrimitive = nullptr;
	}
}

void FFXI::CTk::CTkMenuCtrlData::DestroyPrimitive()
{
	if (this->MenuPrimitive)
		this->MenuPrimitive->OnDestroyPrimitive();
}

void FFXI::CTk::CTkMenuCtrlData::CloseCallback()
{
	if (this->CallbackClose) {
		this->CallbackClose(this->field_4C, this->field_80);
	}
}

void FFXI::CTk::CTkMenuCtrlData::InitialSetMessage()
{
	if (TkManager::g_CTkMenuMng.CurMCD != this)
		return;

	CTkNode* Head = this->DrawCtrlList.GetHeadPosition();
	CTkDrawCtrlFrame* Next{ nullptr };

	while (true) {
		if (!Head)
			return;

		Next = (CTkDrawCtrlFrame*)this->DrawCtrlList.GetNext(&Head);
		if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Frame)
			break;
	}

	const char* Text = Next->field_3C;
	if (!Text)
		Text = Next->field_28;

	if (Text && strlen(Text)) {
		if (PrimMng::g_pTkHelp) {
			throw "NOT IMPLEMENTED";
			//set help string ptk help
			//sub //TODO
			this->field_6D = 1;
		}
	}

	if (Next->field_2C && strlen(Next->field_2C)) {
		if (PrimMng::g_pTkTitle) {
			throw "NOT IMPLEMENTED";
			this->field_6C = 1;
		}
	}

}

void FFXI::CTk::CTkMenuCtrlData::DestroySetMessage()
{
	//sub //TODO help / title
	if (this->field_6D) {
		this->field_6D = 0;
	}
	if (this->field_6C) {
		this->field_6C = 0;
	}
}

void FFXI::CTk::CTkMenuCtrlData::OnDraw()
{

	CTkDrawCtrlCursol* cursol{ nullptr };
	this->DoSomething();

	int v45 = 0;

	if (this->field_56 > 0)
		this->field_56 -= 1;

	if (!PrimMng::g_pTkEventMsg->field_48 ||
		PrimMng::g_pTkEventMsg->MenuCtrlData == this ||
		(this->field_34 & 0x40) != 0) {
		int NeedEndSizWindow = 0;

		if (this->field_9B) {
			this->startSizWindow();
			NeedEndSizWindow = 1;
		}

		CTkNode* Head = this->DrawCtrlList.GetHeadPosition();
		while (Head) {
			CTkObject* Next = this->DrawCtrlList.GetNext(&Head);
			if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Button) {
				CTkDrawCtrlButton* button = (CTkDrawCtrlButton*)Next;

				if (button->field_3C) {
					if (button->field_48) {
						if (button->field_3A != this->field_4C ||
							!this->field_63) {
							if (button->field_4)
								button->field_4->ExtraDraw(button->ButtonLeft, button->ButtonTop + this->field_B8, button->field_1C, button->field_20, 0x80808080, 0, 0, 0);
						}
						else {
							this->field_8 = button;

							if (button->field_4)
								button->field_4->ExtraDraw(button->ButtonLeft, button->ButtonTop + this->field_B8 + 1, button->field_1C, button->field_20, 0x80805020, 0, 0, 0);
						}
					}
					else {
						if (button->field_10 == 5) {
							if (button->field_3A == this->field_4C && this->field_63) {
								if (button->field_4)
									button->field_4->ExtraDraw(button->ButtonLeft + 1, button->ButtonTop + this->field_B8 + 1, button->field_1C, button->field_20, 0x40402810, 0, 0, 0);
							}
							else {
								if (button->field_4)
									button->field_4->ExtraDraw(button->ButtonLeft, button->ButtonTop + this->field_B8, button->field_1C, button->field_20, 0x30808080, 0, 0, 0);
							}
						}
						else {
							if (button->field_10 == 16) {
								if (button->field_4)
									button->field_4->ExtraDraw(button->ButtonLeft, button->ButtonTop + this->field_B8, button->field_1C, button->field_20, 0x80808080, 0, 0, 0);
							}
							else {
								if (button->field_3A != this->field_4C ||
									!this->field_63 ||
									(this->field_34 & 0x80000) != 0 &&
									!TkManager::g_CTkMenuMng.IsCurrentWindow(this)) {
									if (button->field_4)
										button->field_4->ExtraDraw(button->ButtonLeft, button->ButtonTop + this->field_B8, button->field_1C, button->field_20, 0x80808080, 0, 0, 0);
								}
								else {
									if (button->field_18 == 15)
										v45 = 1;
									if (this->field_5E <= 0) {
										if (button->field_4)
											button->field_4->ExtraDraw(button->ButtonLeft - 1, button->ButtonTop + this->field_B8 - 1, button->field_1C, button->field_20, 0x80805020, 0, 0, 0);
									}
									else {
										//"Animates" the button when it is selected
										this->Blur4Draw(button->field_4, button->ButtonLeft + 1, button->ButtonTop + this->field_B8 + 1, 20, button->field_18);
									}
								}
							}
						}
					}
				}
			}
			else if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Cursol) {
				if (this->field_78) {
					cursol = (CTkDrawCtrlCursol*)Next;
				}
				else {
					throw "WHAT";
				}
			}
			else if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Frame) {
				CTkDrawCtrlFrame* frame = (CTkDrawCtrlFrame*)Next;
				short* shortData = (short*)frame->MenuFrame->Buffer;

				if (this->field_69 && frame->field_44 != 1) {
					
					frame->Window->field_8 = TkManager::g_CTkMenuMng.IsActiveBlink(this);
					if (frame->field_26 > 0) {
						if ((this->field_34 & 0x20) != 0)
							CTkMenuCtrlData::BlurFrame(
								frame->field_8,
								this->field_3A.Left,
								this->field_3A.Top + this->field_B8,
								CTkDrawCtrlFrame::AnimOnValue,
								frame->field_26);
						else
							CTkMenuCtrlData::BlurFrame(
								frame->Window,
								frame->field_1A + shortData[7],
								frame->field_1C + this->field_B8 + shortData[8],
								CTkDrawCtrlFrame::AnimOnValue,
								frame->field_26);
						if (this->field_9B && NeedEndSizWindow)
							this->endSizWindow();
						return;
					}
					if (this->field_10 == 14 && this->field_66 <= 0) {
						if ((this->field_34 & 0x20) != 0)
							CTkMenuCtrlData::BlurFrame(
								frame->field_8,
								this->field_3A.Left,
								this->field_3A.Top + this->field_B8,
								CTkDrawCtrlFrame::AnimOnValue,
								CTkDrawCtrlFrame::AnimOnValue - this->field_64);
						else
							CTkMenuCtrlData::BlurFrame(
								frame->Window,
								frame->field_1A + shortData[7],
								frame->field_1C + this->field_B8 + shortData[8],
								CTkDrawCtrlFrame::AnimOnValue,
								CTkDrawCtrlFrame::AnimOnValue - this->field_64);
						if (this->field_9B && NeedEndSizWindow)
							this->endSizWindow();
						return;
					}
					
					double colord = frame->field_C * 128.0;
					int colori = colord;
					int color = (colori << 24) | 0x808080;

					if ((this->field_34 & 0x20) == 0) {
						int v29 = 0;
						if ((this->field_34 & 0x800000) != 0) {
							v29 = 8;
						}
						else {
							if (frame->field_C < 1.0)
								v29 = 68;
						}

						double v52 = (double)shortData[7] * frame->field_10;
						double v53 = (double)shortData[8] * frame->field_14;

						frame->Window->Draw(
							frame->field_1A + (short)v52,
							frame->field_1C + this->field_B8 + (short)v53,
							frame->field_10,
							frame->field_14,
							color,
							v29,
							this->field_D0);

						if (frame->field_8)
							frame->field_8->ExtraDraw(frame->field_1E, frame->field_20, frame->field_10, frame->field_14, color, 0, 0, 1);
					}
					else if (frame->field_8)
						frame->field_8->ExtraDraw(frame->field_1A, frame->field_1C + this->field_B8, frame->field_10, frame->field_14, color, 0, 0, 0);
				}
			}
			else {
				throw "NOT IMPLEMENTED";
			}	
		}

		if (this->MenuPrimitive) {
			if (this->field_6A && this->field_10 != 14)
				this->MenuPrimitive->OnDrawPrimitive();
		}

		if (this->field_9B && NeedEndSizWindow) {
			this->endSizWindow();
			NeedEndSizWindow = 0;
		}
		if (this->field_10 != 14) {
			if (cursol && this->field_62 && !v45) {
				_49SubList* v37{ nullptr };
				if (TkManager::g_CTkMenuMng.IsCurrentWindow(this))
					v37 = cursol->field_C;
				else
					v37 = cursol->field_10;

				int v38 = cursol->field_20 + cursol->field_28;
				int v39 = cursol->field_22 + cursol->field_2A + this->field_B8;
				
				if (this->field_99) {
					float x{}, y{};
					TkManager::g_CTkMenuMng.GetWinCoefficient(&x, &y);
					if (v37) {
						int v40 = v37->field_26 - v37->field_24;
						v37->ExtraDraw(v38, ((double)v40 * (y - 1.0) * 0.5 + (double)v39), x, y, this->field_CC, 1, 0, 0); 
					}
				}
				else if (this->field_9A && v37) {
					double v42 = this->field_3A.Right - v38;
					float v53 = v37->field_22 - v37->field_20;
					float a4 = v42 / v53;
					v37->ExtraDraw(v38, v39, a4, 1.0, this->field_CC, 0, 0, 0);
				}
				else {
					if (v37)
						v37->ExtraDraw(v38, v39, 1.0, 1.0, this->field_CC, 1, 0, 0);
				}
			}
			//sub //TODO
			if (this->field_9B && NeedEndSizWindow)
				this->endSizWindow();
		}
	}
}

void FFXI::CTk::CTkMenuCtrlData::OnActive(bool a2)
{
	CTkNode* Head = this->DrawCtrlList.GetHeadPosition();
	CTkDrawCtrlFrame* frame{ nullptr };
	while (Head) {
		CTkObject* Next = this->DrawCtrlList.GetNext(&Head);
		if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Frame) {
			frame = (CTkDrawCtrlFrame*)Next;
			break;
		}
	}

	if (a2) {
		if (frame)
			;
		this->InitialSetMessage();
		TkManager::g_pCTkInputCtrl->field_74 |= 1u;
		this->SetButtonHelp();
	}
	else {
		TkManager::g_pCTkInputCtrl->field_74 &= ~1u;

	}

	if (this->MenuPrimitive)
		this->MenuPrimitive->OnActive(a2);
}

void FFXI::CTk::CTkMenuCtrlData::OnKeyDown(int a2) 
{
	if (this->field_10 == 14) return;
	if (this->field_56 > 0) return;

	CTkDrawCtrlButton* button = this->GetDrawCtrlButton(this->field_4C);
	if (button && button->field_10 == 16) return;

	if (this->field_5C) {
		if (this->field_5D && a2 != 6) return;

		switch (a2) {
		case 1:
		case 2:
		case 3:
		case 4:
			if (this->field_61) {
				if (this->DrawCtrlButtonCount > 0) {
					short v5 = this->MenuData->GetCommand(this->field_4C, a2);
					if (v5 >= 0) {
						CTkDrawCtrlButton* v7 = this->GetDrawCtrlButton(v5);
						if (!v7 || v7->field_10 != 16) {
							if (v5 == this->field_4C)
								this->field_5A = a2;
							else
								this->field_5A = 0;
							this->SetCursolNoSelect(v5);
							if (this->field_85 && !this->field_5A)
								SoundMng::CYySePlayCursor();
							this->field_4C = v5;
						}
					}
				}
			}
			break;
		case 5:
			if (this->field_86) {
				if (button && button->field_10 == 5)
					SoundMng::CYySePlayBeep();
				else
					SoundMng::CYySePlayClick();
			}
			if (!button) break;
			if (button->field_10 == 5) return;
			button->field_10 = 1;
			//Doesn't look like this sub does anything
			this->field_5E = 9;
			break;
		case 6:
			if (this->field_84)
				SoundMng::CYySePlayCancel();
			if ((this->field_34 & 2) != 0)
				break;
			TkManager::g_CTkMenuMng.DestroyDrawMenu(this, 1);
			return;
		case 9:
			if (!button) break;
			if (this->field_85)
				SoundMng::CYySePlayCursor();
			if (button->field_10 == 5) return;
			button->field_28 = -1;
			button->SetMenuCtrlID(1);
			break;
		case 20:
			if (button && button->field_10 != 5)
				button->field_10 = 2;
			break;
		case 29:
			if (!button) break;
			if (button->field_10 == 5) return;
			button->field_10 = 1;
			break;
		default:
			break;
		}
	}

	if (!this->MenuPrimitive) return;
	if (this->field_10 == 14) return;
	if (!this->MenuData) return;
	if (!TkManager::g_CTkMenuMng.IsEnableCreateDrawMenu(this->MenuData->field_46))
		return;

	this->MenuPrimitive->OnKeyDown(a2, this->field_4C);
}

void FFXI::CTk::CTkMenuCtrlData::ReInit()
{
	if (this->field_6F > 0) {
		TkManager::g_pCTkInputCtrl->field_6A -= this->field_6F;
		this->field_6F = 0;
	}

	this->field_70 = 0;
	this->field_6E = 0;
}

void FFXI::CTk::CTkMenuCtrlData::DoSomething()
{
	CTkNode* Head = this->DrawCtrlList.GetHeadPosition();
	CTkDrawCtrlFrame* v2{ nullptr };
	while (Head) {
		CTkObject* Next = this->DrawCtrlList.GetNext(&Head);
		if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Button) {
			CTkDrawCtrlButton* button = (CTkDrawCtrlButton*)Next;
			//Assuming a frame must come before anything else so v2 is not null here
			if (this->field_94 != -999)
				button->LocateCalc(v2->field_1A, v2->field_1C, 0.0);
			button->ShapeCalc(this->field_99);
		}
		else if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Cursol) {
			CTkDrawCtrlCursol* cursol = (CTkDrawCtrlCursol*)Next;
			if (this->field_94 != -999)
				cursol->LocateCalc(this->field_8);
			cursol->ShapeCalc();
		}
		else if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Frame) {
			CTkDrawCtrlFrame* frame = (CTkDrawCtrlFrame*)Next;
			v2 = frame;

			frame->ShapeCalc(this->field_99);

			if (this->field_94 < 0) {
				if (this->field_94 != -999) {
					frame->field_1A = this->field_8C;
					frame->field_1C = this->field_8E;
					frame->field_22 = this->field_90;
					frame->field_24 = this->field_92;
				}
			}
			else {
				this->field_94 -= CYyDb::g_pCYyDb->CheckTick();
				int v25 = this->field_8C - frame->field_1A;
				double v9 = (double)v25 * 0.55555558;
				if (v9 < 1.0) {
					if (v9 > -1.0 && this->field_8C != frame->field_1A) {
						if (frame->field_1A <= this->field_8C)
							v9 = 1.0;
						else
							v9 = -1.0;
					}
				}
				frame->field_1A += v9;

				v25 = this->field_8E - frame->field_1C;
				v9 = (double)v25 * 0.55555558;
				if (v9 < 1.0) {
					if (v9 > -1.0 && this->field_8E != frame->field_1C) {
						if (frame->field_1C <= this->field_8E)
							v9 = 1.0;
						else
							v9 = -1.0;
					}
				}

				frame->field_1C += v9;
				frame->field_22 = this->field_8C + this->field_90 - frame->field_1A;
				frame->field_24 = this->field_8E + this->field_92 - frame->field_1C;
			}

			this->field_3A.Left = frame->field_1A;
			this->field_3A.Top = frame->field_1C;
			this->field_3A.Right = frame->field_1A + frame->field_22;
			this->field_3A.Bottom = frame->field_1C + frame->field_24;
		}
	}

	if (this->field_94 <= 0)
		this->field_94 = -999;

	if (this->field_10 == 14) {
		if (this->field_66 <= 0) {
			if (this->field_64 <= 0) {
				if (this->field_6E) {
					this->field_70 = 1;
					return;
				}
			}
			else
				this->field_64 -= CYyDb::g_pCYyDb->CheckTick();
		}
		else
			this->field_66 -= CYyDb::g_pCYyDb->CheckTick();
	}

	if (this->field_5E > 0) {
		this->field_5E -= CYyDb::g_pCYyDb->CheckTick();
		if (this->field_5E < 0)
			this->field_5E = 0;
	}
}

void FFXI::CTk::CTkMenuCtrlData::startSizWindow()
{
	this->MenuViewport = *FFXI::CYy::CDx::instance->GetTopViewport();

	D3DVIEWPORT8 Viewport = this->MenuViewport;
	Viewport.Y = this->field_9E;
	Viewport.X = this->field_9C;
	Viewport.Height = this->field_A2- Viewport.Y;
	Viewport.Width = this->field_A0 - Viewport.X;

	FFXI::CYy::CDx::instance->AddSetViewport(&Viewport);
}

void FFXI::CTk::CTkMenuCtrlData::endSizWindow()
{
	FFXI::CYy::CDx::instance->AddSetViewport(&CTkMenuCtrlData::MenuViewport);
}

void FFXI::CTk::CTkMenuCtrlData::GetWindowLocate(TKRECT* a2)
{
	if (this->field_99) {
		float x{}, y{};
		TkManager::g_CTkMenuMng.GetWinCoefficient(&x, &y);

		a2->Left = (double)this->field_3A.Left * x;
		a2->Top = (double)this->field_3A.Top * y;
		a2->Right = (double)this->field_3A.Right * x;
		a2->Bottom = (double)this->field_3A.Left * y;
	}
	else {
		*a2 = this->field_3A;
	}
}

void FFXI::CTk::CTkMenuCtrlData::GetWindowLocateCompen(TKRECT* a2)
{
	this->GetWindowLocate(a2);

	short DrawLocalOffsetX = this->GetDrawLocalOffsetX();
	short DrawLocalOffsetY = this->GetDrawLocalOffsetY();

	a2->Left += DrawLocalOffsetX;
	a2->Right += DrawLocalOffsetX;
	a2->Top += DrawLocalOffsetY;
	a2->Bottom += DrawLocalOffsetY;
	

}

void FFXI::CTk::CTkMenuCtrlData::GetOriginalRect(TKRECT* a2)
{
	*a2 = this->field_42;
}

short FFXI::CTk::CTkMenuCtrlData::GetDrawLocalOffsetX()
{
	CTkDrawCtrlFrame* frame = this->GetDrawCtrlFrame();
	if (frame) {
		return *(short*)(frame->MenuFrame->Buffer + 14);
	}

	return 0;
}

short FFXI::CTk::CTkMenuCtrlData::GetDrawLocalOffsetY()
{
	CTkDrawCtrlFrame* frame = this->GetDrawCtrlFrame();
	if (frame) {
		return *(short*)(frame->MenuFrame->Buffer + 16);
	}

	return 0;
}

void FFXI::CTk::CTkMenuCtrlData::ReqWindowClose(char)
{
	this->field_10 = 14;
	this->field_66 = 0;
	if (this->field_68)
		this->field_64 = CTkDrawCtrlFrame::AnimOnValue;
	else
		this->field_64 = 0;
}

void FFXI::CTk::CTkMenuCtrlData::ResizeWindow(short a2, short a3, short a4, short a5, char a6, char a7, char a8) 
{
	CTkNode* Head = this->DrawCtrlList.GetHeadPosition();
	CTkDrawCtrlFrame* lastFrame{ nullptr };

	while (Head) {
		CTkObject* Next = this->DrawCtrlList.GetNext(&Head);
		if (!Next)
			break;
		if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Button) {
			CTkDrawCtrlButton* button = (CTkDrawCtrlButton*)Next;

			button->LocateCalc(lastFrame->field_1A, lastFrame->field_1C, this->field_99);
		}
		else if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Cursol) {
			CTkDrawCtrlCursol* cursol = (CTkDrawCtrlCursol*)Next;

			cursol->LocateCalc(this->field_8);
		}
		else if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Frame) {
			CTkDrawCtrlFrame* frame = (CTkDrawCtrlFrame*)Next;
			
			this->field_8C = a2;
			this->field_8E = a3;
			this->field_90 = a4;
			this->field_92 = a5;

			frame->field_1E += (a2 - this->field_3A.Left);
			frame->field_20 += (a3 - this->field_3A.Top);

			lastFrame = frame;
			if (!a7) {
				frame->field_1A = this->field_8C;
				frame->field_1C = this->field_8E;
				frame->field_22 = this->field_90;
				frame->field_24 = this->field_92;
			}

			this->field_3A.Left = a2;
			this->field_3A.Right = a2 + a4;
			this->field_3A.Top = a3;
			this->field_3A.Bottom = a5 + a3;
			this->field_94 = 0;
		}
	}

	if (this->ScrollBar) {
		throw "NOT IMPLEMENTED";
	}

	this->field_9A = a8;
}

void FFXI::CTk::CTkMenuCtrlData::BlurFrame(KaWindow* a2, short a3, short a4, short a5, short a6)
{
	int v6 = 0x80808080;

	double v7 = (double)(a5 - a6) / (double)a5;

	if (v7 < 0.0)
		v7 = 0.0;

	float v16 = 1.0;
	float v17 = 1.0;

	float v21 = v7 * v7;

	if (this->field_99)
		TkManager::g_CTkMenuMng.GetWinCoefficient(&v16, &v17);

	a2->Draw(a3, a4, v21 * v16, v21 * v17, 0x80808080, 0, 0);

	for (int i = 1; i < 2; ++i) {
		double v11 = (double)i * v7 * 0.6f + v7;
		double v12 = 96.0 - 0.625 * v11 * 128.0;
		unsigned int v12i = v12;
		if (v12i > 0x80)
			v12i = 0x80;
		v6 = (v12i << 24) | v6 & 0xFFFFFF;

		float a3a = v16 * v11;
		float v19 = v11 * v17;

		if (i)
			a2->Draw(a3, a4, a3a, v19, v6, 68, 0);
		else
			a2->Draw(a3, a4, a3a, v19, v6, 0, 0);
	}
}

void FFXI::CTk::CTkMenuCtrlData::BlurFrame(_49SubList* a2, short a3, short a4, short a5, short a6)
{
	double v7 = (double)(a5 - a6) / (double)(a5);
	float v13 = v7;
	int SomeColor = 0x80808080;
	if (v7 < 0.0)
		v13 = 0.0;
	float v14 = v13 * v13;
	a2->ExtraDraw(a3, a4, v13, v13, 0x80808080, 1, 0, 0);

	for (int i = 1; i < 2; ++i) {
		double v9 = (double)i * v13 * 0.6f + v13;
		float v12 = v9;
		double alpha = 128.0 - v9 * 0.625 * 128.0;
		unsigned int ualpha = alpha;
		if (ualpha > 128)
			ualpha = 128;

		SomeColor = (ualpha << 24) | SomeColor & 0xFFFFFF;
			
		if (i)
			a2->ExtraDraw(a3, a4, v12, v12, SomeColor, 1, 68, 0);
		else
			a2->ExtraDraw(a3, a4, v12, v12, SomeColor, 1, 0, 0);
	}
}

void FFXI::CTk::CTkMenuCtrlData::Blur4Draw(_49SubList* a2, int a3, int a4, int a5, int a6)
{
	this->Blur4DrawHelper(a2, a3, a4, 1.0, 1.0, a5, a6);
}

void FFXI::CTk::CTkMenuCtrlData::Blur4DrawHelper(_49SubList* a2, int a3, int a4, float a5, float a6, int a7, int a8)
{
	if (this->field_99)
		TkManager::g_CTkMenuMng.GetWinCoefficient(&a5, &a6);
	a2->ExtraDraw(a3, a4, a5, a6, 0x80805020, 0, 0, 0);

	float diff = (a7 - a8);
	float scaled = 0.625f * (double)a7;
	float someFloat = (a7 - scaled) * 0.5f;

	for (int i = 0; i < 2; ++i) {
		double someValue = diff - someFloat * (float)i;
		if (someValue >= 0.0f) {
			if (someValue <= scaled) {
				double v18 = 1.0f / scaled;
				double v19 = someValue * v18 * .3f + 1.0f;
				float a4a = a5 * v19;
				float argCa = v19 * a6;
				double alpha = 1.0f - v18 * someValue;
				if (alpha < 0.0)
					alpha = 0.0;
				D3DCOLOR v21 = ((int)(alpha * 128.0) << 24) | 0x805020;
				if (i)
					a2->ExtraDraw(a3, a4, a4a, argCa, v21, 1, 0x44u, 0);
				else
					a2->ExtraDraw(a3, a4, a4a, argCa, v21, 1, 0, 0);
			}
		}
	}
}

bool FFXI::CTk::CTkMenuCtrlData::GetTargetButtonPoint(tagPOINT* tp)
{
	CTkDrawCtrlButton* Button = this->GetDrawCtrlButton(this->field_4C);

	if (!Button) return false;

	tp->x = Button->ButtonLeft + Button->ButtonWidth / 2;
	tp->y = Button->ButtonTop + Button->ButtonHeight / 2;
	return true;
}

bool FFXI::CTk::CTkMenuCtrlData::IsOnWindow(const tagPOINT& a2)
{
	return this->field_3A.Top <= a2.y
		&& this->field_3A.Bottom >= a2.y
		&& this->field_3A.Left <= a2.x
		&& this->field_3A.Right >= a2.x;
}

short FFXI::CTk::CTkMenuCtrlData::IsOnButton(const tagPOINT& a2, bool* a3)
{
	CTkMouse* mouse = TkManager::g_pCTkMouse;

	if (!mouse) return -1;
	if (mouse->field_48 > 0.0) return -1;
	if (mouse->field_4E == 0) return -1;

	CTkNode* head = this->DrawCtrlList.GetHeadPosition();
	if (!head) return -1;

	CTkDrawCtrlButton* button{ nullptr };
	while (true) {
		button = (CTkDrawCtrlButton*)this->DrawCtrlList.GetNext(&head);
		if (button->DrawType == Constants::Enums::TkDrawType::TKDraw_Button) {
			button->field_49 = 0;

			short word7 = *((short*)button->MenuButton->Buffer + 7);
			short word8 = *((short*)button->MenuButton->Buffer + 8);

			if (button->ButtonLeft + word7 <= a2.x
				&& button->ButtonLeft + word7 + button->ButtonWidth >= a2.x
				&& button->ButtonTop + word8 <= a2.y
				&& button->ButtonTop + word8 + button->ButtonHeight >= a2.y
				&& button->field_10 != 16) {
				if (this->field_77)
					break;
			}
		}
		if (!head)
			return -1;
	}
	
	//button can't be nullptr here.
	if (a3)
		*a3 = button->field_10 != 5;
	button->field_49 = 1;
	return button->field_3A;
}

char* FFXI::CTk::CTkMenuCtrlData::GetSomeHelpText()
{
	if (TkManager::g_CTkMenuMng.CurMCD != this)
		return nullptr;

	CTkNode* Head = this->DrawCtrlList.GetHeadPosition();
	while (Head) {
		CTkDrawCtrlFrame* Next = (CTkDrawCtrlFrame*)this->DrawCtrlList.GetNext(&Head);
		if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Frame) {
			if (Next->field_3C)
				return Next->field_3C;
			else
				return Next->field_28;
		}
	}

	return nullptr;
}

char* FFXI::CTk::CTkMenuCtrlData::GetSomeTitleText()
{
	if (TkManager::g_CTkMenuMng.CurMCD != this)
		return nullptr;

	CTkNode* Head = this->DrawCtrlList.GetHeadPosition();
	while (Head) {
		CTkDrawCtrlFrame* Next = (CTkDrawCtrlFrame*)this->DrawCtrlList.GetNext(&Head);
		if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Frame) {
			if (Next->field_40)
				return Next->field_40;
			else
				return Next->field_2C;
		}
	}

	return nullptr;
}

CTkDrawCtrlButton* FFXI::CTk::CTkMenuCtrlData::GetDrawCtrlButton(unsigned short a2)
{
	CTkNode* Head = this->DrawCtrlList.GetHeadPosition();

	while (true) {

		if (!Head)
			return nullptr;

		CTkDrawCtrlButton* Next = (CTkDrawCtrlButton*)this->DrawCtrlList.GetNext(&Head);
		if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Button) {
			if (Next->field_3A == a2)
				return Next;
		}
	}
}

void FFXI::CTk::CTkMenuCtrlData::GetButtonLocate(TKRECT* a2, short a3, bool a4, bool a5)
{
	CTkDrawCtrlButton* button = this->GetDrawCtrlButton(a3);
	if (!button) return;

	int v7 = button->ButtonLeft;
	int v8 = button->ButtonTop;

	if (button->field_3A == this->field_4C && this->field_63 && a4) {
		if (button->field_10 == 5) {
			++v7;
			++v8;
		}
		else {
			--v7;
			--v8;
			if (a5) {
				if (this->field_5E > 0) {
					v7 += 2;
					v8 += 2;
				}
			}
		}
	}

	a2->Left = v7;
	a2->Top = v8;
	a2->Right = v7 + button->ButtonWidth;
	a2->Bottom = v8 + button->ButtonHeight;
}

void FFXI::CTk::CTkMenuCtrlData::SetButtonLocate(TKRECT* a2, short a3)
{
	CTkDrawCtrlButton* button = this->GetDrawCtrlButton(a3);
	if (!button) return;

	button->ButtonLeft = a2->Left;
	button->ButtonTop = a2->Top;
	button->ButtonWidth = a2->Right - a2->Left;
	button->ButtonHeight = a2->Bottom - a2->Top;

	if (this->field_4C == a3)
		this->RepositionCursol(a3);
}

void FFXI::CTk::CTkMenuCtrlData::DisableDrawButton(short a2, bool a3)
{
	CTkDrawCtrlButton* button = this->GetDrawCtrlButton(a2);
	if (button) {
		button->field_3C = 0;
		if (!a3)
			button->field_4A = 0;
	}
}

CTkDrawCtrlFrame* FFXI::CTk::CTkMenuCtrlData::GetDrawCtrlFrame()
{
	CTkNode* Head = this->DrawCtrlList.GetHeadPosition();

	while (Head) {

		CTkDrawCtrlFrame* Next = (CTkDrawCtrlFrame*)this->DrawCtrlList.GetNext(&Head);
		if (Next->DrawType == Constants::Enums::TkDrawType::TKDraw_Frame)
			return Next;
	}

	return nullptr;
}

void FFXI::CTk::CTkMenuCtrlData::SetButtonStatus(int a2, int a3)
{
	CTkNode* head = this->DrawCtrlList.GetHeadPosition();
	while (head) {
		CTkObject* next = this->DrawCtrlList.GetNext(&head);
		if (next->DrawType == FFXI::Constants::Enums::TkDrawType::TKDraw_Button) {
			CTkDrawCtrlButton* button = (CTkDrawCtrlButton*)next;
			if (button->field_3A == a3) {
				button->SetMenuCtrlID(a2);
				break;
			}
		}
	}
}
