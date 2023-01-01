#include "SoundMng.h"
#include "SoundController.h"
#include "CommandHandlers.h"
#include "MusicServer.h"
#include "Globals.h"
#include "Enums.h"
#include "Values.h"
#include "CYySepRes.h"
#include "CYyDb.h"
#include "CMoResourceMng.h"
#include "CYyResourceFile.h"
#include "CYySoundElem.h"
#include "CommandLineConfig.h"
#include <iostream>

using namespace FFXI;
SoundMng* FFXI::SoundMng::g_pSoundMng{ nullptr };
DWORD WINAPI ThreadProc(LPVOID lpParam);

const char* SoundMng::BGMPrefix = "BGMStream\0";
IDirectSound8* SoundMng::DirectSound8{ nullptr };
LPTHREAD_START_ROUTINE SoundMng::SoundThreadProc = ThreadProc;
bool SoundMng::SoundThreadRunning{ false };
SoundController* SoundMng::SoundController{ nullptr };

CYy::CYySepRes** SoundMng::SepCursor{ nullptr };
CYy::CYySepRes** SoundMng::SepClick{ nullptr };
CYy::CYySepRes** SoundMng::SepBeep{ nullptr };
CYy::CYySepRes** SoundMng::SepCancel{ nullptr };
CYy::CYySepRes** SoundMng::SepWindowSelect{ nullptr };

bool SoundMng::sound_command_send{ false };

float SoundMng::SomeFloat1{ 1.0 };
float SoundMng::SomeFloat2{ 1.0 };

const SoundMng::MusicTableEntry SoundMng::MusicTable[] = {
		{0, 0},
		{2, 0},
		{1, 0},
		{1, 1},
		{1, 2},
		{1, 3},
		{1, 4}
};

const SoundMng::MusicTable2Entry SoundMng::MusicTable2[] = {
	{0, 0, 0},
	{0x6C, 0, 0x6C},
	{0xF2, 0x1FA4, 0x6C},
	{0xB3, 0x1BE4, 0x6C},
	{0x8C, 0x1FE0, 0x6C},
	{0x3A, 0x1FE0, 0x6C}
};

const int SoundMng::SomeAudioTable[] = {
	0, 0x924, 0x9CF, 0x0A49, 0x0AA7, 0x0AF4, 0x0B35, 0x0B6D, 0x0B9F,
	0x0BCB, 0x0BF4, 0x0C18, 0x0C3A, 0x0C59, 0x0C77, 0x0C92, 0x0CAB,
	0x0CC4, 0x0CDA, 0x0CF0, 0x0D05, 0x0D18, 0x0D2B, 0x0D3D, 0x0D4E,
	0x0D5F, 0x0D6F, 0x0D7E, 0x0D8D, 0x0D9B, 0x0DA9, 0x0DB6, 0x0DC3,
	0x0DD0, 0x0DDC, 0x0DE8, 0x0DF4, 0x0DFF, 0x0E0A, 0x0E15, 0x0E1F,
	0x0E29, 0x0E33, 0x0E3D, 0x0E46, 0x0E50, 0x0E59, 0x0E62, 0x0E6A,
	0x0E73, 0x0E7B, 0x0E83, 0x0E8B, 0x0E93, 0x0E9B, 0x0EA3, 0x0EAA,
	0x0EB1, 0x0EB9, 0x0EC0, 0x0EC7, 0x0ECE, 0x0ED4, 0x0EDB, 0x0EE1,
	0x0EE8, 0x0EEE, 0x0EF5, 0x0EFB, 0x0F01, 0x0F07, 0x0F0D, 0x0F12,
	0x0F18, 0x0F1E, 0x0F23, 0x0F29, 0x0F2E, 0x0F34, 0x0F39, 0x0F3E,
	0x0F44, 0x0F49, 0x0F4E, 0x0F53, 0x0F58, 0x0F5D, 0x0F61, 0x0F66,
	0x0F6B, 0x0F6F, 0x0F74, 0x0F79, 0x0F7D, 0x0F82, 0x0F86, 0x0F8A,
	0x0F8F, 0x0F93, 0x0F97, 0x0F9C, 0x0FA0, 0x0FA4, 0x0FA8, 0x0FAC,
	0x0FB0, 0x0FB4, 0x0FB8, 0x0FBC, 0x0FC0, 0x0FC3, 0x0FC7, 0x0FCB,
	0x0FCF, 0x0FD2, 0x0FD6, 0x0FDA, 0x0FDD, 0x0FE1, 0x0FE4, 0x0FE8,
	0x0FEB, 0x0FEF, 0x0FF2, 0x0FF5, 0x0FF9, 0x0FFC, 0x1000
};

const int SoundMng::SomeAudioTable2[] = {
	0x1000, 0x0FFE, 0x0FFC, 0x0FFA, 0x0FF8, 0x0FF6, 0x0FF4, 0x0FF2,
	0x0FF1, 0x0FEF, 0x0FED, 0x0FEB, 0x0FE9, 0x0FE7, 0x0FE5, 0x0FE3,
	0x0FE1, 0x0FDF, 0x0FDD, 0x0FDB, 0x0FD9, 0x0FD7, 0x0FD5, 0x0FD3,
	0x0FD2, 0x0FD0, 0x0FCE, 0x0FCC, 0x0FCA, 0x0FC8, 0x0FC6, 0x0FC4,
	0x0FC2, 0x0FC0, 0x0FBE, 0x0FBC, 0x0FBA, 0x0FB8, 0x0FB6, 0x0FB4,
	0x0FB3, 0x0FB1, 0x0FAF, 0x0FAD, 0x0FAB, 0x0FA9, 0x0FA7, 0x0FA5,
	0x0FA3, 0x0FA1, 0x0F9F, 0x0F9D, 0x0F9B, 0x0F99, 0x0F97, 0x0F95,
	0x0F94, 0x0F92, 0x0F90, 0x0F8E, 0x0F8C, 0x0F8A, 0x0F88, 0x0F86
};

const char* MusicDirsDev[8] = {
	"c:\\image\\ffxi\\sound\\win\\music\\data\0",
	"c:\\image\\ffxi_ex\\sound\\win\\music\\data\0",
	"c:\\image\\ffxi_ex2\\sound\\win\\music\\data\0",
	"c:\\image\\ffxi\\TestUser\\ffxi_ex3\\sound\\win\\music\\data\0",
	"c:\\image\\ffxi\\TestUser\\ffxi_ex4\\sound\\win\\music\\data\0",
	"c:\\image\\ffxi\\TestUser\\ffxi_dl\\sound\\win\\music\\data\0",
	"c:\\image\\ffxi\\TestUser\\ffxi_ex5\\sound\\win\\music\\data\0",
	"c:\\image\\ffxi\\TestUser\\ffxi_gf\\sound\\win\\music\\data\0"
};
const char* SoundEffectDirsDev[8] = {
	"c:\\image\\ffxi\\sound\\win\\se\0",
	"c:\\image\\ffxi_ex\\sound\\win\\se\0",
	"c:\\image\\ffxi_ex2\\sound\\win\\se\0",
	"c:\\image\\ffxi\\TestUser\\ffxi_ex3\\sound\\win\\se\0",
	"c:\\image\\ffxi\\TestUser\\ffxi_ex4\\sound\\win\\se\0",
	"c:\\image\\ffxi\\TestUser\\ffxi_dl\\sound\\win\\se\0",
	"c:\\image\\ffxi\\TestUser\\ffxi_ex5\\sound\\win\\se\0",
	"c:\\image\\ffxi\\TestUser\\ffxi_gf\\sound\\win\\se\0",
};
const char* RelativeMusicDirs[9] = {
	"\\sound\\win\\music\\data",
	"\\sound2\\win\\music\\data",
	"\\sound3\\win\\music\\data",
	"\\sound4\\win\\music\\data",
	"\\sound5\\win\\music\\data",
	"\\sound6\\win\\music\\data",
	"\\sound7\\win\\music\\data",
	"\\sound8\\win\\music\\data",
	"\\sound9\\win\\music\\data"
};
const char* RelativeSoundEffectDirs[9] = {
	"\\sound\\win\\se\0",
	"\\sound2\\win\\se\0",
	"\\sound3\\win\\se\0",
	"\\sound4\\win\\se\0",
	"\\sound5\\win\\se\0",
	"\\sound6\\win\\se\0",
	"\\sound7\\win\\se\0",
	"\\sound8\\win\\se\0",
	"\\sound9\\win\\se\0",
};

void SetMusicDir(char* a1, int index) {
	if (index >= Constants::Values::INDEX_SOUND_MAX) return;

	char* dir = SoundController::MusicDirs + 0x108 * index;
	char v5{};
	if (a1) {
		int index = 0;
		do {
			v5 = a1[index];
			dir[4 + index] = v5;
			++index;
		} while (v5);
	}
	else {
		dir[4] = 0;
	}
}

void SetSoundEffectDir(char* a1, int index) {
	if (index >= Constants::Values::INDEX_SOUND_MAX) return;

	char* dir = SoundController::SoundEffectDirs + 0x108 * index;
	char v5{};
	if (a1) {
		int index = 0;
		do {
			v5 = a1[index];
			dir[4 + index] = v5;
			++index;
		} while (v5);
	}
	else {
		dir[4] = 0;
	}
}

void SetOOFFlag(int a1) {
	SoundController::SoundOOFFlag = a1 != 0 ? DSBCAPS_GLOBALFOCUS : NULL;
}

DWORD WINAPI ThreadProc(LPVOID lpParam) {
	SoundMng::ThreadParam* param = static_cast<SoundMng::ThreadParam*>(lpParam);
	while (param->shouldRun) {
		SoundController::UpdateBin1();
		SoundController::UpdateBin2();
		SoundController::UpdateQueue();
		SoundMng::SoundController->field_4 &= 0xFFFFFFFE;
		Sleep(0x10u);
	}
	return 0;
}

void FFXI::SoundMng::ParseCLA()
{
	char v2[256], Str[256];
	sprintf_s(v2, sizeof(v2), "-%ssound", "0001");
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam(v2, Str, sizeof(Str), 0) != nullptr) {
		if (strstr(Str, "off")) {
			FFXI::Config::RegistryConfig::g_pOptions->Main.EnableSound = false;
		}
	}
}

bool FFXI::SoundMng::InitSound(HWND hwnd)
{
	unsigned int curdir_len = GetCurrentDirectoryA(NULL, NULL);
	char* curdir = new char[curdir_len];
	GetCurrentDirectoryA(curdir_len, curdir);
	bool ret = SoundMng::Init(hwnd, curdir);
	if (curdir != nullptr) {
		delete[] curdir;
	}
	return ret;
}

void FFXI::SoundMng::UninitSound()
{
	if (FFXI::SoundMng::g_pSoundMng != nullptr) {
		FFXI::SoundMng::g_pSoundMng->Clean();
		delete FFXI::SoundMng::g_pSoundMng;
		FFXI::SoundMng::g_pSoundMng = nullptr;
	}
}

FFXI::SoundMng::SoundMng()
{
	SoundMng::SoundThreadRunning = false;
	SoundMng::DirectSound8 = nullptr;
	this->field_0 = 1;
	this->field_4 = 0;
	this->Param = nullptr;

	this->Param = new ThreadParam;
	SoundMng::SoundController = new FFXI::SoundController();

	if (SoundMng::SoundController && this->Param) {
		if (SoundMng::SoundController->field_0 == 1) {
			this->Param->shouldRun = 0;
			this->Param->handle = nullptr;
			this->Param->field_4 = 0;
			this->Param->field_C = 0;
			return;
		}
	}
	else {
		this->field_0 = 0;
		if (SoundMng::SoundController) {
			delete SoundMng::SoundController;
			SoundMng::SoundController = nullptr;
		}
		if (this->Param) {
			delete this->Param;
			this->Param = nullptr;
		}
	}
}

FFXI::SoundMng::~SoundMng()
{
	if (this->Param && this->Param->handle)
		SoundMng::CleanThread(this->Param);

	if (SoundMng::SoundController) {
		delete SoundMng::SoundController;
		SoundMng::SoundController = nullptr;
	}

	if (SoundMng::DirectSound8) {
		SoundMng::DirectSound8->Release();
		SoundMng::DirectSound8 = nullptr;
	}

	if (this->Param)
		delete this->Param;
}

void FFXI::SoundMng::SqsSeVolumeDirect(int a1, int a2, int a3)
{
	if (a1 < 0)
	{
		return;
	}

	if (a2 < 0) {
		a2 = 127;
	}
	else if (a2 > 127) {
		a2 = 127;
	}
	if (FFXI::SoundMng::g_pSoundMng != nullptr) {
		FFXI::SoundMng::g_pSoundMng->QueueNewSoundCommand(FFXI::Constants::Enums::SoundCommand::VolumeDirect, a1, 0, a2 & 0x7F, a3, 0, 0, 0);
	}
}

void FFXI::SoundMng::SqsSePanDirect(int a1, int a2, int a3)
{
	if (FFXI::SoundMng::g_pSoundMng != nullptr) {
		FFXI::SoundMng::g_pSoundMng->QueueNewSoundCommand(FFXI::Constants::Enums::SoundCommand::PanDirect, a1, 0, a2 & 0x7F, a3, 0, 0, 0);
	}
}

void FFXI::SoundMng::QueueNewSoundCommand(int CommandCode, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
	unsigned int* v8 = SoundController::CommandQueueReadCursor;
	unsigned int* v9 = SoundController::CommandQueueWriteCursor;
	unsigned int* v10{ nullptr };
	if (v9 >= v8)
		v8 = SoundController::CommandQueueReadCursor + SoundController::CommandQueueSize;

	if (CommandCode == Constants::Enums::SoundCommand::PlaySoundFromDisk) {
		if (v8 <= v9 + 0x48) return;
		v9[0] = 7;
		v9[1] = a3;
		v9[2] = a4;

		v10 = v9 + 8;
		char* v11 = (char*)a2;
		for (int i = 0; i < 8; ++i) {
			if (v10 >= SoundController::CommandQueue + SoundController::CommandQueueSize) {
				v10 = SoundController::CommandQueue;
			}
			memcpy(v10, v11, 32);
			v10 += 8;
			v11 += 32;
		}
		if (v10 >= SoundController::CommandQueue + SoundController::CommandQueueSize)
			v10 = SoundController::CommandQueue;
	}
	else {
		if (v8 <= SoundController::CommandQueueWriteCursor + 8)
			return;

		v9[0] = CommandCode;
		v9[1] = (int)a2;
		v9[2] = a3;
		v9[3] = a4;
		v9[4] = a5;
		v9[5] = a6;
		v9[6] = a7;
		v9[7] = a8;
		v10 = v9 + 8;
		if (v10 >= SoundController::CommandQueue + SoundController::CommandQueueSize) {
			SoundController::CommandQueueWriteCursor = SoundController::CommandQueue;
			return;
		}
	}
	SoundController::CommandQueueWriteCursor = v10;
}

void FFXI::SoundMng::PlayMusicFromDisk(int a1, char a2, int a3)
{
	char* v9{ nullptr };
	char v11{};
	int index{};

	if (!FFXI::SoundMng::g_pSoundMng) return;

	char* MusicDir = SoundController::MusicDirs + 4;
	if (!*MusicDir) return;

	int a1a = a1 & 0x3FF;
	char* v4 = new char[0x104];
	char* v13[Constants::Values::INDEX_SOUND_MAX];
	FILE* v8{ nullptr };
	if (v4) {
		int v5 = 0;
		int a2a = a2 & 0x7F;
		do {
			v13[v5++] = new char[0x104];
		} while (v5 < Constants::Values::INDEX_SOUND_MAX);
		int v6 = 0;
		while (true) {
			if (v13[v6])
				sprintf_s(v13[v6], 0x104, "%s\\music%3.3u.bgw", MusicDir, a1a);
			fopen_s(&v8, v13[v6], "rb");
			if (v8)
				break;
			MusicDir += 0x108;
			if (++v6 >= Constants::Values::INDEX_SOUND_MAX)
				goto LABEL_13;
		}
		v9 = v13[v6];
		v11 = 0;
		index = 0;
		do {
			v11 = v9[index];
			v4[index] = v11;
			++index;
		} while (v11);
	LABEL_13:
		if (v8) {
			fclose(v8);
			SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::PlaySoundFromDisk, (int)v4, a2a, a3, 0, 0, 0, 0);
		}
		delete[] v4;
		for (int i = 0; i < Constants::Values::INDEX_SOUND_MAX; ++i) {
			if (v13[i])
				delete[] v13[i];
		}
	}
}

int FFXI::SoundMng::GetMusicTable2Index(int a1)
{
	char v3[11];
	v3[1] = 2;
	v3[2] = 2;
	v3[0] = 1;
	v3[3] = 3;
	v3[4] = 3;
	v3[7] = 11;
	v3[5] = 4;
	v3[6] = 4;
	v3[8] = 5;
	if (((1 << v3[2 * a1 - 1]) & FFXI::GlobalStruct::g_GlobalStruct.ClientExpansions) != 0)
		return v3[2 * a1];

	return -1;
}

bool FFXI::SoundMng::Init(HWND a1, char* a2)
{
	CYy::CYySoundElem::check_flag = false;

	CYy::CYySoundElem::sound_far = 30.0;
	CYy::CYySoundElem::sound_far = 30.0;
	CYy::CYySoundElem::sound_far = 30.0;

	SoundMng::SoundThreadRunning = 0;
	CYy::CYySoundElem::head = nullptr;
	CYy::CYySoundElem::play_num = 0;
	CYy::CYySoundElem::entry_num = 0;
	CYy::CYySoundElem::sound_standby = false;

	SoundMng::sound_command_send = 0;
	CYy::CYySoundElem::se_req_num = 0;
	CYy::CYySoundElem::zone_sound_id = 0;
	CYy::CYySoundElem::zone_sound = nullptr;

	CYy::CYySoundElem::system_volume = 1.0;
	SoundMng::SomeFloat1 = 1.0;
	SoundMng::SomeFloat2 = 1.0;
	MusicServer::last_request = 0;

	SoundMng::SepCursor = 0;
	SoundMng::SepClick = nullptr;
	SoundMng::SepCancel = nullptr;
	SoundMng::SepBeep = nullptr;
	SoundMng::SepWindowSelect = nullptr;

	SoundMng* newSound = new SoundMng();
	FFXI::SoundMng::g_pSoundMng = newSound;

	if (newSound) {
		if (Config::MainRegistryConfig::EnableSound) {
			if (newSound->StartThread(a1)) {
				SoundMng::SoundThreadRunning = true;
			}
		}
		{//Inline func
			newSound->field_4 |= 1;
			newSound->SoundController->field_8 = newSound->field_4;
		}
		if (newSound)
			SetOOFFlag(Config::MainRegistryConfig::WindowOutOfFocusSound);

		int buffsize = 0x104;
		char* Buffer = new char[buffsize];
		if (Buffer) {
			int v6 = FFXI::Config::RegistryConfig::g_pOptions->Other.field_1C;
			Buffer[0x103] = 0;
			if (v6 <= 0 || v6 > 3) {
				for (int i = 0; i < sizeof(MusicDirsDev) / sizeof(MusicDirsDev[0]); ++i) {
					strncpy_s(Buffer, buffsize, MusicDirsDev[i], buffsize - 1);
					if (newSound)
						SetMusicDir(Buffer, i);
				}
				for (int j = 0; j < sizeof(SoundEffectDirsDev) / sizeof(SoundEffectDirsDev[0]); ++j) {
					strncpy_s(Buffer, buffsize, SoundEffectDirsDev[j], buffsize - 1);
					if (newSound)
						SetSoundEffectDir(Buffer, j);
				}
			}
			else {
				for (int k = 0; k < sizeof(RelativeMusicDirs) / sizeof(RelativeMusicDirs[0]); ++k) {
					strncpy_s(Buffer, buffsize, a2, buffsize - 1);
					strncat_s(Buffer, buffsize, RelativeMusicDirs[k], buffsize - 1);
					if (newSound)
						SetMusicDir(Buffer, k);
				}
				for (int m = 0; m < sizeof(RelativeSoundEffectDirs) / sizeof(RelativeSoundEffectDirs[0]); ++m) {
					strncpy_s(Buffer, buffsize, a2, buffsize - 1);
					strncat_s(Buffer, buffsize, RelativeSoundEffectDirs[m], buffsize - 1);
					if (newSound)
						SetSoundEffectDir(Buffer, m);
				}
			}
			delete[] Buffer;
			return true;
		}
	}
	
	FFXI::SoundMng::g_pSoundMng = nullptr;
	return false;
}

void FFXI::SoundMng::CleanThread(ThreadParam* a1)
{
	a1->shouldRun = 0;
	while (WaitForSingleObject(a1->handle, 0)) {
		Sleep(1u);
	}
	CloseHandle(a1->handle);
	a1->handle = nullptr;
}

void FFXI::SoundMng::GetPanAndVolume(int* a1, int* a2)
{
	int audioTableSize = sizeof(SomeAudioTable2) / sizeof(SomeAudioTable2[0]);
	*a1 &= 0x7F;
	int v3 = a1[0];
	if (*a1 == audioTableSize) {
		*a1 = 0;
	}
	else {
		if (*a1 < audioTableSize)
			*a1 ^= (audioTableSize - 1);
		else
			*a1 &= (audioTableSize - 1);
		int v5 = 10000 * *a1;
		*a1 = v5 / (audioTableSize - 1);
		if (v3 >= audioTableSize)
			goto LABEL_8;
		*a1 = v5 / -audioTableSize;
	}
	if (v3 >= audioTableSize) {
	LABEL_8:
		v3 = 127 - v3;
	}
	*a2 = ((10000 * ((*a2 * SomeAudioTable2[v3]) >> 12)) >> 12) - 10000;
}

void FFXI::SoundMng::SetBufferVolume(LPDIRECTSOUNDBUFFER a1, int a2)
{
	if (a2 >= -10000) {
		if (a2 > 0)
			a1->SetVolume(0);
		else
			a1->SetVolume(a2);
	}
	else
		a1->SetVolume(-10000);
}

void FFXI::SoundMng::SetBufferPan(LPDIRECTSOUNDBUFFER a1, int a2)
{
	if (a2 >= -10000) {
		if (a2 > 10000)
			a1->SetPan(10000);
		else
			a1->SetPan(a2);
	}
	else
		a1->SetPan(-10000);
}

bool FFXI::SoundMng::CanPlaySoundResource(CYy::CYySepRes** a1)
{
	if (CYy::CYySoundElem::se_req_num >= Constants::Values::INDEX_SOUND_REQUEST_MAX) return false;

	for (unsigned int i = 0; i < CYy::CYySoundElem::se_req_num; ++i) {
		if (CYy::CYySoundElem::req_buffer[i] == a1)
			return true;
	}

	CYy::CYySoundElem::req_buffer[CYy::CYySoundElem::se_req_num] = a1;
	CYy::CYySoundElem::se_req_num += 1;

	return true;
}

int FFXI::SoundMng::SqsSeCheck(char* a1, float* a2, int a3)
{
	if (!FFXI::SoundMng::g_pSoundMng) {
		return -1;
	}

	int v3 = a3;

	if (v3 < 0)
		v3 = 127;
	else if (v3 > 127)
		v3 = 127;

	if (SoundController::GetBuff1Flag(5, -1, -1) >= Config::MainRegistryConfig::MaxConcurrentSoundEffects) {
		return -1;
	}

	int fileID = *(int*)(a1 + 8);
	if (fileID == 0x8BF || fileID == 0x80F) {
		if (CYy::CYySoundElem::check_flag == true) {
			return 0;
		}
		CYy::CYySoundElem::check_flag = true;
	}

	return FFXI::SoundMng::g_pSoundMng->SqsSePlay(a1, *a2, v3);
}

void FFXI::SoundMng::CYySePlayCursor()
{
	CYy::CMoResource** Unused{ nullptr };
	if (!SepCursor)
		SepCursor = (CYy::CYySepRes**)*(*CYyDb::pCMoResourceMng->Unknown3)->FindResourceUnder(
			&Unused, Constants::Enums::ResourceType::Sep, '1000');
	
	if (SepCursor && *SepCursor)
		(*SepCursor)->Play(64, CYy::CYySoundElem::system_volume * 127.0);
}

void FFXI::SoundMng::CYySePlayClick()
{
	CYy::CMoResource** Unused{ nullptr };
	if (!SepClick)
		SepClick = (CYy::CYySepRes**)*(*CYyDb::pCMoResourceMng->Unknown3)->FindResourceUnder(
			&Unused, Constants::Enums::ResourceType::Sep, '2000');

	if (SepClick && *SepClick)
		(*SepClick)->Play(64, CYy::CYySoundElem::system_volume * 127.0);
}

void FFXI::SoundMng::CYySePlayBeep()
{
	CYy::CMoResource** Unused{ nullptr };
	if (!SepBeep)
		SepBeep = (CYy::CYySepRes**)*(*CYyDb::pCMoResourceMng->Unknown3)->FindResourceUnder(
			&Unused, Constants::Enums::ResourceType::Sep, '4000');

	if (SepBeep && *SepBeep)
		(*SepBeep)->Play(64, CYy::CYySoundElem::system_volume * 127.0);
}

void FFXI::SoundMng::CYySePlayCancel()
{
	CYy::CMoResource** Unused{ nullptr };
	if (!SepCancel)
		SepCancel = (CYy::CYySepRes**)*(*CYyDb::pCMoResourceMng->Unknown3)->FindResourceUnder(
			&Unused, Constants::Enums::ResourceType::Sep, '3000');

	if (SepCancel && *SepCancel)
		(*SepCancel)->Play(64, CYy::CYySoundElem::system_volume * 127.0);
}

void FFXI::SoundMng::CYySePlayWindowSelect()
{
	CYy::CMoResource** Unused{ nullptr };
	if (!SepWindowSelect)
		SepWindowSelect = (CYy::CYySepRes**)*(*CYyDb::pCMoResourceMng->Unknown3)->FindResourceUnder(
			&Unused, Constants::Enums::ResourceType::Sep, '3100');

	if (SepWindowSelect && *SepWindowSelect)
		(*SepWindowSelect)->Play(64, CYy::CYySoundElem::system_volume * 127.0);
}


bool FFXI::SoundMng::StartThread(HWND hwnd)
{
	if (!hwnd) return false;
	if (!this->field_0) return false;
	this->Param->shouldRun = 1;
	this->Param->field_C;
	HANDLE thread = CreateThread(NULL, 0, SoundMng::SoundThreadProc, this->Param, NULL, &this->Param->field_4);
	this->Param->handle = thread;
	if (!thread) return false;
	if (!SetThreadPriority(thread, THREAD_PRIORITY_HIGHEST)) {
		SoundMng::CleanThread(this->Param);
		return false;
	}
	if (DirectSoundCreate8(NULL, &SoundMng::DirectSound8, NULL) >= DS_OK) {
		if (SoundMng::DirectSound8->SetCooperativeLevel(hwnd, 2) >= DS_OK) {
			return true;
		}
		else {
			SoundMng::CleanThread(this->Param);
			if (SoundMng::DirectSound8) {
				SoundMng::DirectSound8->Release();
				SoundMng::DirectSound8 = nullptr;
			}
			return false;
		}
	}
	else {
		SoundMng::CleanThread(this->Param);
		return false;
	}
}

void FFXI::SoundMng::Clean()
{
	if (this->field_0 == 1) {
		SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::_6, -1, 0, 0, 0, 0, 0, 0);
		SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::_0, -1, -1, 0, 0, 0, 0, 0);
		while (SoundController::GetBuff1Flag(0, -1, -1) | SoundController::GetBuff2Flag(0, -1)) {
			Sleep(40);
		}
	}
	if (this->Param && this->Param->handle)
		SoundMng::CleanThread(this->Param);
}

int FFXI::SoundMng::SqsSePlay(char* a2, int a3, int a4)
{
	int v4 = UpdateCounter();
	int v7 = a4 & 0x7F;
	int v6 = a3 & 0x7F;

	if (!memcmp(a2, "SeSep", 5))
		QueueNewSoundCommand(Constants::Enums::SoundCommand::PlaySep, *(int*)(a2 + 8), v4, v6, v7, 0, 0, 0);
	else
		QueueNewSoundCommand(Constants::Enums::SoundCommand::_1, (int)a2, v4, v6, v7, 0, 0, 0);
	
	return v4;
}

int FFXI::SoundMng::UpdateCounter()
{
	int newValue = this->field_8 + 1;
	newValue &= 0x7FFFFFFF;

	if (newValue == 0)
		this->field_8 = 1;
	else
		this->field_8 = newValue;

	return this->field_8;
}
