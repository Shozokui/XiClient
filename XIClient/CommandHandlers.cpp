#include "CommandHandlers.h"
#include "SoundController.h"
#include "WeirdSoundThing.h"
#include "FuncSigs.h"
#include "SoundBuff1.h"
#include "SoundBuff2.h"
#include "SoundMng.h"
#include "Enums.h"
#include <string>

using namespace FFXI;
using namespace FFXI::Constants;

const unsigned int FFXI::CommandHandlerCount = 14;

int MakeTag(const char* tag) { return *(int*)(tag); }
const int RiffTag = MakeTag("RIFF");
const int WaveTag = MakeTag("WAVE");
const int DataTag = MakeTag("data");
const int CueTag = MakeTag("cue ");
const int FmtTag = MakeTag("fmt ");

extern void Type0Decode(unsigned char a1, unsigned char a2, unsigned char* a3, short* a4, int* a5, unsigned int a6);

void Handle0(unsigned int* a1) {
	SoundController::SomeClean(a1[0], a1[1]);
}
void Handle1(unsigned int* a1) {
	char* data = (char*)a1[0];
	int updateCounter = a1[1];
	int unknown = a1[2];
	int volume = a1[3];

	throw "NOT IMPLEMENTED";
}

void Handle2(unsigned int* a1) {
	int fileID = a1[0];
	int updateCounter = a1[1];
	int pan = a1[2];
	int volume = a1[3];

	SoundController::CachedSoundEffect* se = SoundController::GetCachedSoundEffect(fileID);
	if (!se)
		se = SoundController::CreateCachedSoundEffect(fileID);

	if (se) {
		SoundController::NotSure(1, fileID);
		SoundController::PlaySeWav(se->Data, updateCounter, pan, volume);
	}
}
void Handle3(unsigned int* a1) { 
	int fileID = a1[0];
	int updateCounter = a1[1];
	int pan = a1[2];
	int vol = a1[3];
	EnterCriticalSection(&SoundController::cs);
	int v5 = pan & 0x7F;
	int v7 = 50 * vol / 60;
	if (fileID != 0) {
		for (int i = 0; i < SoundController::SoundBuff1BinSize; ++i) {
			SoundBuff1* sb1 = SoundController::SoundBuff1Bin + i;
			if (fileID < 0 || sb1->field_C0 == fileID) {
				if (v7 != 0) {
					sb1->field_7C = v7;
					int v8 = (v5 - (sb1->field_80 >> 12)) << 12;
					v8 |= 0x800;
					sb1->field_84 = v8 / v7;
				}
				else {
					sb1->field_80 = v5 << 12;
					sb1->field_84 = 0;
					sb1->field_7C = 1;
				}
			}
		}
	}
	else if (updateCounter != 0) {
		for (int i = 0; i < SoundController::SoundBuff1BinSize; ++i) {
			SoundBuff1* sb1 = SoundController::SoundBuff1Bin + i;
			if (updateCounter < 0 || sb1->field_C4 == updateCounter) {
				if (v7 != 0) {
					sb1->field_7C = v7;
					int v8 = (v5 - (sb1->field_80 >> 12)) << 12;
					v8 |= 0x800;
					sb1->field_84 = v8 / v7;
				}
				else {
					sb1->field_80 = v5 << 12;
					sb1->field_84 = 0;
					sb1->field_7C = 1;
				}
			}
		}
	}
	LeaveCriticalSection(&SoundController::cs);
}

void Handle4(unsigned int* a1) {
	int fileID = a1[0];
	int updateCounter = a1[1];
	int pan = a1[2];
	int vol = a1[3];
	int v5 = pan & 0x7F;
	int v7 = 50 * vol / 60;
	if (fileID != 0) {
		for (int i = 0; i < SoundController::SoundBuff1BinSize; ++i) {
			SoundBuff1* sb1 = SoundController::SoundBuff1Bin + i;
			if (fileID < 0 || sb1->field_C0 == fileID) {
				if (v7 != 0) {
					sb1->field_6C = v7;
					int v8 = (SoundMng::SomeAudioTable[v5] << 12) - sb1->field_70;
					sb1->field_74 = (v8 + 2048) / v7;
				}
				else {
					sb1->field_70 = SoundMng::SomeAudioTable[v5] << 12;
					sb1->field_74 = 0;
					sb1->field_6C = 1;
					if ((sb1->field_D0 & 0xC8) == 0) {
						sb1->field_CC = sb1->field_70 >> 12;
					}
				}
			}
		}
	}
	else if (updateCounter != 0) {
		for (int i = 0; i < SoundController::SoundBuff1BinSize; ++i) {
			SoundBuff1* sb1 = SoundController::SoundBuff1Bin + i;
			if (updateCounter < 0 || sb1->field_C4 == updateCounter) {
				if (v7 != 0) {
					sb1->field_6C = v7;
					int v8 = (SoundMng::SomeAudioTable[v5] - (sb1->field_70 >> 12)) << 12;
					v8 |= 0x800;
					sb1->field_74 = v8 / v7;
				}
				else {
					sb1->field_70 = SoundMng::SomeAudioTable[v5] << 12;
					sb1->field_74 = 0;
					sb1->field_6C = 1;
					if ((sb1->field_D0 & 0xC8) == 0) {
						sb1->field_CC = sb1->field_70 >> 12;
					}
				}
			}
		}
	}
}

void Handle5(unsigned int* a1) { 
	int fileID = a1[0];
	int updateCounter = a1[1];
	int pan = a1[2];
	int v4 = 50 * pan / 60;

	if (fileID != 0) {
		for (int i = 0; i < SoundController::SoundBuff1BinSize; ++i) {
			SoundBuff1* sb1 = SoundController::SoundBuff1Bin + i;
			if (fileID < 0 || sb1->field_C0 == fileID) {
				if (v4 != 0) {
					int v5 = (SoundMng::SomeAudioTable[1] << 12) - sb1->field_70;
					sb1->field_6C = v4;
					sb1->field_74 = (v5 + 2048) / v4;
				}
				else {
					sb1->field_70 = SoundMng::SomeAudioTable[0] << 12;
					sb1->field_74 = 0;
					sb1->field_6C = 1;
					if ((sb1->field_D0 & 0xC8) == 0) {
						sb1->field_CC = sb1->field_70 >> 12;
					}
				}
				sb1->field_78 = 1;
			}
		}
	}
	else if (updateCounter != 0) {
		for (int i = 0; i < SoundController::SoundBuff1BinSize; ++i) {
			SoundBuff1* sb1 = SoundController::SoundBuff1Bin + i;
			if (updateCounter < 0 || sb1->field_C4 == updateCounter) {
				if (v4 != 0) {
					int v5 = SoundMng::SomeAudioTable[1] - (sb1->field_70 >> 12);
					v5 <<= 12;
					v5 |= 0x800;
					sb1->field_6C = v4;
					sb1->field_74 = v5 / v4;
				}
				else {
					sb1->field_70 = SoundMng::SomeAudioTable[0] << 12;
					sb1->field_74 = 0;
					sb1->field_6C = 1;
					if ((sb1->field_D0 & 0xC8) == 0) {
						sb1->field_CC = sb1->field_70 >> 12;
					}
				}
				sb1->field_78 = 1;
			}
		}
	}

	EnterCriticalSection(&SoundController::cs);

	LeaveCriticalSection(&SoundController::cs);
}

void Handle6(unsigned int* a1) { 
	SoundBuff2* buff = SoundController::SoundBuff2Bin;
	int v2 = (int)a1[0];
	for (int i = 0; i < SoundController::SoundBuff2BinSize - 1; ++i) {
		if (v2 < 0 || buff->field_94 == v2) {
			if (buff->field_94 >= 0)
				buff->field_94 = -1;
		}
		buff += 1;
	}
}

void ParseWavHeader(int* a1, char* a2) {
	FILE* file{ nullptr };
	int intone{ 0 }, inttwo{ 0 }, intthree{ 0 };
	int v22{ 0 };
	fopen_s(&file, a2, "rb");
	if (!file) return;

	while (true) {
		fread(&intone, 1, 4, file);
		if (intone <= RiffTag) break;
		if (intone != DataTag) goto LABEL_16;
		fread(&inttwo, 1, 4, file);
		a1[6] = inttwo;
		a1[7] = ftell(file);
		fseek(file, inttwo, SEEK_CUR);
	LABEL_19:
		if (intthree) {
			intthree += -8 - inttwo;
			if (intthree > 0)
				continue;
		}
		goto LABEL_23;
	}
	if (intone != RiffTag) {
		if (intone == CueTag) {
			throw "NOT IMPLEMENTED CUE";
		}
		else {
			if (intone != FmtTag)
				goto LABEL_16;
			
			fread(&inttwo, 1, 4, file);
			if (inttwo < 16) goto LABEL_22;
			BYTE* v3 = new BYTE[inttwo];
			if (!v3) goto LABEL_22;
			fread(v3, 1, inttwo, file);
			int v5 = v3[3] << 8;
			a1[1] = v5 + v3[2];
			int v6 = v3[15] << 8;
			a1[2] = v6 + v3[14];
			int v7 = v3[7] << 8;
			int v8 =(v7 + v3[6]) << 8;
			int v9 = (v8 + v3[5]) << 8;
			a1[3] = v9 + v3[4];
			delete[] v3;
		}
		goto LABEL_19;
	}
	fread(&intthree, 1, 4, file);
	intthree += 8;
	inttwo = 4;
	fread(&intone, 1, 4, file);
	if (intone == WaveTag) {
		++v22;
		goto LABEL_19;
	}
	LABEL_16:
	if (v22) {
		fread(&inttwo, 1, 4, file);
		fseek(file, inttwo, SEEK_CUR);
		goto LABEL_19;
	}
LABEL_22:
	intthree = 0;
	a1[7] = 0;
LABEL_23:
	fclose(file);
	if (a1[7] > 0) {
		int v16 = a1[5];
		a1[6] /= a1[1] * (a1[2] / 8);
		if (v16 < a1[4]) {
			a1[5] = -1;
			a1[4] = -1;
		}
		int v17 = a1[5];
		if (v17 > 0) {
			int v18 = a1[6];
			int v19 = v17 + 1;
			if (v18 >= v19) {
				if (v18 > v19)
					a1[6] = v19;
			}
			else
				a1[5] = v18 - 1;
		}
	}
}

void ParseAudioHeader(int* a1, char* a2, int a3) {
	a1[5] = -1;
	a1[4] = -1;
	a1[7] = -1;
	a1[6] = -1;
	int* v4{ nullptr };
	FILE* v5{ nullptr };
	if (a3) {
		v4 = new int[3];
		if (v4) {
			v4[0] = 0;
			v4[1] = 0;
			v4[2] = 0;
			fopen_s(&v5, a2, "rb");
			if (v5) {
				fread(v4, 1, 12, v5);
				fclose(v5);
			}

			if (v4[0] == RiffTag &&
				v4[2] == WaveTag) {
				*a1 = 0;
				ParseWavHeader(a1, a2);
				delete[] v4;
			}
			else {
				*a1 = 1;
				throw "NOT IMPLEMENTED";
				//sub //TODO
				delete[] v4;
			}
		}
	}
	else if (a2[0] == RiffTag &&
		a2[2] == WaveTag) {
		*a1 = 0;
		throw "NOT IMPLEMENETED";
		//sub //TODO
	}
	else {
		*a1 = 1;
		throw "NOT IMPLEMENETED";
		//sub //TODO
	}
}
void PrepareSoundData(SoundBuff2* sb2, char* EncodedBuffer, char* DecodedBuffer, int DecodedBufferSize) {
	int v55{ 0 }, v8{ 0 }, v9{ 0 }, BytesPerSample{ 0 }, v10{ 0 };
	FILE* file{ nullptr };
	AudioFileHeader* header = &sb2->FileHeader;
	SoundController::SomeValue = 10;

	if (header->EncodingFormat == Enums::AudioEncoding::Encoding0) {
		v55 = ((header->field_2F >> 1) + 1) * ((DecodedBufferSize >> 1) / header->field_2F);
		BytesPerSample = header->nChannels * ((header->field_2F >> 1) + 1);
	}
	else if (header->EncodingFormat == Enums::AudioEncoding::ATRAC3) {
		v55 = sb2->field_7C * ((((DecodedBufferSize >> 1) / header->nChannels) >> 10) + 4);
		v8 = header->FileSize;
		v9 = header->FileHeaderSize + sb2->field_7C * (sb2->field_88 / 1024);
		goto LABEL_12;
	}
	else {
		v55 = DecodedBufferSize;
		BytesPerSample = header->nChannels * (header->wBitsPerSample >> 3);
	}
		
	memset(DecodedBuffer, 0, v55);
	
	v10 = header->FileHeaderSize;
	v8 = v10 + BytesPerSample * header->SampleCount;
	v9 = v10 + BytesPerSample * sb2->field_B0;
LABEL_12:
	int count = v55;
	if (header->field_1C >= 0 || v9 < v8) {
		fopen_s(&file, sb2->FileName, "rb");
		if (file) {
			int v13{ 0 }, v16{ 0 };
			size_t v11{ 0 }, v12{ 0 };
			while (true) {
				v11 = count;
				if (v8 < count + v9)
					v11 = v8 - v9;
				fseek(file, v9, 0);
				v12 = fread(EncodedBuffer, 1u, v11, file);
				count -= v12;
				if (count <= 0 || (v13 = header->field_1C, v13 < 0)) {
					fclose(file);
					break;
				}
				if (header->EncodingFormat == Enums::AudioEncoding::Encoding0) {
					v16 = (header->field_2F >> 1) + 1;
					v9 = header->FileHeaderSize + v13 * header->nChannels * v16;
					continue;
				} 
				else if (header->EncodingFormat == Enums::AudioEncoding::ATRAC3) {
					v9 = header->FileHeaderSize + sb2->field_7C * (v13 / 1024 - 1);
				}
				else {
					v16 = header->wBitsPerSample >> 3;
					v9 = header->FileHeaderSize + v13 * header->nChannels * v16;
				}
			}
		}
	}

	SoundController::SomeValue = 11;
	if (header->EncodingFormat == Enums::AudioEncoding::Encoding0) {
		SoundController::SomeValue = 12;
		//FuncSigs::Type0Helper(header->nChannels, header->field_2F, EncodedBuffer, DecodedBuffer, &sb2->field_9C, DecodedBufferSize);
		Type0Decode(header->nChannels, header->field_2F, (unsigned char*)EncodedBuffer, (short*)DecodedBuffer, sb2->Type0DecodeContext, DecodedBufferSize);
	}
	else if (header->EncodingFormat == Enums::AudioEncoding::ATRAC3) {
		throw "NOT IMPLEMENTED";
	}
	else if (header->EncodingFormat == Enums::AudioEncoding::Encoding2) {
		memcpy(DecodedBuffer, EncodedBuffer, DecodedBufferSize);
		char* v39 = DecodedBuffer;
		do {
			throw "NOT SURE IF THIS IS CORRECT. CHECK IT";
			char temp = v39[1];
			v39[1] = v39[0];
			v39[0] = temp;
			v39 += 2;
		} while (v39 < &DecodedBuffer[2 * (DecodedBufferSize >> 1)]);
	}
	else {
		memcpy(DecodedBuffer, EncodedBuffer, DecodedBufferSize);
	}

	int v43{ 0 }, v44{ 0 }, v45{ 0 }, v46{ 0 }, v47{ 0 };

	if (header->EncodingFormat == Enums::AudioEncoding::Encoding0) {
		v43 = (header->field_2F >> 1) + 1;
		v44 = v55;
	}
	else if (header->EncodingFormat == Enums::AudioEncoding::ATRAC3) {
		v44 = DecodedBufferSize;
		v43 = header->wBitsPerSample >> 3;
	}
	else {
		v43 = header->wBitsPerSample >> 3;
		v44 = v55;
	}
	
	v45 = header->field_1C;
	v46 = v44 / (header->nChannels * v43) + sb2->field_B0;
	if (v45 >= 0) {
		v47 = header->SampleCount;
		while (v46 >= v47) {
			v46 += v45 - v47;
		}
	}
	sb2->field_B0 = v46;
	SoundController::SomeValue = 13;
}

DWORD WINAPI Handler7Thread(LPVOID param) {
	DWORD v50{ 0 }, v51{ 0 }, v54{ 0 }, v55{ 0 };
	SoundBuff2* sb2 = static_cast<SoundBuff2*>(param);
	char* v27{ nullptr }, *v32{ nullptr }, * v57{ nullptr }, * v56{ nullptr }, * v7{ nullptr }, * v8{ nullptr };
	HRESULT v10{};
	int v22{ 0 }, v23{ 0 }, v24{ 0 }, v53{ 0 };
	unsigned int v29{ 0 }, v31{ 0 }, v52{ 0 };
	SoundController::SomeValue = 0;
	sb2->field_68 = 0;
	sb2->field_58.~WeirdSoundThing();
	sb2->Type0DecodeContext[0] = 0;
	sb2->Type0DecodeContext[1] = 0;
	sb2->Type0DecodeContext[2] = 0;
	sb2->Type0DecodeContext[3] = 0;

	AudioFileHeader* header = &sb2->FileHeader;

	if (header->EncodingFormat == Enums::AudioEncoding::Encoding0) {
		int v6 = 0x60000 / header->field_2F * ((header->field_2F >> 1) + 1);
		v52 = v6;
	}
	else if (header->EncodingFormat == Enums::AudioEncoding::ATRAC3) {
		/*if (sb2->field_8[47] == 1) {
			sb2->field_7C = 304;
			sb2->field_84 = 105;
			goto LABEL_13;
		}
		if (sb2->field_8[47] != 2) {
			sb2->field_7C = 384;
			sb2->field_84 = 132;
		LABEL_13:
			sb2->field_80 = 1;
			goto LABEL_14;
		}
		sb2->field_7C = 192;
		sb2->field_80 = 2;
		sb2->field_84 = 66;
	LABEL_14:
		v52 = (((unsigned int)(0xC0000 / (unsigned char)sb2->field_8[46]) >> 11) + 8) * sb2->field_7C;

		throw "NOT IMPLEMENENTED";
		if (FuncSigs::WavObjInit) {
			if (FuncSigs::WavObjInit(&sb2->field_68) >= S_OK) {
				if (FuncSigs::WavCheckSystem) {
					if (FuncSigs::WavCheckSystem(sb2->field_68, sb2->field_80, sb2->field_84) >= S_OK) {
						sb2->field_6C = sb2->field_68[4];
						sb2->field_70 = sb2->field_68[5];
						sb2->field_74 = sb2->field_68[10];
						sb2->field_78 = sb2->field_68[11];
					}
					else
						v52 = 0;
				}
				else v52 = 0;
			}
			else {
				v52 = 0;
			}
		}
		else {
			v52 = 0;
		}

		v50 = intheader[9];
		sb2->field_8C = 0;*/

		throw "NOT IMPLEMENETED";
		//sub //TODO
	}
	else {
		v52 = 0xC0000;
		goto LABEL_4;
	}
	
	if (v52 <= 0) {
	LABEL_36:
		sb2->field_94 = -1;
		v51 = 1;
		goto LABEL_37;
	}
LABEL_4:
	v7 = new char[0xC0000u];
	if (!v7)
		goto LABEL_36;
	v8 = new char[v52];
	if (v8) {
		PrepareSoundData(sb2, v8, v7, 0xC0000u);
		delete[] v8;
	}
	else {
		v51 = 2;
	}
	v10 = sb2->SecondaryBuffer->Lock(0, 0xC0000, (LPVOID*)&v57, &v54, (LPVOID*)&v56, &v55, NULL);
	if (v10 != DS_OK) {
		sb2->field_94 = -1;
		if (v10 > DSERR_PRIOLEVELNEEDED) {
			if (v10 == DSERR_BUFFERLOST) {
				v51 |= 4;
				delete[] v7;
				goto LABEL_37;
			}
		}
		else {
			switch (v10) {
			case DSERR_PRIOLEVELNEEDED:
				v51 |= 0x20;
				delete[] v7;
				goto LABEL_37;
			case DSERR_INVALIDPARAM:
				v51 |= 0x10;
				delete[] v7;
				goto LABEL_37;
			case DSERR_INVALIDCALL:
				v51 |= 8;
				delete[] v7;
				goto LABEL_37;
			}
		}
		v51 |= 0x40;
		delete[] v7;
		goto LABEL_37;
	}
	memcpy(v57, v7, v54);
	if (v56) {
		unsigned int v14 = v55 >> 2;
		memcpy(v56, v7 + v54, 4 * v14);
		memcpy(v56 + 4 * v14, v7 + v54 + 4 * v14, v55 & 3);
	}
	sb2->SecondaryBuffer->Unlock(v57, v54, v56, v55);
	sb2->SecondaryBuffer->SetCurrentPosition(NULL);
	
	v52 = 64;
	v53 = sb2->field_4C >> 12;
	if (sb2->field_B8 >= sizeof(SoundMng::SomeAudioTable) / sizeof(SoundMng::SomeAudioTable[0]))
		throw "THIS HAPPENED";
	v53 *= SoundMng::SomeAudioTable[sb2->field_B8];
	v53 >>= 12;

	SoundMng::GetPanAndVolume((int*)&v52, &v53);
	SoundMng::SetBufferVolume(sb2->SecondaryBuffer, v53);

	sb2->field_AC = 0;
	sb2->SecondaryBuffer->Play(NULL, 0x10000, DSBPLAY_LOOPING);
	delete[] v7;
LABEL_37:
	if (sb2->field_94 >= 0) {
		while (true) {
			SoundController::SomeValue = 1;
			Sleep(1u);
			if (sb2->field_94 < 0)
				goto LABEL_80;
			SoundController::SomeValue = 2;
			if (sb2->SecondaryBuffer->GetCurrentPosition(&v54, &v55) == S_OK)
				break;
		LABEL_79:
			if (sb2->field_94 < 0)
				goto LABEL_80;
		}
		v22 = v54 - sb2->field_AC;
		v23 = v55 - sb2->field_AC;
		v24 = v54 - 0xC000;
		if (v22 < 0)
			v22 += 0xC0000 * ((-1 - v22) / 0xC0000u + 1);
		if (v23 < 0)
			v23 += 0xC0000 * ((-1 - v23) / 0xC0000u + 1);
		
		if (v22 > v23) {
			while (v24 < 0) {
				v24 += 0xC0000;
			}

			sb2->field_AC = v24;
			sb2->field_AC /= v52;
			sb2->field_AC *= v52;
			v22 = v52;
		}

		int v26 = v22 / 0xC000;
		if (v26 <= 4) {
			if (v26 <= 0)
				goto LABEL_76;
		}
		else
			v26 = 4;
		SoundController::SomeValue = 3;

		v52 = 0xC000 * v26;
		v27 = new char[2 * (v52 >> 1)];
		if (!v27) {
			sb2->field_94 = -1;
			v51 |= 1;
			goto LABEL_76;
		}

		if (header->EncodingFormat == Enums::AudioEncoding::Encoding0) {
			v29 = header->field_2F;
			v53 = v52 / (2 * v29);
			sb2->field_98 += v53 / header->nChannels;
			v31 = v53 * ((header->field_2F >> 1) + 1);
			v53 = v31;
			v32 = new char[v31];
		}
		else if (header->EncodingFormat == Enums::AudioEncoding::ATRAC3) {
			v29 = header->wBitsPerSample;
			v53 = (int)v52 / (int)(header->nChannels * (v29 >> 3));
			sb2->field_98 += v53;
			v53 = v53 / 1024 + 8;
			v53 *= sb2->field_7C;
			v32 = new char[v53];
		}
		else {
			v29 = header->nChannels;
			v53 = (int)v52 / (int)(v29 * (header->wBitsPerSample >> 3));
			sb2->field_98 += v53;
			v31 = v52;
			v32 = new char[v31];
		}

		if (v32) {
			PrepareSoundData(sb2, v32, v27, v52);
			delete[] v32;
			SoundController::SomeValue = 4;
			v10 = sb2->SecondaryBuffer->Lock(sb2->field_AC, v52, (LPVOID*)&v57, &v54, (LPVOID*)&v56, &v55, NULL);
			if (v10 != S_OK) {
				sb2->field_94 = -1;
				if (v10 > DSERR_PRIOLEVELNEEDED) {
					if (v10 == DSERR_BUFFERLOST) {
						v51 |= 4;
						delete[] v27;
						goto LABEL_76;
					}
				}
				else {
					switch (v10) {
					case DSERR_PRIOLEVELNEEDED:
						v51 |= 0x20;
						delete[] v27;
						goto LABEL_76;
					case DSERR_INVALIDPARAM:
						v51 |= 0x10;
						delete[] v27;
						goto LABEL_76;
					case DSERR_INVALIDCALL:
						v51 |= 8;
						delete[] v27;
						goto LABEL_76;
					}
				}
				v51 |= 0x40;
				delete[] v27;
				goto LABEL_76;
			}
			memcpy(v57, v27, v54);
			if (v56) {
				unsigned int v38 = v55 >> 2;
				memcpy(v56, v27 + v54, 4 * (v55 >> 2));
				memcpy(v56 + 4 * v38, v27 + v54 + 4 * v38, v55 & 3);
			}
			sb2->SecondaryBuffer->Unlock(v57, v54, v56, v55);
			sb2->field_AC = (int)(v52 + sb2->field_AC) % 0xC0000;
			delete[] v27;
		}
		else {
			v51 |= 2;
			delete[] v27;
		}
	LABEL_76:
		if (sb2->field_98 >= header->SampleCount) {
			if (header->field_1C < 0)
				goto LABEL_80;
			sb2->field_98 = sb2->field_98 + header->field_1C - header->SampleCount;
		}
		goto LABEL_79;
	}
LABEL_80:
	if (sb2->field_68) {
		//sub //todo wavobj destructor
		throw "NOT IMPLEMENTED CH68";
	}
	sb2->SecondaryBuffer->Stop();
	sb2->field_94 = -1;
	SoundController::SomeValue = -1;
	//sub
	sb2->HandleCpy = nullptr;
	if (v51)
		v51 |= 0x100;
	return v51;
	return 0;
}

void Helper7(char* a1, unsigned int a2, unsigned int a3) {
	char* v3 = new char[0x30];
	if (v3) {
		SoundBuff2* v4 = SoundController::SoundBuff2Bin;
		int* intv3 = reinterpret_cast<int*>(v3);
		memset(v3, 0, 0x30);
		FILE* v5{ nullptr };
		fopen_s(&v5, a1, "rb");
		if (v5) {
			fread_s(v3, 0x30, sizeof(char), 0x30, v5);
			fclose(v5);
		}
		if (strcmp(v3, SoundMng::BGMPrefix)) {
			memset(v3, 0, 0x30);
			char* v7 = new char[0x20];
			if (v7) {
				int* intv7 = reinterpret_cast<int*>(v7);
				ParseAudioHeader(intv7, a1, 1);
				if (intv7[7] > 0) {
					if (intv7[0])
						intv3[3] = 2;
					else
						intv3[3] = 1;
					intv3[10] = intv7[7];
					v3[46] = v7[4];
					v3[47] = 1;
					intv3[4] = intv7[6];
					v3[45] = v7[8];
					intv3[6] = intv7[6];
					intv3[8] = intv7[3];
					if (intv7[5] <= 0)
						intv3[7] = -1;
					else
						intv3[7] = intv7[4];
					v3[44] = 127;
					intv3[5] = 1024;
				}
				delete[] v7;
			}
		}
		if (intv3[4] > 0) {
			int v9 = intv3[5];
			bool v10 = v9 == 1024;
			if (v9 < 1024) {
				if (v9 != v4->field_94)
					goto LABEL_20;
				v10 = v9 == 1024;
			}
			if (v10) {
			LABEL_20:
				if (v4->SecondaryBuffer) {
					v4->field_94 = -1;
					if (v4->Handle) {
						while (WaitForSingleObject(v4->Handle, 0)) {
							Sleep(1u);
						}
						CloseHandle(v4->Handle);
						v4->Handle = nullptr;
					}
					if (v4->SecondaryBuffer) {
						v4->SecondaryBuffer->Stop();
						if (v4->SecondaryBuffer) {
							v4->SecondaryBuffer->Release();
							v4->SecondaryBuffer = nullptr;
						}
						if (v4->PrimaryBuffer) {
							v4->PrimaryBuffer->Release();
							v4->PrimaryBuffer = nullptr;
						}
					}
				}

				memset(v4->FileName, 0, sizeof(v4->FileName));
				LPWAVEFORMATEX fmt = new WAVEFORMATEX();
				LPDSBUFFERDESC desc = new DSBUFFERDESC();
				if (fmt) {
					if (desc) {
						strcpy_s(v4->FileName, sizeof(v4->FileName), a1);
						fmt->wFormatTag = 1;
						fmt->nChannels = *((unsigned char*)v3 + 46);
						fmt->wBitsPerSample = *((unsigned char*)v3 + 45);
						fmt->nBlockAlign = *((unsigned char*)v3 + 45) * *((unsigned char*)v3 + 46) / 8;
						fmt->cbSize = 0;
						fmt->nSamplesPerSec = (intv3[8] + intv3[9]) & 0x7FFFFFFF;
						fmt->nAvgBytesPerSec = fmt->nSamplesPerSec * fmt->nBlockAlign;
						memset(desc, 0, sizeof(DSBUFFERDESC));
						desc->dwSize = 36;
						desc->lpwfxFormat = fmt;
						desc->dwFlags = SoundController::SoundOOFFlag | 
							DSBCAPS_LOCDEFER |
							DSBCAPS_GETCURRENTPOSITION2 |
							DSBCAPS_CTRLVOLUME;
						desc->dwBufferBytes = 0xC0000;
						if (SoundMng::DirectSound8->CreateSoundBuffer(desc, &v4->PrimaryBuffer, NULL) >= DS_OK) {
							if (v4->PrimaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&v4->SecondaryBuffer) < DS_OK) {
								if (v4->PrimaryBuffer) {
									v4->PrimaryBuffer->Release();
									v4->PrimaryBuffer = nullptr;
								}
							}
						}
					}

					delete fmt;
				}
				if (desc)
					delete desc;
				if (v4->PrimaryBuffer) {
					if (intv3[5] < 0 || intv3[5] >= 1024) {
						v4->field_98 = 0;
						v4->field_88 = 0;
						v4->field_B0 = 0;
					}
					else {
						v4->field_98 = SoundController::AudioSpotBackups[intv3[5]];
						v4->field_88 = v4->field_98;
						v4->field_B0 = v4->field_98;
					}

					if (SoundController::AudioStartPos > 0.0) {
						double v28 = (double)intv3[6] * SoundController::AudioStartPos;
						v4->field_98 = v28;
						v4->field_88 = v28;
						v4->field_B0 = v28;
						SoundController::AudioStartPos = 0;
					}
					
					memcpy(&v4->FileHeader, v3, sizeof(v4->FileHeader));
					v4->field_94 = intv3[5];
					v4->field_B8 = (unsigned char)v3[44];
					v4->field_40 = v4;
					v4->Handle = CreateThread(NULL, 0, Handler7Thread, v4, NULL, &v4->field_3C);
					v4->HandleCpy = v4->Handle;
					if (v4->Handle)
						SetThreadPriority(v4->Handle, THREAD_PRIORITY_ABOVE_NORMAL);
					else
						v4->field_94 = -1;
					v4->field_54 = 0;
					int v31 = 50 * a3 / 60;
					if (v31) {
						int v32 = SoundMng::SomeAudioTable[1];
						int v33 = (SoundMng::SomeAudioTable[a2 & 0x7F] - SoundMng::SomeAudioTable[1]) << 12;
						v33 |= 0x800;
						v4->field_48 = v31;
						v4->field_4C = v32 << 12;
						v4->field_50 = v33 / v31;
						delete[] v3;
						return;
					}
					v4->field_50 = 0;
					v4->field_48 = 1;
					v4->field_4C = SoundMng::SomeAudioTable[a2 & 0x7F] << 12;
				}
			}
		}
		delete[] v3;
	}
}
void Handle7(unsigned int* a1) {
	char* v1 = new char[0x104];
	if (v1) {
		memset(v1, 0, 0x104);
		char* v2 = v1;
		unsigned int* v3 = a1 + 7;
		for (int i = 0; i < 8; ++i) {
			if (v3 >= SoundController::CommandQueue + SoundController::CommandQueueSize)
				v3 = SoundController::CommandQueue;
			memcpy(v2, v3, 0x20);
			v2 += 32;
			v3 += 8;
		}
		Helper7(v1, a1[0], a1[1]);
		delete[] v1;
	}
}
void Handle8(unsigned int* a1) { 
	int* data = (int*)a1;
	int v3 = data[1] & 0x7F;
	int v5 = 50 * data[2] / 60;
	int v7{}, v8{};
	for (int i = 0; i < SoundController::SoundBuff2BinSize - 1; ++i) {
		SoundBuff2* sb2 = SoundController::SoundBuff2Bin + i;
		if (data[0] < 0 || sb2->field_94 == a1[0] || sb2->field_94 < 0) {
			if (v5) {
				v7 = (SoundMng::SomeAudioTable[v3] << 12) - sb2->field_4C;
				sb2->field_48 = v5;
				sb2->field_50 = (v7 + 2048) / v5;
			}
			else {
				v8 = SoundMng::SomeAudioTable[v3] << 12;
				sb2->field_50 = 0;
				sb2->field_4C = v8;
				sb2->field_48 = 1;
			}
		}
	}
}
void Handle9(unsigned int* a1) { 
	int v3 = (int)(50 * a1[1]) / 60;
	for (int i = 0; i < SoundController::SoundBuff2BinSize - 1; ++i) {
		SoundBuff2* sb2 = SoundController::SoundBuff2Bin + i;
		if ((int)a1[0] < 0 || sb2->field_94 == (int)a1[0] || (int)a1[0] < 0) {
			if (v3) {
				int v5 = (SoundMng::SomeAudioTable[1] << 12) - sb2->field_4C;
				sb2->field_48 = v3;
				sb2->field_50 = (v5 + 2048) / v3;
			}
			else {
				int v6 = SoundMng::SomeAudioTable[0];
				sb2->field_50 = 0;
				sb2->field_4C = v6 << 12;
				sb2->field_48 = 1;
			}
			sb2->field_54 = 1;
		}
	}
}
void Handle10(unsigned int* a1) { 
	int v1 = SoundController::SoundBuff2Bin[0].field_94;
	if (v1 >= 0 && v1 < SoundController::AudioPosBackupSize)
		SoundController::AudioSpotBackups[v1] = SoundController::SoundBuff2Bin[0].field_98;
}
void Handle11(unsigned int* a1) { 
	int v1 = (int)a1[0];
	if (v1 >= 0) {
		if (v1 < SoundController::AudioPosBackupSize)
			SoundController::AudioSpotBackups[v1] = 0;
	}
	else
		memset(SoundController::AudioSpotBackups, 0, sizeof(SoundController::AudioSpotBackups) * SoundController::AudioPosBackupSize);
}
void Handle12(unsigned int* a1) { 
	int inta1 = (int)*a1;
	SoundController::AudioStartPos = (double)inta1 / 4096.0;
}
void Handle13(unsigned int*) { throw "NOT IMPLEMENTED"; }

void (__cdecl *FFXI::CommandHandlers[CommandHandlerCount])(unsigned int*) = {
	Handle0,
	Handle1,
	Handle2,
	Handle3,
	Handle4,
	Handle5,
	Handle6,
	Handle7,
	Handle8,
	Handle9,
	Handle10,
	Handle11,
	Handle12,
	Handle13
};