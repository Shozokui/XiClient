#include "MusicServer.h"
#include "CYyMusicLoadTask.h"
#include "CMoTaskMng.h"
#include "Globals.h"
#include "CYyDb.h"
#include "SoundMng.h"
#include <timeapi.h>

using namespace FFXI;

int MusicServer::last_request{ 0 };
CYy::CYyMusicLoadTask* MusicServer::MusicLoadTask{ nullptr };

void PlayFromTable(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int TableIndex, int TableSubIndex) {
	int v9{}, v10{}, v12{}, v14{};
	char v11{}, v13{};

	if (TableIndex == 0) {
		v10 = 4;
		v11 = 0;
	}
	else if (TableIndex == 1) {
		v10 = TableSubIndex;
		v11 = 1;
	}
	else if (TableIndex == 2) {
		v10 = timeGetTime() % 5;
		v11 = 1;
	}

	v12 = v10;
	if (v10 >= 0) {
		while (true) {
			v13 = SoundMng::GetMusicTable2Index(v12);
			if (v13 >= 0)
				break;
			if (--v12 < 0)
				goto LABEL_12;
		}
		v9 = v13;
	}
LABEL_12:
	v14 = SoundMng::MusicTable2[v9].One;
	if (!v14)
		v14 = a1;
	if (MusicServer::last_request != v14) {
		MusicServer::last_request = v14;
		CYy::CMoTaskMng::DeleteThisTask = true;
		char* mem = StorageLists::instance->Get(sizeof(CYy::CYyMusicLoadTask), Constants::Enums::MEM_MODE::Ex);
		if (mem) {
			CYy::CYyMusicLoadTask* newTask = new (mem) CYy::CYyMusicLoadTask();
			newTask->field_35 = 0;
			newTask->field_34 = a7;
			newTask->field_44 = a4;
			newTask->field_3C = v14;
			newTask->field_40 = a3;
			newTask->field_48 = a5;
			newTask->field_4C = a6;
			newTask->field_50 = v9;
			newTask->field_54 = v11;
			if (a6 < 30)
				newTask->field_4C = 30;
			newTask->field_38 = 0;
			if (MusicServer::MusicLoadTask)
				delete MusicServer::MusicLoadTask;
			MusicServer::MusicLoadTask = newTask;
			CYyDb::g_pCYyDb->pCMoTaskMng->SomeShift4(newTask, 5);
		}
	}
}

void FFXI::MusicServer::clear_last_request()
{
	last_request = 0;
}

void FFXI::MusicServer::Play(int index)
{
	PlayFromTable(108, 0, 127, 127, 0, 0, 0, SoundMng::MusicTable[index].Index, SoundMng::MusicTable[index].SubIndex);
}
