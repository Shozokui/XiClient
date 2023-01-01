#include "CMoDisplayColorTask.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoTaskMng.h"
#include "CMoDx.h"
using namespace FFXI::CYy;

FFXI::CYy::CMoDisplayColorTask::CMoDisplayColorTask(int type, float durmult, char* data) 
{
	this->Type = type;
	int curColor{};
	CMoDisplayColorTask** curTask{ nullptr };

	switch (this->Type) {
	case 0xF:
		curColor = CYyDb::g_pCYyDb->field_320;
		curTask = &CMoTaskMng::current_display_color_task_f;
		break;
	case 0x51:
		curColor = CYyDb::g_pCYyDb->field_31C;
		curTask = &CMoTaskMng::current_display_color_task_51;
		break;
	case 0x72:
		curColor = CYyDb::g_pCYyDb->field_328;
		curTask = &CMoTaskMng::current_display_color_task_72;
		break;
	default:
		throw "CMoDisplayColorTask ctor: Invalid type";
	}
	if (*curTask)
		delete* curTask;

	this->FinalColorGreen = data[8];
	this->FinalColorBlue = data[9];
	this->FinalColorRed = data[10];
	this->FinalColorAlpha = data[11];
	this->StartColorGreen = ((curColor >> 16) & 0xFF) - this->FinalColorGreen;
	this->StartColorBlue = ((curColor >> 8) & 0xFF) - this->FinalColorBlue;
	this->StartColorRed = ((curColor >> 0) & 0xFF) - this->FinalColorRed;
	this->StartColorAlpha = ((curColor >> 24) & 0xFF) - this->FinalColorAlpha;

	double arg0 = (double)*((short*)data + 3);
	arg0 *= durmult;
	arg0 = floor(arg0);
	arg0 -= 1.0;
	this->TaskDuration = arg0;
	this->TimeLeft = arg0;
	*curTask = this;
}

FFXI::CYy::CMoDisplayColorTask::~CMoDisplayColorTask()
{
	if (this->Type == 0xf) {
		if (CMoTaskMng::current_display_color_task_f == this) {
			CYyDb::g_pCYyDb->SetField338(*(int*)&this->FinalColorRed);
			CMoTaskMng::current_display_color_task_f = nullptr;
		}
	}
	else if (this->Type == 0x51) {
		if (CMoTaskMng::current_display_color_task_51 == this) {
			CYyDb::g_pCYyDb->SetField334(*(int*)&this->FinalColorRed);
			CMoTaskMng::current_display_color_task_51 = nullptr;
		}
	}
	else if (this->Type == 0x72) {
		if (CMoTaskMng::current_display_color_task_72 == this) {
			CYyDb::g_pCYyDb->SetField328(*(int*)&this->FinalColorRed);
			CMoTaskMng::current_display_color_task_72 = nullptr;
		}
	}
}

char FFXI::CYy::CMoDisplayColorTask::OnMove()
{
	double progress = this->TimeLeft / this->TaskDuration;
	char color[4]{};
	color[2] = (double)this->StartColorGreen * progress + (double)this->FinalColorGreen;
	color[1] = (double)this->StartColorBlue * progress + (double)this->FinalColorBlue;
	color[0] = (double)this->StartColorRed * progress + (double)this->FinalColorRed;
	color[3] = (double)this->StartColorAlpha * progress + (double)this->FinalColorAlpha;
	
	switch (this->Type) {
	case 0xF:
		CYyDb::g_pCYyDb->SetField338(*(int*)color);
		break;
	case 0x51:
		CYyDb::g_pCYyDb->SetField334(*(int*)color);
		break;
	case 0x72:
		CYyDb::g_pCYyDb->SetField328(*(int*)color);
		break;
	}

	this->TimeLeft -= CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
	if (this->TimeLeft >= 0.0)
		return false;

	switch (this->Type) {
	case 0xF:
		CYyDb::g_pCYyDb->SetField338(*(int*)&this->FinalColorRed);
		break;
	case 0x51:
		CYyDb::g_pCYyDb->SetField334(*(int*)&this->FinalColorRed);
		break;
	case 0x72:
		CYyDb::g_pCYyDb->SetField328(*(int*)&this->FinalColorRed);
		break;
	}

	delete this;
	return true;
}
