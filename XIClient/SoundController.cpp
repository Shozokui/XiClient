#define WIN32_LEAN_AND_MEAN
#include "SoundController.h"
#include "SoundMng.h"
#include "SoundBuff1.h"
#include "SoundBuff2.h"
#include "CommandHandlers.h"
#include "Values.h"
#include "Enums.h"
#include <cmath>
#include <string>
#include <iostream>
using namespace FFXI;

CRITICAL_SECTION SoundController::cs{};

//Min write size is 8 bytes
const int SoundController::CommandQueueSize = 8 * 0x100;
const int SoundController::SoundBuff1BinSize = 12;
const int SoundController::SoundBuff2BinSize = 3;
const int SoundController::AudioPosBackupSize = 0x400;
const int SoundController::SoundEffectCacheSize = 14;

unsigned int* SoundController::CommandQueue{ nullptr };
unsigned int* SoundController::CommandQueueWriteCursor{ nullptr };
unsigned int* SoundController::CommandQueueReadCursor{ nullptr };

SoundBuff2* SoundController::SoundBuff2Bin{ nullptr };
SoundBuff1* SoundController::SoundBuff1Bin{ nullptr };
int* SoundController::AudioSpotBackups{ nullptr };
SoundController::CachedSoundEffect* SoundController::SoundEffectCache{ nullptr };

char* SoundController::MusicDirs{ nullptr };
char* SoundController::SoundEffectDirs{ nullptr };

int SoundController::SoundOOFFlag{ 0 };
int SoundController::SomeValue{ 0 };
float SoundController::AudioStartPos{ 0.0 };

DWORD __stdcall SeWaveProc(LPVOID param) {
	int v5{}, v48{}, v49{}, v56{}, v58{}, v63{};
	unsigned int v57{}, v33{};
	DWORD v59{}, v60{};
	char* v6{ nullptr }, * v7{ nullptr }, * v29{ nullptr }, *v61{ nullptr }, * v62{ nullptr }, * v34{ nullptr };
	HRESULT result{};
	SoundBuff1* sb1 = (SoundBuff1*)param;
	sb1->field_98 = 0;
	memset(sb1->Type0DecodeContext, 0, sizeof(sb1->Type0DecodeContext));
	sb1->field_88.Clean();

	int EncodingType = *(int*)(sb1->FileHeader + 12);
	if (EncodingType == Constants::Enums::AudioEncoding::Encoding0) {
		v57 = 0x6000u / (unsigned char)sb1->FileHeader[44] * (((unsigned char)sb1->FileHeader[44] >> 1) + 1);
	}
	else if (EncodingType == Constants::Enums::AudioEncoding::ATRAC3) {
		throw "NOT SUPPORTED";
		if (sb1->FileHeader[44] == 1) {
			sb1->field_AC = 304;
			sb1->field_B0 = 1;
			sb1->field_B4 = 105;
		}
		else if (sb1->FileHeader[44] == 2) {
			sb1->field_AC = 192;
			sb1->field_B0 = 2;
			sb1->field_B4 = 66;
		}
		else {
			sb1->field_AC = 384;
			sb1->field_B0 = 1;
			sb1->field_B4 = 132;
		}

		v57 = (((49152 / (unsigned char)sb1->FileHeader[42]) >> 11) + 8) * sb1->field_AC;
		if (false) {//sub //TODO
			if (false)
				;
			else
				v57 = 0;
		}
		else {
			v57 = 0;
		}
		sb1->field_BC = 0;
		int v55 = *(int*)(sb1->FileHeader + 32);
		//sub //TODO;
	}
	else 
		v57 = 0xC000;
	

	if (v57 <= 0) {
		sb1->field_48 = 0;
		v56 = 1;
	}
	else {
		v6 = new char[0xC000];
		if (!v6) {
			sb1->field_48 = 0;
			v56 = 1;
		}
		else {
			v7 = new char[v57];
			if (v7) {
				sb1->PrepareSoundData(v7, v6, 0xC000);
				delete[] v7;
			}
			else {
				v56 = 2;
			}

			result = sb1->SecondaryBuffer->Lock(0, 0xC000, (LPVOID*)&v62, &v59, (LPVOID*)&v61, &v60, 0);
			if (result != DS_OK) {
				sb1->field_48 = 0;
				switch (result) {
				case DSERR_PRIOLEVELNEEDED:
					v56 |= 4;
					break;
				case DSERR_INVALIDPARAM:
					v56 |= 0x20;
					break;
				case DSERR_INVALIDCALL:
					v56 |= 0x10;
					break;
				case DSERR_BUFFERLOST:
					v56 |= 8;
					break;
				default:
					v56 |= 0x40;
					break;
				}
				delete[] v6;
			}
			else {
				memcpy(v62, v6, v59);
				if (v61) {
					char* v12 = v6 + v59;
					unsigned v13 = 4 * (v60 >> 2);
					memcpy(v61, v12, v13);
					memcpy(v61 + v13, v12 + v13, (char)v60 & 3);
				}

				sb1->SecondaryBuffer->Unlock(v62, v59, v61, v60);
				sb1->SecondaryBuffer->SetCurrentPosition(0);
				int pan = sb1->field_80 >> 12;
				int volume = sb1->field_70 >> 12;
				SoundMng::GetPanAndVolume(&pan, &volume);
				SoundMng::SetBufferPan(sb1->SecondaryBuffer, pan);
				SoundMng::SetBufferVolume(sb1->SecondaryBuffer, volume);
				sb1->field_EC = 0xC000;
				sb1->SecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
				delete[] v6;
			}
		}
	}

	unsigned int v20{}, v21{};

	if (EncodingType == Constants::Enums::AudioEncoding::ATRAC3) {
		v21 = (0x30000 - sb1->field_EC) >> 1;
	}
	else {
		v21 = (0x30000 - sb1->field_EC) / (2 * (unsigned char)sb1->FileHeader[44]);
	}
	v20 = sb1->FileHeader[42];
	sb1->field_D8 -= v21 / v20;

		while (sb1->field_48) {
			Sleep(1);
			sb1->field_D4 += 1;
			if (sb1->SecondaryBuffer->GetCurrentPosition(&v59, &v60) == DS_OK) {
				int v24 = v59 - sb1->field_EC;
				int v25 = v60 - sb1->field_EC;
				if (v24 < 0)
					v24 += 0x30000 * ((-1 - v24) / 0x30000u + 1);
				if (v25 < 0)
					v25 += 0x30000 * ((-1 - v25) / 0x30000u + 1);
				if (v24 > v25) {
					int v26 = v59 - 0xC000;
					while (v26 < 0) {
						v26 += 0x30000;
					}
					sb1->field_EC = (v26 / 0xC000) * 0xC000;
					v24 = 0xC000;
				}
				int v28 = v24 / 0xC000;
				if (v28 > 0) {
					if (v28 > 2) {
						v28 = 2;
					}
					v57 = 0xC000 * v28;
					v29 = new char[2 * (v57 >> 1)];
					if (!v29) {
						sb1->field_48 = 0;
						v56 |= 1;
					}
					else {
						if (EncodingType == Constants::Enums::AudioEncoding::Encoding0) {
							v58 = v57 / (2 * (unsigned char)sb1->FileHeader[44]);
							sb1->field_D8 += v58 / (unsigned char)sb1->FileHeader[42];
							v33 = v58 * (((unsigned char)sb1->FileHeader[44] >> 1) + 1);
							v58 = v33;
						}
						else if (EncodingType == Constants::Enums::AudioEncoding::ATRAC3) {
							v58 = v57 / ((unsigned char)sb1->FileHeader[42] * ((unsigned char)sb1->FileHeader[43] >> 3));
							sb1->field_D8 += v58;
							v58 = v58 / 1024 + 8;
							v58 *= sb1->field_AC;
							v33 = v58;
						}
						else {
							v58 = v57 / ((unsigned char)sb1->FileHeader[42] * ((unsigned char)sb1->FileHeader[43] >> 3));
							sb1->field_D8 += v58;
							v33 = v57;
						}

						v34 = new char[v33];
						if (v34) {
							if (v63)
								++v63;
							sb1->PrepareSoundData(v34, v29, v57);
							delete[] v34;
							result = sb1->SecondaryBuffer->Lock(sb1->field_EC, v57, (LPVOID*)&v62, &v59, (LPVOID*)&v61, &v60, 0);
							if (result != DS_OK) {
								sb1->field_48 = 0;
								switch (result) {
								case DSERR_PRIOLEVELNEEDED:
									v56 |= 4;
									break;
								case DSERR_INVALIDPARAM:
									v56 |= 0x20;
									break;
								case DSERR_INVALIDCALL:
									v56 |= 0x10;
									break;
								case DSERR_BUFFERLOST:
									v56 |= 8;
									break;
								default:
									v56 |= 0x40;
									break;
								}
							}
							else {
								memcpy(v62, v29, v59);
								if (v61) {
									char* v39 = v29 + v59;
									int v40 = 4 * (v60 >> 2);
									memcpy(v61, v39, v40);
									memcpy(v61 + v40, v39 + v40, (char)v60 & 3);
								}

								sb1->SecondaryBuffer->Unlock(v62, v59, v61, v60);
								sb1->field_EC = (v57 + sb1->field_EC) % 0x30000;
							}
						}
						else
							v56 |= 2;

						delete[] v29;
					}
				}

				v48 = sb1->field_D8;
				v49 = *(int*)(sb1->FileHeader + 20);
				if (v48 >= v49) {
					int v50 = *(int*)(sb1->FileHeader + 24);
					if (v50 >= 0)
						sb1->field_D8 += v50 - v49;
					else {
						if (EncodingType == Constants::Enums::AudioEncoding::ATRAC3) {
							v58 = 0x24000;
							unsigned char v51 = (unsigned char)sb1->FileHeader[43];
							v58 = 0x24000 / ((unsigned char)sb1->FileHeader[42] * (v51 >> 3));
							v58 += *(int*)(sb1->FileHeader + 20);
							if (sb1->field_D8 >= v58)
								break;
						} else
							break;
					}
				}
			}
		}
	
	if (sb1->field_98)
		throw "NOT IMPLEMENTED";
	sb1->SecondaryBuffer->Stop();
	sb1->field_C0 = 0;
	sb1->field_C4 = -1;
	sb1->FileData = nullptr;
	sb1->field_88.Clean();
	SoundController::NotSure(0, sb1->field_C4);
	sb1->field_48 = 0;
	if (v56)
		v56 |= 2;
	return v56;
}

FFXI::SoundController::SoundController()
{
	this->field_0 = 1;
	this->field_8 = 0;
	this->field_4 = 0;
	InitializeCriticalSection(&SoundController::cs);

	SoundController::AudioStartPos = 0.0;
	SoundController::SoundOOFFlag = 0;
	SoundController::SomeValue = -1;

	SoundController::SoundBuff1Bin = new SoundBuff1[SoundBuff1BinSize];
	SoundController::SoundBuff2Bin = new SoundBuff2[SoundBuff2BinSize];

	SoundController::CommandQueue = new unsigned int[SoundController::CommandQueueSize];
	SoundController::AudioSpotBackups = new int[SoundController::AudioPosBackupSize];

	SoundController::SoundEffectCache = new CachedSoundEffect[SoundController::SoundEffectCacheSize];

	//Each string has length 0x108
	int music_dir_size = 0x108 * Constants::Values::INDEX_SOUND_MAX;

	SoundController::SoundEffectDirs = new char[music_dir_size];
	SoundController::MusicDirs = new char[music_dir_size];

	//TODO

	if (SoundBuff1Bin &&
		SoundBuff2Bin &&
		CommandQueue &&
		AudioSpotBackups &&
		SoundEffectCache &&
		SoundEffectDirs &&
		MusicDirs) {
		memset(SoundBuff1Bin, 0, SoundBuff1BinSize * sizeof(SoundBuff1));
		memset(SoundBuff2Bin, 0, SoundBuff2BinSize * sizeof(SoundBuff2));
		SoundBuff2Bin[0].field_94 = -1;
		SoundBuff2Bin[1].field_94 = -1;

		memset(CommandQueue, 0, SoundController::CommandQueueSize);
		CommandQueueWriteCursor = CommandQueue;
		CommandQueueReadCursor = CommandQueue;
		
		memset(AudioSpotBackups, 0, sizeof(AudioSpotBackups[0]) * AudioPosBackupSize);
		memset(SoundEffectCache, 0, sizeof(SoundEffectCache[0]) * SoundEffectCacheSize);
		memset(SoundEffectDirs, 0, music_dir_size);
		memset(MusicDirs, 0, music_dir_size);
	}
	else {
		this->field_0 = 0;
		if (SoundBuff1Bin) {
			delete[] SoundBuff1Bin;
			SoundBuff1Bin = nullptr;
		}
		if (SoundBuff2Bin) {
			delete[] SoundBuff2Bin;
			SoundBuff2Bin = nullptr;
		}
		if (CommandQueue) {
			delete[] CommandQueue;
			CommandQueue = nullptr;
		}

		if (AudioSpotBackups) {
			delete[] AudioSpotBackups;
			AudioSpotBackups = nullptr;
		}

		if (SoundEffectCache) {
			delete[] SoundEffectCache;
			SoundEffectCache = nullptr;
		}

		if (MusicDirs) {
			delete[] MusicDirs;
			MusicDirs = nullptr;
		}

		if (SoundEffectDirs) {
			delete[] SoundEffectDirs;
			SoundEffectDirs = nullptr;
		}

		//TODO
	}
}

FFXI::SoundController::~SoundController()
{
	if (SoundBuff2Bin && SoundBuff2Bin[0].SecondaryBuffer) {
		SoundBuff2* sb = &SoundBuff2Bin[0];
		sb->field_94 = -1;
		if (sb->Handle) {
			while (WaitForSingleObject(sb->Handle, 0))
				Sleep(1);
			CloseHandle(sb->Handle);
			sb->Handle = nullptr;
		}

		sb->SecondaryBuffer->Stop();

		if (sb->SecondaryBuffer) {
			sb->SecondaryBuffer->Release();
			sb->SecondaryBuffer = nullptr;
		}
		if (sb->PrimaryBuffer) {
			sb->PrimaryBuffer->Release();
			sb->PrimaryBuffer = nullptr;
		}
	}

	if (SoundBuff1Bin) {
		SoundController::SomeClean(-1, 0);
		for (int i = 0; i < SoundBuff1BinSize; ++i) {
			SoundBuff1* sb = SoundBuff1Bin + i;
			if (sb->Handle) {
				while (WaitForSingleObject(sb->Handle, 0))
					Sleep(1);
				sb->Handle = nullptr;
			}

			if (sb->SecondaryBuffer) {
				sb->SecondaryBuffer->Stop();
				if (sb->SecondaryBuffer) {
					sb->SecondaryBuffer->Release();
					sb->SecondaryBuffer = nullptr;
				}
			}

			if (sb->PrimaryBuffer) {
				sb->PrimaryBuffer->Release();
				sb->PrimaryBuffer = nullptr;
			}
		}

		delete[] SoundBuff1Bin;
	}
	if (SoundBuff2Bin)
		delete[] SoundBuff2Bin;

	if (CommandQueue)
		delete[] CommandQueue;

	if (AudioSpotBackups)
		delete[] AudioSpotBackups;

	if (SoundEffectCache) {
		for (int i = 0; i < SoundEffectCacheSize; ++i) {
			if (SoundEffectCache[i].Data) {
				delete[] SoundEffectCache[i].Data;
				SoundEffectCache[i].Data = nullptr;
			}
		}

		delete[] SoundEffectCache;
	}

	if (MusicDirs)
		delete[] MusicDirs;

	if (SoundEffectDirs)
		delete[] SoundEffectDirs;

	//sub //TODO
	DeleteCriticalSection(&SoundController::cs);
}

void FFXI::SoundController::SomeClean(int a1, int a2)
{
	
	SoundBuff2Bin[SoundBuff2BinSize - 1].field_B4 = 0;
	if (a1) {
		if (SoundBuff1Bin) {
			for (int i = 0; i < SoundBuff1BinSize; ++i) {
				SoundBuff1* sb = SoundBuff1Bin + i;
				if (sb->field_C0 &&
					(a1 < 0 || sb->field_C0 == a1))
					sb->field_48 = 0;
			}
		}
	}
	else {
		if (SoundBuff1Bin) {
			for (int i = 0; i < SoundBuff1BinSize; ++i) {
				SoundBuff1* sb = SoundBuff1Bin + i;
				if (sb->field_C0 &&
					(a2 < 0 || sb->field_C4 == a2))
					sb->field_48 = 0;
			}
		}
	}
}

int FFXI::SoundController::GetBuff2Flag(int a1, int a2)
{
	int result = -1;
	if (a1) {
		if (a1 == 1) {
			int v4 = SoundBuff2Bin[0].field_94;
			if (v4 >= 0 && (a2 < 0 || v4 == a2))
				return SoundBuff2Bin[0].field_4C >> 12;
		}
		else if (a1 == 3) {
			int v3 = SoundBuff2Bin[0].field_94;
			if (v3 >= 0 && (a2 < 0 || v3 == a2))
				return SoundBuff2Bin[0].field_48;
		}
	}
	else {
		int v5 = SoundBuff2Bin[0].field_94;
		result = 0;
		if (v5 >= 0 && (a2 < 0 || v5 == a2))
			return 1;
	}
	return result;
}

void FFXI::SoundController::UpdateBin1()
{
	if (!SoundBuff1Bin) return;

	int v3{ 0 };

	for (int i = 0; i < SoundBuff1BinSize; ++i) {
		SoundBuff1* sb1 = SoundBuff1Bin + i;
		if (sb1->SecondaryBuffer) {
			v3 = 0;
			if (sb1->field_6C > 0) {
				sb1->field_6C -= 1;
				sb1->field_70 += sb1->field_74;
				if (sb1->field_6C < 1 && sb1->field_78 == 1)
					sb1->field_48 = 0;
				if ((sb1->field_D0 & 0xC8) == 0)
					sb1->field_CC = sb1->field_70 >> 12;
				v3 = 1;
			}
			if (sb1->field_7C > 0) {
				sb1->field_7C -= 1;
				++v3;
				sb1->field_80 += sb1->field_84;
			}
			if (v3) {
				int v12 = sb1->field_80 >> 12;
				int v11 = sb1->field_70 >> 12;
				SoundMng::GetPanAndVolume(&v12, &v11);
				SoundMng::SetBufferVolume(sb1->SecondaryBuffer, v11);
				SoundMng::SetBufferPan(sb1->SecondaryBuffer, v12);
			}
		}

		if (sb1->Handle) {
			if (!WaitForSingleObject(sb1->Handle, 0)) {
				CloseHandle(sb1->Handle);
				sb1->Handle = nullptr;
				if (sb1->SecondaryBuffer) {
					sb1->SecondaryBuffer->Stop();
					if (sb1->SecondaryBuffer) {
						sb1->SecondaryBuffer->Release();
						sb1->SecondaryBuffer = nullptr;
					}
					if (sb1->PrimaryBuffer) {
						sb1->PrimaryBuffer->Release();
						sb1->PrimaryBuffer = nullptr;
					}
				}
				sb1->field_C0 = 0;
			}
		}
	}
}

void FFXI::SoundController::UpdateBin2()
{
	if (!SoundBuff2Bin) return;

	SoundBuff2* sb2 = SoundBuff2Bin;
	if (sb2[0].field_94 < 0) goto LABEL_9;
	if (sb2[0].field_48 > 0) {
		sb2[0].field_48 -= 1;
		sb2[0].field_4C += sb2[0].field_50;
		if (sb2[0].field_48 < 1 && sb2[0].field_54 == 1)
			sb2[0].field_94 = -1;
		int v6 = (sb2[0].field_4C >> 12) * SoundMng::SomeAudioTable[sb2[0].field_B8];
		int v12 = 64;
		int v11 = v6 >> 12;
		SoundMng::GetPanAndVolume(&v12, &v11);
		if (sb2[0].SecondaryBuffer)
			SoundMng::SetBufferVolume(sb2[0].SecondaryBuffer, v11);
	}

	if (sb2[0].field_94 >= 0) return;

LABEL_9:
	if (!sb2[0].SecondaryBuffer) return;
	if (WaitForSingleObject(sb2[0].Handle, 0)) return;
	if (sb2[0].SecondaryBuffer) {
		sb2[0].SecondaryBuffer->Stop();
		if (sb2[0].SecondaryBuffer) {
			sb2[0].SecondaryBuffer->Release();
			sb2[0].SecondaryBuffer = nullptr;
		}
		if (sb2[0].PrimaryBuffer) {
			sb2[0].PrimaryBuffer->Release();
			sb2[0].PrimaryBuffer = nullptr;
		}
	}

	CloseHandle(sb2[0].Handle);
	sb2[0].Handle = nullptr;
}

void FFXI::SoundController::UpdateQueue()
{
	unsigned int* v0 = CommandQueueReadCursor;
	unsigned int* v1 = CommandQueueWriteCursor;
	unsigned int* v2{ nullptr };
	while (v1 != v0) {
		v2 = CommandQueue;
		if (v0[0] < CommandHandlerCount) {
			CommandHandlers[v0[0]](v0 + 1);
		}
		if (v0[0] == 7) {
			v0 += 72;
			if (v0 >= v2 + CommandQueueSize)
				v0 -= CommandQueueSize;
		}
		else {
			v0 += 8;
			if (v0 >= v2 + CommandQueueSize)
				v0 = v2;
		}
	}

	SoundController::CommandQueueReadCursor = v0;
}

SoundController::CachedSoundEffect* FFXI::SoundController::GetCachedSoundEffect(int a1)
{
	for (int i = 0; i < SoundEffectCacheSize; ++i) {
		SoundController::CachedSoundEffect* cse = SoundEffectCache + i;
		if (cse->ID == a1 && cse->Data)
			return cse;
	}

	return nullptr;
}

SoundController::CachedSoundEffect* MakeFreeSlot() 
{
	EnterCriticalSection(&SoundController::cs);

	SoundController::CachedSoundEffect* retval{ nullptr };
	int v1 = 0;
	for (int i = 0; i < SoundController::SoundEffectCacheSize; ++i) {
		SoundController::CachedSoundEffect* cse = SoundController::SoundEffectCache + i;
		if (cse->Data != nullptr && v1 < cse->LoadOrder) {
			v1 = i;
			retval = cse;
		}
	}

	if (retval != nullptr) {
		if (retval->Data != nullptr && retval->LoadOrder > 0) {
			for (int i = 0; i < SoundController::SoundBuff1BinSize; ++i) {
				SoundBuff1* sb1 = SoundController::SoundBuff1Bin + i;
				if (sb1->FileData == retval->Data) {
					retval = nullptr;
					break;
				}
			}

			if (retval != nullptr) {
				FFXI::SoundController::NotSure2(retval->LoadOrder);
				retval->ID = -1;
				retval->LoadOrder = 0;
				delete[] retval->Data;
				retval->Data = nullptr;
			}
		}
	}

	LeaveCriticalSection(&SoundController::cs);
	return retval;
}
SoundController::CachedSoundEffect* FFXI::SoundController::CreateCachedSoundEffect(int a1)
{
	char* SeDirs = SoundEffectDirs + 4;
	if (!*SeDirs) return nullptr;

	SoundController::CachedSoundEffect* slot{ nullptr };
	int i = 0;
	for (i = 0; i < SoundEffectCacheSize; ++i) {
		slot = SoundEffectCache + i;
		if (slot->Data == nullptr)
			break;
	}

	if (i >= SoundEffectCacheSize) {
		slot = MakeFreeSlot();
	}

	if (slot == nullptr)
		return nullptr;

	char* v5 = new char[0x104];
	int* v6 = new int[0xC];
	char* v22[Constants::Values::INDEX_SOUND_MAX];

	if (v5 && v6) {
		FILE* fp{ nullptr };
		for (i = 0; i < Constants::Values::INDEX_SOUND_MAX; ++i)
			v22[i] = new char[0x104];

		for (i = 0; i < Constants::Values::INDEX_SOUND_MAX; ++i) {
			if (v22[i]) {
				if (*((int*)SeDirs - 1)) {
					sprintf_s(v22[i], 0x104, "%s\\se%6.6u.spw", SeDirs, a1);
				}
				else {
					unsigned int v10 = (((unsigned long long)a1 * 0x10624DD3LLu) >> 32LLu) >> 6LLu;
					sprintf_s(v22[i], 0x104, "%s\\se%3.3u\\se%6.6u.spw", SeDirs, (v10 >> 31) + v10, a1);
				}
			}

			fopen_s(&fp, v22[i], "rb");
			if (fp)
				break;

			SeDirs += 0x108;
		}

		if (i < Constants::Values::INDEX_SOUND_MAX)
			strcpy_s(v5, 0x104, v22[i]);
		
		for (i = 0; i < Constants::Values::INDEX_SOUND_MAX; ++i) {
			if (v22[i])
				delete[] v22[i];
		}

		if (fp) {
			fread(v6, 1, 0x30, fp);
			if (!strcmp((char*)v6, "SeWave")) {
				unsigned int fileSize = 4 * ((unsigned int)(v6[2] + 3) >> 2);
				slot->Data = new char[fileSize];
				if (slot->Data) {
					fseek(fp, 0, SEEK_SET);
					fread(slot->Data, 1, v6[2], fp);
					UpdateCacheLoadOrders();
					slot->LoadOrder = 1;
					slot->ID = a1;
				}
				else
					slot = nullptr;
			}
			else
				slot = nullptr;

			fclose(fp);
			if (v5)
				delete[] v5;
			if (v6)
				delete[] v6;
			return slot;
		}
	}

	if (v5)
		delete[] v5;

	if (v6)
		delete[] v6;

	return nullptr;;
}

void FFXI::SoundController::UpdateCacheLoadOrders()
{
	for (int i = 0; i < SoundEffectCacheSize; ++i) {
		SoundController::CachedSoundEffect* cse = SoundEffectCache + i;
		if (cse->Data) {
			if (cse->LoadOrder > 0)
				cse->LoadOrder += 1;
		}
	}
}

void FFXI::SoundController::NotSure(int a1, int a2)
{
	EnterCriticalSection(&SoundController::cs);
	if (a1) {
		SoundController::CachedSoundEffect* cse = GetCachedSoundEffect(a2);
		if (cse && cse->LoadOrder > 0) {
			NotSure2(cse->LoadOrder);
			cse->LoadOrder = 0;
		}
	}
	else {
		SoundBuff1* sb1 = SoundBuff1Bin;
		while (!sb1->SecondaryBuffer || sb1->field_C4 != a2) {
			if (++sb1 >= SoundBuff1Bin + SoundBuff1BinSize) {
				SoundController::CachedSoundEffect* cse = GetCachedSoundEffect(a2);
				if (cse && cse->LoadOrder == 0) {
					UpdateCacheLoadOrders();
					cse->LoadOrder = 1;
				}
				break;
			}
		}
	}
	LeaveCriticalSection(&SoundController::cs);
}

void FFXI::SoundController::NotSure2(int a1)
{
	if (a1 == 0) {
		return;
	}

	for (int i = 0; i < SoundEffectCacheSize; ++i) {
		SoundController::CachedSoundEffect* cse = SoundEffectCache + i;
		if (cse->Data != nullptr) {
			if (cse->LoadOrder > a1)
				cse->LoadOrder -= 1;
		}
	}
}

bool PlaySeWavHelper(int a1, int a2, int a3) 
{
	bool result = false;
	int v7{};
	const int v5 = SoundMng::SomeAudioTable[a2 & 0x7F];

	for (int i = 0; i < SoundController::SoundBuff1BinSize; ++i) {
		SoundBuff1* sb1 = SoundController::SoundBuff1Bin + i;

		if (sb1->field_C0 && (sb1->field_D0 & 0x40) != 0) {
			if (sb1->field_C4 == a1) {
				result = true;
				sb1->field_78 = 0;
				sb1->field_C0 = a3;
				v7 = v5 << 12;
			}
			else {
				sb1->field_78 = 1;
				v7 = SoundMng::SomeAudioTable[1] << 12;
			}

			sb1->field_6C = 150;
			unsigned int v9 = (unsigned int)((unsigned long long)(0x1B4E81B5i64 * (v7 - sb1->field_70)) >> 32) >> 4;
			sb1->field_74 = (v9 >> 31) + v9;
		}
	}

	return result;
}

void FFXI::SoundController::PlaySeWav(char* a1, int a2, int a3, int a4)
{
	EnterCriticalSection(&SoundController::cs);
	SoundBuff2Bin[2].field_B4 = 0;
	if ((a1[41] & 0x20) != 0 && PlaySeWavHelper(*((int*)a1 + 4), a4, a2)) {
		LeaveCriticalSection(&SoundController::cs);
		return;
	}
	
	int v6 = 0;
	int v7 = 0;
	if (a1[41] < 0) {
		v6 = 0;
		v7 = SoundMng::SomeAudioTable[(unsigned char)a1[40]];
	}
	else if ((a1[41] & 0x20) != 0) {
		v6 = 0;
		v7 = 20000;
	}
	else if ((a1[41] & 8) != 0) {
		v6 = (unsigned char)a1[41];
		v7 = 0x4E20;
	}
	else {
		v6 = (unsigned char)a1[41];
		v7 = SoundMng::SomeAudioTable[a4];
	}

	LeaveCriticalSection(&SoundController::cs);

	SoundBuff1* sb1 = GetSB1(1, v7, v6);
	unsigned char* byteData = (unsigned char*)a1;
	if (sb1) {
		if (sb1->SecondaryBuffer) {
			if (sb1->Handle) {
				sb1->field_48 = 0;
				while (WaitForSingleObject(sb1->Handle, 0))
					Sleep(1);
				CloseHandle(sb1->Handle);
				sb1->Handle = nullptr;
			}

			sb1->SecondaryBuffer->Stop();
			if (sb1->SecondaryBuffer) {
				sb1->SecondaryBuffer->Release();
				sb1->SecondaryBuffer = nullptr;
			}

			if (sb1->PrimaryBuffer) {
				sb1->PrimaryBuffer->Release();
				sb1->PrimaryBuffer = nullptr;
			}
		}
		sb1->field_D4 = 0;
		if (a1[41] < 0) {
			sb1->field_D0 = 0;
			sb1->field_CC = SoundMng::SomeAudioTable[byteData[40]];
		}
		else if ((a1[41] & 0x20) != 0) {
			sb1->field_D0 = 64;
			sb1->field_CC = 20000;
		}
		else if ((a1[41] & 8) != 0) {
			sb1->field_D0 = a1[41];
			sb1->field_CC = 20000;
		}
		else {
			sb1->field_D0 = a1[41];
			sb1->field_CC = SoundMng::SomeAudioTable[a4];
		}

		if ((a1[41] & 0x20) != 0) {
			int v15 = (SoundMng::SomeAudioTable[a4] - SoundMng::SomeAudioTable[1]) << 12;
			v15 |= 0x800;
			sb1->field_74 = v15;
			sb1->field_70 = SoundMng::SomeAudioTable[1] << 12;
			sb1->field_6C = 150;
			sb1->field_74 = v15 / 150;
		}
		else {
			sb1->field_6C = 0;
			sb1->field_70 = SoundMng::SomeAudioTable[a4] << 12;
		}
		sb1->field_78 = 0;
		sb1->field_80 = a3 << 12;
		sb1->field_7C = 0;
		sb1->field_C8 = 0;

		LPWAVEFORMATEX fmt = new WAVEFORMATEX();
		LPDSBUFFERDESC desc = new DSBUFFERDESC();
		
		if (fmt) {
			if (desc) {
				fmt->wFormatTag = 1;
				fmt->nChannels = byteData[42];
				fmt->wBitsPerSample = byteData[43];
				fmt->nBlockAlign = byteData[42] * byteData[43] / 8;
				int v20 = (*((int*)a1 + 7) + *((int*)a1 + 8)) & 0x7FFFFFFF;
				int v21 = v20 * fmt->nBlockAlign;
				fmt->nSamplesPerSec = v20;
				fmt->nAvgBytesPerSec = v21;
				fmt->cbSize = 0;
				memset(desc, 0, sizeof(DSBUFFERDESC));
				desc->dwSize = 36;
				desc->lpwfxFormat = fmt;
				desc->dwFlags = SoundController::SoundOOFFlag | 0x500C0;
				desc->dwBufferBytes = 0x30000;
				if (SoundMng::DirectSound8->CreateSoundBuffer(desc, &sb1->PrimaryBuffer, NULL) >= DS_OK) {
					if (sb1->PrimaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&sb1->SecondaryBuffer) < DS_OK) {
						if (sb1->PrimaryBuffer) {
							sb1->PrimaryBuffer->Release();
							sb1->PrimaryBuffer = nullptr;
						}
					}
				}
			}
			delete fmt;
		}
		if (desc)
			delete desc;
		if (sb1->PrimaryBuffer) {
			sb1->field_C0 = a2;
			sb1->FileData = a1;
			sb1->field_C4 = *((int*)a1 + 4);
			sb1->field_B8 = 0;
			memcpy(sb1->FileHeader, a1, sizeof(sb1->FileHeader));
			sb1->field_D8 = 0;
			sb1->field_F0 = 0;
			sb1->field_48 = 1;
			sb1->field_44 = sb1;
			sb1->Handle = CreateThread(NULL, 0, SeWaveProc, sb1, 0, &sb1->ThreadID);
			if (sb1->Handle) {
				SetThreadPriority(sb1->Handle, THREAD_PRIORITY_ABOVE_NORMAL);
			}
			else {
				sb1->field_C0 = 0;
				sb1->field_C4 = -1;
			}
		}
	}
}

bool GetSB1Helper1(int a1, int a2) {
	SoundBuff1* sb1 = SoundController::SoundBuff1Bin;
	SoundBuff1* end = SoundController::SoundBuff1Bin + SoundController::SoundBuff1BinSize;
	do {
		if ((sb1->field_C0 || sb1->Handle) && sb1->field_CC <= a1 && (a2 & sb1->field_D0) != 0) {
			sb1->field_48 = 0;
			sb1->field_6C = 0;
			sb1->field_70 = 0;
		}
		++sb1;
	} while (sb1 < end);

	return true;
}

int GetSB1Helper2(int a1) {
	SoundBuff1* sb1 = SoundController::SoundBuff1Bin;
	SoundBuff1* end = SoundController::SoundBuff1Bin + SoundController::SoundBuff1BinSize;

	int v2{}, v3{};
	do {
		if (sb1->field_C0 && sb1->field_48 && !sb1->field_D0 && sb1->field_CC <= a1 && sb1->field_D4 >= v2)
		{
			v2 = sb1->field_D4;
			v3 = sb1->field_C0;
		}
		++sb1;
	} while (sb1 < end);

	if (!v3) return 0;

	sb1 = SoundController::SoundBuff1Bin;
	while (sb1 < end) {
		if (sb1->field_C0) {
			if (sb1->field_48 && sb1->field_C0 == v3) {
				sb1->field_48 = 0;
				sb1->field_6C = 0;
				sb1->field_70 = 0;
				break;
			}
		}
	}

	if (sb1 != end)
		return v3;

	return 0;
}

SoundBuff1* FFXI::SoundController::GetSB1(int a1, int a2, int a3)
{
	if (a3 && !GetSB1Helper1(a2, a3)) return nullptr;

	SoundBuff1* start{ nullptr };
	SoundBuff1* end{ nullptr };
	while (true) {
		start = SoundBuff1Bin;
		end = SoundBuff1Bin + SoundBuff1BinSize;
		while (--end >= start && end->field_C0 && end->field_48 || a1 != 1) {
			if (end < start)
				goto LABEL_11;
		}
		if (end >= start) return end;
		LABEL_11:
		if ((a3 & 0x80) == 0) {
			if (GetSB1Helper2(a2))
				continue;
		}
		break;
	}

	if (end >= start)
		return end;

	return nullptr;
}

int FFXI::SoundController::GetBuff1Flag(int a1, int a2, int a3)
{
	SoundBuff1* sb1 = SoundBuff1Bin;
	if (!sb1) return -1;

	int i = 0, handlecount = 0;
	switch (a1) {
	case 0:
		if (a2) {
			while ((a2 >= 0 || sb1->field_C0 <= 0) && sb1->field_C0 != a2) {
				if (++i >= SoundBuff1BinSize) return 0;
				sb1 = SoundBuff1Bin + i;
			}
		}
		else {
			while (sb1->field_C0 <= 0 || a3 >= 0 && sb1->field_C4 != a3) {
				if (++i >= SoundBuff1BinSize) return 0;
				sb1 = SoundBuff1Bin + i;
			}
		}
		return 1;
	case 1:
		if (a2 > 0) {
			while (sb1->field_C0 != a2) {
				if (++i >= SoundBuff1BinSize) return -1;
				sb1 = SoundBuff1Bin + i;
			}
			return sb1->field_70 >> 12;
		}
		return -1;
	case 2:
		if (a2 > 0) {
			while (sb1->field_C0 != a2) {
				if (++i >= SoundBuff1BinSize) return -1;
				sb1 = SoundBuff1Bin + i;
			}
			return sb1->field_80 >> 12;
		}
		return -1;
	case 3:
		if (a2 > 0) {
			while (sb1->field_C0 != a2) {
				if (++i >= SoundBuff1BinSize) return -1;
				sb1 = SoundBuff1Bin + i;
			}
			return sb1->field_6C;
		}
		return -1;
	case 4:
		if (a2 > 0) {
			while (sb1->field_C0 != a2) {
				if (++i >= SoundBuff1BinSize) return -1;
				sb1 = SoundBuff1Bin + i;
			}
			return sb1->field_7C;
		}
		return -1;
	case 5:
		do {
			if (sb1->Handle)
				++handlecount;
			++i;
			sb1 = SoundBuff1Bin + i;
		} while (i < SoundBuff1BinSize);
		return handlecount;
	default:
		return -1;
	}
	return -1;
}
