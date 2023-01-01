#include "CTsZoneMap.h"
#include "Globals.h"
#include "CMoResourceMng.h"
#include "CYyResourceFile.h"
#include "CMoKeyframe.h"
#include "MmbListNode.h"
#include "CYyDb.h"
#include "CYyTexMng.h"
#include "CYyTex.h"
#include "CDx.h"
#include "MapChunk.h"
#include "MmbStruct.h"
#include "MmbStruct2.h"
#include "RidListNode.h"
#include "WMatrix.h"
#include "Strings.h"
#include "CYyCamMng2.h"
#include <vcruntime_string.h>
#include <iostream>
#include "XiZone.h"
#include "PlaceHolders.h"
#include "TextRenderer.h"
#include "UnderscoreAtStruct.h"
#include "CMoAttachments.h"

using namespace FFXI::CYy;

int CTsZoneMap::LoadedZoneID{ 0 };
int CTsZoneMap::ZBias{ 0 };
int CTsZoneMap::SomeInt2{ 0 };
int CTsZoneMap::SomeInt3{ 0 };
int CTsZoneMap::SomeInt4{ 0 };
int CTsZoneMap::SomeInt5{ 0 };

float CTsZoneMap::SomeTiming{ 0.0 };

bool CTsZoneMap::EventFlag{ false };
bool CTsZoneMap::ZoneFlag{ false };
bool CTsZoneMap::SomeFlag{ true };
bool CTsZoneMap::SomeFlag2{ false };

char CTsZoneMap::SomeBytes[256] = { 0 };
SomeStruct CTsZoneMap::SomeStructs[256]{};

unsigned char initDecryptTable[];
const DWORD VertexShader1Decl[] = {
	0x20000000, 0x40020000, 0x20000001, 0x40020003, 0x40040005,
	0x40010007, 0xFFFFFFFF
};

const DWORD VertexShader2Decl[] = {
	0x20000000, 0x40020000, 0x40020003, 0x40040005, 0x40010007, 0xFFFFFFFF
};

D3DLIGHT8 TempLight{};

//Local funcs
void CreateAreas(CYyResourceFile* a1, CTsZoneMapSubStruct1* a2) {
	if (a2->field_70 != 0)
		return;
	
	for (int i = 0; i < a2->FourCCListSize; ++i) {
		char* mem = FFXI::StorageLists::instance->Get(sizeof(XiArea), FFXI::Constants::Enums::MEM_MODE::Ex);
		if (mem != nullptr) {
			new (mem) XiArea(a1, a2->FourCCList[i]);
		}
	}
}

void CleanField64(void* a1) {
	int* intData = (int*)a1;
	for (int i = 0; i < sizeof(CTsZoneMap::SomeStructs) / sizeof(CTsZoneMap::SomeStructs[0]); ++i) {
		if (*intData) {
			SomeStruct* ss = (SomeStruct*)(intData + 1);
			if (ss) {
				ss->field_4 -= 1;
				if (ss->field_4 == 0) {
					memset(ss, 0, sizeof(*ss));
				}
			}
		}
		intData += 19;
	}
}

void DoSomeCalc(FFXI::Math::WMatrix* a2, float a3, float a4, float a5, float a6, float a7)
{
	FFXI::Math::WMatrix v33{}, v34{};
	v33.Identity();
	v34.Identity();

	v33._33 = -1.0;
	
	double v7 = a5 + a5;
	double v9 = a7 - a6;
	v34._11 = v7 / a3;
	v34._22 = v7 / a4;
	v34._33 = (a6 + a7) / v9;
	v34._43 = a6 * a7 * -2.0 / v9;
	v34._34 = 1.0;
	v34._44 = 0.0;
	
	D3DXMatrixMultiply(a2, &v33, &v34);
}

void __fastcall SomeMZBHelper(CTsZoneMapSubStruct1* ss, MapChunk* chunk, char* data)
{
	char v15[256] = { 0 };
	int v3 = -1;
	for (int i = 15; i > -1; --i) {
		v15[i] = data[i];
		if (v3 < 0 && (unsigned char)data[i] > ' ')
			v3 = i;
	}

	MmbListNode* mmb = ss->MMBManager.GetByName(data, false);
	chunk->field_18 = mmb;
	chunk->field_10 = mmb;
	chunk->field_14 = mmb;
	
	if (v3 >= 1) {
		char v8 = v15[v3];
		if (v8 == 'h' || v8 == 'm' || v8 == 'l') {
			v15[v3] = 'm';
			mmb = ss->MMBManager.GetByName(v15, false);
			if (mmb) {
				chunk->field_10 = mmb;
				chunk->field_14 = mmb;
				chunk->field_18 = mmb; 
			}
			v15[v3] = 'h';
			mmb = ss->MMBManager.GetByName(v15, false);
			if (mmb) {
				chunk->field_18 = mmb;
				if (!chunk->field_14)
					chunk->field_14 = mmb;
				if (!chunk->field_10)
					chunk->field_10 = mmb;
			}
			v15[v3] = 'l';
			mmb = ss->MMBManager.GetByName(v15, false);
			if (mmb) {
				chunk->field_10 = mmb;
				if (!chunk->field_18)
					chunk->field_18 = mmb;
				if (!chunk->field_14)
					chunk->field_14 = mmb;
			}
		}
	}

	chunk->field_D6 = 0;
	if (*data == '_')
		chunk->field_D6 = 1;
	if (*data == '#')
		chunk->field_D6 = 2;
}

SomeStruct* localGetSomeStruct(int a1) {
	for (int i = 0; i < sizeof(CTsZoneMap::SomeStructs) / sizeof(CTsZoneMap::SomeStructs[0]); ++i) {
		SomeStruct* ss = CTsZoneMap::SomeStructs + i;
		if (!ss->field_0 || ss->field_0 == a1) {
			ss->field_4 += 1;
			ss->field_0 = a1;
			ss->field_6 = i;
			return ss;
		}
	}
	return nullptr;
}
void localDoSomething(void* a1) {
	int* intData = (int*)a1;
	for (int i = 0; i < sizeof(CTsZoneMap::SomeStructs) / sizeof(CTsZoneMap::SomeStructs[0]); ++i) {
		if (*intData) {
			SomeStruct* ss = localGetSomeStruct(*intData);
			intData[1] = (int)ss;
			if (ss)
				CTsZoneMap::SomeBytes[i] = ss->field_6;
		}
		intData += 19;
	}
}
//~Local funcs
FFXI::CYy::CTsZoneMap::CTsZoneMap()
{
	//sub //TODO
	this->field_34 = 0;
	this->data[0] = nullptr;
	this->data[1] = nullptr;
	memset(this->SubStruct1s, 0, sizeof(this->SubStruct1s));
	this->field_54 = 0;
	this->field_150 = 1;
	this->field_154 = 0;
	this->VertexShader1 = 0;
	this->VertexShader2 = 0;
	this->field_39498 = 0;
	this->field_3946C = 0;
	this->field_39470 = 0;
	this->field_39474 = 0;
	this->field_39478 = 0;
	this->field_3947C = 0;
	this->field_39494 = 0;
	this->field_158 = 0;
	this->field_3944C = 0;
	this->field_39450 = 0;
	this->field_38E1C.Identity();
	InitSomeStructs();
	this->dataindex = 0;
	this->BigStruct.field_4 = -1;
	this->field_3C = 0;
	LoadedZoneID = 0;
	this->field_40 = 0;
	this->field_44 = 0;
	this->field_48 = 0;
	this->field_50 = 0;
	this->field_174 = 0;
	this->PrimitiveCount = 0;
	this->field_39430 = 0;
	this->field_39438 = 0;
	this->field_3943C = 0;
	this->field_39440 = 0;
	this->field_39444 = 0;
	this->field_38D90 = 0;
	this->field_39130 = 0;
	this->field_39448 = 0;
	this->DrawDistance = 0;
	this->SystemKage = nullptr;
	CTsZoneMap::SomeFlag = true;
	this->field_15C = 0;
	this->field_3949C = 0;
	this->field_16C = 2.0;
	this->field_170 = 2.0;
	this->field_38D94 = 0;
	this->field_38D98 = 0;
	CTsZoneMap::ZBias = 0;
	CTsZoneMap::SomeInt2 = 8;
	CTsZoneMap::SomeInt3 = 15;
	CTsZoneMap::SomeInt4 = 15;
	CTsZoneMap::SomeInt5 = 0;
	this->field_3954C = 1.0;
	this->field_394A4 = 1;
	this->SetLightParams();
}

FFXI::CYy::CTsZoneMap::~CTsZoneMap()
{
}

void FFXI::CYy::CTsZoneMap::AddRid(char* a2)
{
	this->RidManager.Add(a2);
}

void FFXI::CYy::CTsZoneMap::SetSomeFlag2(bool a1)
{
	SomeFlag2 = a1;
}

void FFXI::CYy::CTsZoneMap::InitSomeStructs()
{
	memset(SomeStructs, 0, sizeof(CTsZoneMap::SomeStructs));
	for (int i = 0; i < sizeof(SomeStructs) / sizeof(SomeStructs[0]); ++i) {
		SomeStructs[i].field_8.Type = D3DLIGHT_DIRECTIONAL;
		SomeStructs[i].field_8.Direction = { 0.0, 1.0, 0.0 };
	}
	memset(SomeBytes, 0, sizeof(SomeBytes));
}

void FFXI::CYy::CTsZoneMap::SetLightParams()
{
	if (this->field_394A4)
	{
		this->field_394A8 = 0;
		this->field_394AC = 1;
		this->field_394B0 = 2;
		this->field_394B4 = 3;
		this->field_394B8 = 4;
		this->field_394BC = 5;
	}
	else {
		this->field_394AC = 0;
		this->field_394A8 = 0;
		this->field_394B0 = 1;
		this->field_394B4 = 2;
		this->field_394B8 = 3;
		this->field_394BC = 4;
	}
}

MmbListNode* FFXI::CYy::CTsZoneMap::OpenMmb(CYyResourceFile** a2, int* a3, int a4, int a5, D3DXVECTOR4* a6)
{
	CTsZoneMapSubStruct1* ss = this->SubStruct1s;
	if (this->field_54)
		ss += this->dataindex;

	return ss->MMBManager.AddFromData(a2, a3, a4, ss->field_58, a5, a6, this->field_54);
}

void FFXI::CYy::CTsZoneMap::OpenMzb(void* a2, CYyResourceFile* a3)
{
	unsigned char* byteData = (unsigned char*)a2;
	int* intData = (int*)a2;

	//MZB First 8 bytes:
	// 0-2: encrypted byte count
	// 3: version
	// 4-6: chunk count
	// 7: initial decrypt table index


	int Version = byteData[3];
	//Decode
	if (Version >= 0x1B) {
		int decrypter = initDecryptTable[255 - byteData[7]];
		int encryptedCount = intData[0] & 0xFFFFFF;
		int pieceLen = 0;
		int decrypterAdder = 0;	
		//The first 8 bytes are never encrypted
		for (int position = 0; position < encryptedCount; position += pieceLen) {
			pieceLen = (((unsigned char)decrypter >> 4) & 7) + 16;
			//Check if this piece is encrypted
			if ((decrypter & 1) && (position + pieceLen) < encryptedCount) {
				char* v11 = (char*)a2 + position + 8; 
				for (int j = 0; j < pieceLen; ++j)
					v11[j] = ~v11[j];
			}
			decrypter += ++decrypterAdder;
		}
	}
	this->data[this->dataindex] = a2;
	CTsZoneMapSubStruct1* ss = this->SubStruct1s + this->dataindex;
	ss->field_0 = byteData[28] + 1;
	ss->data = a2;
	ss->field_5C = a3;
	ss->field_10 = byteData[12];
	ss->field_14 = byteData[13];
	ss->field_18 = byteData[14];
	ss->field_1C = byteData[15];
	ss->field_64 = 0;
	ss->MapChunkCount = intData[1] & 0xFFFFFF;
	if (intData[6] && Version >= 0x12) {
		ss->field_64 = byteData + intData[6];
		localDoSomething(ss->field_64);
	}

	int size = sizeof(MapChunk) * ss->MapChunkCount;
	MapChunk* chunks = (MapChunk*)CYyObject::Wrap(size, Constants::Enums::MEM_MODE::Ex);
	ss->MapChunks = chunks;
	memset(chunks, 0, 4 * (size >> 2));
	if (ss->MapChunkCount > 0 && chunks) {
		FFXI::Math::WMatrix SomeMatrix1{}, SomeMatrix2{}, SomeMatrix3{}, SomeMatrix4{}, SomeMatrix5{};
		FFXI::Math::WMatrix SomeMatrix6{}, SomeMatrix7{}, SomeMatrix8{}, SomeMatrix9{};
		memset(&SomeMatrix1, 0, sizeof(SomeMatrix1));
		memset(&SomeMatrix2, 0, sizeof(SomeMatrix2));
		memset(&SomeMatrix3, 0, sizeof(SomeMatrix3));
		memset(&SomeMatrix4, 0, sizeof(SomeMatrix4));
		SomeMatrix1._44 = 1.0f;
		SomeMatrix2._44 = 1.0f;
		SomeMatrix3._44 = 1.0f;
		SomeMatrix4._44 = 1.0f;
		SomeMatrix5._44 = 1.0f;

		SomeMatrix2._11 = 1.0f;
		SomeMatrix3._22 = 1.0f;
		SomeMatrix4._33 = 1.0f;

		SomeMatrix5._11 = 1.0f;
		SomeMatrix5._22 = 1.0f;
		SomeMatrix5._33 = 1.0f;


		//Each data chunk is length 100
		int* dataStart = (int*)((char*)a2 + 32);
		for (unsigned int i = 0; i < ss->MapChunkCount; ++i) {
			MapChunk* chunk = ss->MapChunks + i;
			int* chunkData = dataStart + 25 * i;

			//Decode string
			if (Version >= 0x1B) {
				for (int j = 0; j < 4; ++j)
					chunkData[j] ^= 0x55555555;
			}

			this->LinkMMBtoMZB(ss, chunk, (char*)chunkData);

			memcpy(&chunk->Translation, chunkData + 4, sizeof(chunk->Translation));
			memcpy(&chunk->Rotation, chunkData + 7, sizeof(chunk->Rotation));
			memcpy(&chunk->Scaling, chunkData + 10, sizeof(chunk->Scaling));

			float v24 = chunk->Translation.x - -57.946602;
			float v25 = chunk->Translation.y - -15.7264;
			float v26 = chunk->Translation.z - -269.38699;

			if (v24 < 0.0) v24 = -v24;
			if (v25 < 0.0) v25 = -v25;
			if (v26 < 0.0) v26 = -v26;

			if (this->field_3C == FFXI::Constants::Enums::Zone::Labyrinth_of_Onzozo) {
				if (v24 < 0.05f && v25 < 0.05f && v26 < 0.05f) {
					if (!strncmp((char*)chunkData, Constants::Strings::es_anmo, 7u))
						chunk->field_E = 1;
				}
			}
			
			//Scaling
			SomeMatrix1._11 = chunk->Scaling.x;
			SomeMatrix1._22 = chunk->Scaling.y;
			SomeMatrix1._33 = chunk->Scaling.z;
			
			SomeMatrix6.Identity();
			SomeMatrix6.MatrixMultiply(&SomeMatrix1);

			//Rotate X
			SomeMatrix2._22 = cos(chunk->Rotation.x);
			SomeMatrix2._23 = sin(chunk->Rotation.x);
			SomeMatrix2._32 = -SomeMatrix2._23;
			SomeMatrix2._33 = SomeMatrix2._22;
			SomeMatrix6.MatrixMultiply(&SomeMatrix2);
			

			//Rotate Y
			SomeMatrix3._11 = cos(chunk->Rotation.y);
			SomeMatrix3._13 = -sin(chunk->Rotation.y);
			SomeMatrix3._31 = -SomeMatrix3._13;
			SomeMatrix3._33 = SomeMatrix3._11;
			SomeMatrix6.MatrixMultiply(&SomeMatrix3);

			//Rotate Z
			SomeMatrix4._11 = cos(chunk->Rotation.z);
			SomeMatrix4._12 = sin(chunk->Rotation.z);
			SomeMatrix4._21 = -SomeMatrix4._12;
			SomeMatrix4._22 = SomeMatrix4._11;
			SomeMatrix6.MatrixMultiply(&SomeMatrix4);

			//Translate
			SomeMatrix5._41 = chunk->Translation.x;
			SomeMatrix5._42 = chunk->Translation.y;
			SomeMatrix5._43 = chunk->Translation.z;
			D3DXMatrixMultiply(&chunk->field_40, &SomeMatrix6, &SomeMatrix5);

			chunk->field_154 = 0;
			chunk->field_15C = 0;
			chunk->field_158 = 0;
			chunk->field_160 = 0;

			chunk->DoTransNorms();

			FFXI::Math::WMatrix SomeMatrix10{};
			SomeMatrix10.Identity();

			FFXI::Math::WMatrix Translater{}, ZRotation{}, YRotation{}, XRotation{};

			D3DXMatrixTranslation(&Translater, -chunk->Translation.x, -chunk->Translation.y, -chunk->Translation.z);
			D3DXMatrixRotationZ(&ZRotation, -chunk->Rotation.z);
			D3DXMatrixRotationY(&YRotation, -chunk->Rotation.y);
			D3DXMatrixRotationX(&XRotation, -chunk->Rotation.x);

			float v105{ 1.0 };
			if (chunk->Scaling.x != 0.0)
				v105 /= chunk->Scaling.x;

			float v111{ 1.0 };
			if (chunk->Scaling.y != 0.0)
				v111 /= chunk->Scaling.y;

			float v109{ 1.0 };
			if (chunk->Scaling.z != 0.0)
				v109 /= chunk->Scaling.z;


			FFXI::Math::WMatrix Scaler{};
			D3DXMatrixScaling(&Scaler, v105, v111, v109);

			FFXI::Math::WMatrix SomeMatrix16{}, SomeMatrix17{}, SomeMatrix18{}, SomeMatrix19{};
			D3DXMatrixMultiply(&SomeMatrix16, &SomeMatrix10, &Translater);
			D3DXMatrixMultiply(&SomeMatrix17, &SomeMatrix16, &ZRotation);
			D3DXMatrixMultiply(&SomeMatrix18, &SomeMatrix17, &YRotation);
			D3DXMatrixMultiply(&SomeMatrix19, &SomeMatrix18, &XRotation);
			D3DXMatrixMultiply(&chunk->TransRotScaler, &SomeMatrix19, &Scaler);
			chunk->field_8 = 2;
			chunk->field_9 = ((char*)chunkData)[68];
			chunk->field_A = ((char*)chunkData)[69];
			chunk->field_B = ((char*)chunkData)[70] & 1;
			chunk->field_D = ((char*)chunkData)[71];
			chunk->field_F = (((char*)chunkData)[70] >> 2) & 1;
			
			chunk->field_C0 = chunkData[13];
			float one = ((float*)chunkData)[14];
			float two = ((float*)chunkData)[15];
			float three = ((float*)chunkData)[16];
			chunk->field_C8 = one * one;

			if (one > two)
				chunk->field_CC = chunk->field_C8;
			else
				chunk->field_CC = two * two;

			chunk->field_D0 = three * three;
			if (chunk->field_D0 < 1 && chunk->field_D0)
				int i = 1;
			chunk->field_D4 = 0;
			if (chunk->Scaling.x * chunk->Scaling.z < 0.0)
				chunk->field_D4 = 1;

			chunk->field_D8 = nullptr;
			if (chunkData[18])
				chunk->field_D8 = (char*)a2 + chunkData[18];

			chunk->SomeFourCC = chunkData[19];

			//not sure of this data type
			chunk->field_C4 = chunkData[20];

			if (Version < 0x12) {
				chunk->field_E4 = 0;
				chunk->field_E8 = 0;
				chunk->field_EC = 0;
				chunk->field_F0 = 0;
			}
			else {
				chunk->field_E4 = chunkData[21];

				//Not sure of these data types
				chunk->field_E8 = chunkData[22];
				chunk->field_EC = chunkData[23];
				chunk->field_F0 = chunkData[24];
			}
		}

		char nullit[17] = { 0 };
		/*for (auto& m : mmblist) {
			MapChunk* chunk{ nullptr };
			for (unsigned int i = 0; i < ss->MapChunkCount; ++i) {
				chunk = ss->MapChunks + i;
				int* chunkData = dataStart + 25 * i;
				if (memcmp(m.first.c_str(), chunkData, 16) == 0)
					break;
			}

			std::cout << m.first << ": " << std::dec << m.second << std::endl;
			MmbListNode* mmb = ss->MMBManager.GetByName(m.first.c_str(), false);
			std::map<std::string, int> texlist{};
			for (int j = 0; j < mmb->field_44; ++j) {
				MmbStruct* s = mmb->field_60 + j;
				for (int k = 0; k < s->ListCount; ++k) {
					MmbStruct2* s2 = s->List + k;
					memcpy(nullit, s2->field_4->Unique, 16);
					std::string tex{ nullit };
					texlist[tex] += 1;
				}
			}
			for (auto& n : texlist)
				std::cout << '\t' << n.first << ": " << std::dec << n.second << std::endl;
		}*/
	}

	if (Version < 0x15) {
		ss->field_30 = (int*)((char*)a2 + intData[5]);
		ss->field_2C = intData[4];
		ss->field_34 = ss->field_2C != 0;
		ss->field_38 = nullptr;
	}
	else {
		ss->field_30 = (int*)((char*)a2 + intData[5] + 4);
		ss->field_2C = *(ss->field_30 - 1);
		ss->field_34 = ss->field_2C != 0;
		ss->field_38 = (int*)((char*)a2 + intData[4]);
		ss->GetChunky((char*)a2, ss->field_38);
	}

	ss->MakeChunkList();
	ss->InitUnderscoreAtStructs();
	ss->BuildFourCCList();
	this->MzbChunk();

	ss->field_74 = ss->MMBManager.GetSomething();

	int v85 = intData[2];
	if (v85) {
		ss->field_C = (int*)((char*)a2 + v85);
		if (this->field_154) {
			throw "not implemented";
			//sub //todo
		}
		else {
			this->BigStruct.PrepareData(ss, SomeBytes, Version, ((char*)a2)[29]);
			RidListNode* node = RidManager::GetHead();
			while (node) {
				this->BigStruct.InitRid(node);
				node = node->Next;
			}
			this->field_154 = 1;
		}
	}

	const int subcount = sizeof(this->SubStruct1s) / sizeof(this->SubStruct1s[0]);
	this->dataindex += 1;
	if (this->dataindex < 0)
		this->dataindex = 0;
	else if (this->dataindex >= subcount)
		this->dataindex = subcount - 1;
	
}

void FFXI::CYy::CTsZoneMap::LinkMMBtoMZB(CTsZoneMapSubStruct1* a2, MapChunk* a3, char* a4)
{
	if (a2->field_0) {
		SomeMZBHelper(a2, a3, a4);
		if (a3->field_14 != nullptr)
			return;
	}

	for (int i = 0; i < sizeof(this->SubStruct1s) / sizeof(this->SubStruct1s[0]); ++i) {
		CTsZoneMapSubStruct1* ss = this->SubStruct1s + i;
		if (ss->field_0) {
			SomeMZBHelper(ss, a3, a4);
			if (a3->field_14)
				break;
		}
	}
}

void FFXI::CYy::CTsZoneMap::MzbChunk()
{
	for (int i = 0; i < sizeof(this->SubStruct1s) / sizeof(this->SubStruct1s[0]); ++i) {
		CTsZoneMapSubStruct1* ss = this->SubStruct1s + i;
		if (ss->field_0) {
			if (ss->MapChunkCount > 0) {
				for (unsigned int j = 0; j < ss->MapChunkCount; ++j) {
					MapChunk* chunk = ss->MapChunks + j;

					chunk->field_8 = 2;
					if (chunk->field_C0)
						chunk->field_8 = 0;

					if (chunk->field_C4 == this->BigStruct.field_4)
						chunk->field_8 = 1;
				}
			}
		}
	}
}

void FFXI::CYy::CTsZoneMap::CleanSubstructs()
{
	for (CTsZoneMapSubStruct1& ss : this->SubStruct1s)
		ss.CleanFourCCList();
	
}

void FFXI::CYy::CTsZoneMap::ClearZone()
{
	Init();
	this->AnotherSubStr.Reset();
	this->field_34 = 1;
}

void FFXI::CYy::CTsZoneMap::Init()
{
	this->RidManager.Clean();
	
	for (int i = 0; i < sizeof(this->SubStruct1s) / sizeof(this->SubStruct1s[0]); ++i)
		this->CleanSubstruct(i);
	
	this->dataindex = 0;
	this->BigStruct.field_4 = -1;
	this->field_3C = 0;
	CTsZoneMap::LoadedZoneID = 0;
	this->field_154 = 0;
	this->field_158 = 0;
	InitSomeStructs();
	this->field_38E1C.Identity();

	this->BigStruct.Clean();
	
	if (this->field_174) {
		this->field_174->SomeClean3();
		CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(this->field_174->PreHeader.PointerToThisInRPL);
		this->field_174 = nullptr;
	}
	
	this->field_34 = 1;
	this->field_44 = 0;

	//sub //todo clearing some statics
}

void FFXI::CYy::CTsZoneMap::InitWeather(CYyResourceFile* a2)
{
	for (int i = 0; i < sizeof(this->SubStruct1s) / sizeof(this->SubStruct1s[0]); ++i) {
		this->SubStructResFunc(a2, this->SubStruct1s + i);
	}
}

void FFXI::CYy::CTsZoneMap::SetSubAreaFourCCs(CTsZoneMapSubStruct1* a2)
{
	for (unsigned int i = 0; i < a2->MapChunkCount; ++i) {
		MapChunk* chunk = a2->MapChunks + i;
		chunk->field_E0 = nullptr;
		if (chunk->SomeFourCC != 0) {
			chunk->field_E0 = XiArea::GetByFourCC(chunk->SomeFourCC);
		}
	}
}

void FFXI::CYy::CTsZoneMap::SubStructResFunc(CYyResourceFile* a2, CTsZoneMapSubStruct1* a3)
{
	if (a3->field_0 == 0)
		return;

	CreateAreas(a2, a3);
	this->SetSubAreaFourCCs(a3);
}

void FFXI::CYy::CTsZoneMap::InitSomeStatics()
{
	CTsZoneMap::ZBias = 0;
	CTsZoneMap::SomeInt5 = 0;
	this->field_3954C = 1.0;

	if (FFXI::CYy::CDx::instance->GPUVendor == 0)
	{
		CTsZoneMap::SomeInt2 = 8;
		CTsZoneMap::SomeInt3 = 15;
		CTsZoneMap::SomeInt4 = 15;
	}
	else {
		CTsZoneMap::SomeInt2 = 1;
		CTsZoneMap::SomeInt3 = 2;
		CTsZoneMap::SomeInt4 = 2;
	}
}

void FFXI::CYy::CTsZoneMap::CreateVertexShaders()
{
	FFXI::CYy::CDx::instance->DXDevice->CreateVertexShader(VertexShader1Decl , 0, &this->VertexShader1, 0);
	FFXI::CYy::CDx::instance->DXDevice->CreateVertexShader(VertexShader2Decl, 0, &this->VertexShader2, 0);
}

void FFXI::CYy::CTsZoneMap::InitBumpMappingShaders()
{
	this->field_39498 = 0;
	this->field_3949C = 0;
	if (Config::MainRegistryConfig::BumpMapping && FFXI::CYy::CDx::instance->field_909 == 1 && FFXI::CYy::CDx::instance->field_90A == 1) {
		throw "Bump mapping shaders not implemented";
	}
}

void FFXI::CYy::CTsZoneMap::SomeInit2()
{
	this->field_3912C = this->BigStruct.field_1807C;
	CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;
	if (cammng) 
		this->field_38D4C = cammng->Position;
	this->field_3949C;
	this->field_38D58 = this->field_3912C;
}

CYyResourceFile* FFXI::CYy::CTsZoneMap::LoadZoneFile(int a2)
{
	this->field_54 = 1;
	this->dataindex = 0;
	this->field_50 = 0;
	this->field_3C = a2;
	LoadedZoneID = a2;

	if (a2 == FFXI::Constants::Enums::Zone::Davoi)
	{
		this->field_16C = 1.5;
		this->field_170 = 0.5;
	}
	else
	{
		this->field_16C = 2.0;
		this->field_170 = 2.0;
	}
	SomeFlag = true;

	CTsZoneMapSubStruct1* ss = this->SubStruct1s + this->dataindex;
	ss->ZoneID = a2;

	CYyResourceFile** resfile{ nullptr };
	if (this->field_39498) {
		int v6 = a2 + 0x9B97;
		if (a2 >= 700 || a2 >= 600)
			v6 = a2 + 0x14817;

		CYyDb::g_pCYyDb->pCMoResourceMng->LoadNumFile2(&resfile, v6);
	
		if (resfile != nullptr) {
			ss->field_58 = *resfile;
			CMoResource::DoTheThing(ss->field_58);
		}
		else
			ss->field_58 = nullptr;
	}

	int v10 = a2 + 100;
	if (a2 >= 700 || a2 >= 600)
		v10 = a2 + 0x144F7;

	CYyDb::g_pCYyDb->pCMoResourceMng->LoadNumFile2(&resfile, v10);
	if (resfile) {
		ss->ZoneDat = *resfile;
		CMoResource::DoTheThing(ss->ZoneDat);
	}	
	else
		ss->ZoneDat = nullptr;
		

	resfile = *CYyDb::g_pCYyDb->pCMoResourceMng->LoadNumFile2(&resfile, 0x1B8C);
	if (resfile != nullptr) {
		this->field_174 = *resfile;
		CMoResource::DoTheThing(this->field_174);
	}
	else
		this->field_174 = nullptr;


	this->field_44 = 0;
	
	{ //Inlined func
		this->SystemKage = CYyDb::g_pCYyDb->pCYyTexMng->GetByUnique(FFXI::Constants::Strings::TextureSystemKage);
		memset(&this->field_37508, 0, 0x38u);
		this->field_39128 = 0;
		this->field_3912C = 0;
		this->field_355B0 = 512.0;
		this->field_355B4 = (float)CYyDb::g_pCYyDb->GetBackgroundYRes();
		this->field_38E9C.Identity();
		FFXI::Math::WMatrix a3{};
		a3.Identity();
		a3._11 = 0.5;
		a3._22 = 0.5;
		this->field_38E9C.MatrixMultiply(&a3);
		a3.Identity();
		a3._41 = 0.5;
		a3._42 = 0.5;
		this->field_38E9C.MatrixMultiply(&a3);

		this->field_164 = this->field_355B0 * 0.5;
		this->field_168 = this->field_355B4 * 0.5;

		this->BigStruct.field_3413C = 0;
		this->BigStruct.field_34140 = 0;
		this->BigStruct.field_34144 = 0;
		this->BigStruct.field_1807C = 0;
		this->BigStruct.field_34148 = 0;
		this->BigStruct.field_3414C = 0;
		this->BigStruct.field_34150 = 0;
		this->BigStruct.field_34158 = 0;
		this->BigStruct.field_34154 = 0;
		this->BigStruct.field_3415C = 0;

		memset(&this->Material, 0, sizeof(this->Material));
		this->Material.Diffuse = { 1.0, 1.0, 1.0, 0.0 };
		this->Material.Ambient = { 0.7, 0.7, 0.7, 0.0 };
		this->Material.Specular = { 0.0, 0.0, 0.0, 0.0 };
		this->Material.Power = 50.0;

		this->RidManager.InitZonelines();
		this->RidManager.InitSubModels();

		this->field_3949C = this->field_394A0;
		if (XiZone::zone && this->field_3949C == 1) {
			int zoneid = XiZone::zone->ZoneID;
			if (zoneid == FFXI::Constants::Enums::Zone::Walk_of_Echoes || zoneid == 723 || zoneid == 742)
				this->field_3949C = 0;
		}
	}
	this->field_54 = 0;
	this->field_150 = 1;
	this->field_34 = 2;
	if (ss->ZoneDat != nullptr)
		this->AnotherSubStr.InitPath(ss->ZoneDat->PreHeader.PointerToThisInRPL);
	return ss->ZoneDat;
}

void FFXI::CYy::CTsZoneMap::PreDraw()
{
	switch (this->field_44) {
	case 0:
		if (CTsZoneMap::SomeFlag == false) {
			int v1 = this->BigStruct.field_4;
			if (v1 < 0) {
				v1 = 0;
			}
			CTsZoneMap::SomeFlag = Placeholder::gcZoneSubMapChangeSet(1, v1);
		}
		break;
	case 1: 
	{
		if (this->SubStruct1s[this->dataindex].field_0 != 0) {
			this->SubAndBig(this->dataindex);
		}
		this->field_54 = 1;
		this->field_40 = this->field_48;
		int v3 = this->field_40 + 100;
		if (this->field_40 >= 700 || this->field_40 >= 600) {
			v3 = this->field_40 + 83191;
		}
		this->field_4C = CYyDb::g_pCYyDb->pCMoResourceMng->GetOrLoadDatByIndex(v3);
		this->field_44 = 2;
	}
	case 2:
		if (CYyDb::g_pCYyDb->pCMoResourceMng->CheckResourceThing((CMoResource***) & this->field_4C) == true) {
			exit(0x10171D58);
		}
		break;
	default:
		break;
	}
}

void FFXI::CYy::CTsZoneMap::Draw()
{
	CDx* CDx = FFXI::CYy::CDx::instance;
	CDx->DXDevice->SetTexture(0, nullptr);
	CDx->DXDevice->SetTexture(1, nullptr);
	CDx->DXDevice->SetTexture(2, nullptr);
	CDx->DXDevice->SetTexture(3, nullptr);
	CDx->DXDevice->SetTexture(4, nullptr);
	CDx->DXDevice->SetTexture(5, nullptr);
	CDx->DXDevice->SetTexture(6, nullptr);
	CDx->DXDevice->SetTexture(7, nullptr);

	this->field_38D44 = 0;
	this->field_38D48 = 0;
	//sub //TODO //statics
	int v39 = 0;
	if (this->field_38D94 == 0) {
		CDx->DXDevice->CreateStateBlock(D3DSBT_ALL, &this->field_38D94);
		if (this->field_38D94 != -1) {
			CDx->DXDevice->BeginStateBlock();
			v39 = 1;
		}
	}
	
	if (this->field_38D94 == -1 || v39) {
		CDx->DXDevice->SetRenderState((D3DRENDERSTATETYPE)D3DRS_ZBIAS, CTsZoneMap::ZBias);
		CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_POINT);
		CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		CDx->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_POINT);
		CDx->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		CDx->DXDevice->SetTextureStageState(2, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_POINT);
		CDx->DXDevice->SetTextureStageState(2, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		CDx->DXDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
		CDx->DXDevice->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 2);
		CDx->DXDevice->SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, 3);
		CDx->DXDevice->SetTextureStageState(4, D3DTSS_TEXCOORDINDEX, 4);
		CDx->DXDevice->SetTextureStageState(5, D3DTSS_TEXCOORDINDEX, 5);
		CDx->DXDevice->SetTextureStageState(6, D3DTSS_TEXCOORDINDEX, 6);
		CDx->DXDevice->SetTextureStageState(7, D3DTSS_TEXCOORDINDEX, 7);
		CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		CDx->DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		CDx->DXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		CDx->DXDevice->SetRenderState(D3DRS_ALPHAREF, 0x60);
		CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		CDx->DXDevice->SetRenderState(D3DRS_LIGHTING, true);
		CDx->DXDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		CDx->DXDevice->SetRenderState(D3DRS_FOGENABLE, true);
		CDx->DXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
		CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		CDx->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		CDx->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		CDx->DXDevice->SetTextureStageState(2, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		CDx->DXDevice->SetTextureStageState(2, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE4X);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		CDx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		CDx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		CDx->DXDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
		CDx->DXDevice->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	}

	if (this->field_38D94 != -1) {
		if (v39 == 1)
			CDx->DXDevice->EndStateBlock(&this->field_38D94);
		else
			CDx->DXDevice->ApplyStateBlock(this->field_38D94);
	}

	if (this->field_3949C != 1) {
		if (CDx->field_92D) {
			CDx->DXDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
			CDx->DXDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
			CDx->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
		}
		else if (CDx->field_92E) {
			CDx->DXDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
			CDx->DXDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
			if (CDx->field_92F)
				CDx->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, true);
			else
				CDx->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
		}
	}
	else {
		CDx->DXDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		CDx->DXDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		if (CDx->field_92F)
			CDx->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, true);
		else
			CDx->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
	}

	if (Config::MainRegistryConfig::MIPMapping) {
		CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
		CDx->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
		if (this->field_3949C == 1)
			CDx->DXDevice->SetTextureStageState(2, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

	}
	else {
		CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, D3DTEXF_NONE);
		CDx->DXDevice->SetTextureStageState(1, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, D3DTEXF_NONE);
		if (this->field_3949C == 1)
			CDx->DXDevice->SetTextureStageState(2, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, D3DTEXF_NONE);
	}

	this->field_39114 = 0;
	this->field_39110 = 0;
	this->field_394A4 = 1;
	if (this->field_3949C == 1 || this->field_3949C == 2)
		this->field_394A4 = 0;
	this->field_39118 = 0;
	this->SetLightParams();
	CDx->DXDevice->LightEnable(this->field_394A8, true);
	CDx->DXDevice->LightEnable(this->field_394B0, true);
	CDx->DXDevice->LightEnable(this->field_394B4, true);
	CDx->DXDevice->LightEnable(this->field_394B8, true);
	CDx->DXDevice->LightEnable(this->field_394BC, true);
	if (this->field_394A4)
		CDx->DXDevice->LightEnable(this->field_394AC, true);
	else
		CDx->DXDevice->LightEnable(5, false);
	CDx->DXDevice->LightEnable(6, false);
	CDx->DXDevice->LightEnable(7, false);

	memset(&this->field_39180, 0, sizeof(this->field_39180));
	memset(&this->field_39260, 0, sizeof(this->field_39260));
	memset(&this->field_39340, 0, sizeof(this->field_39340));
	if (this->field_394A4) {
		memset(&this->field_391E8, 0, sizeof(this->field_391E8));
		memset(&this->field_392C8, 0, sizeof(this->field_392C8));
		memset(&this->field_393A8, 0, sizeof(this->field_393A8));
		XiZone::zone->GetTwoWeatherLights(&this->field_39180, &this->field_391E8);
		CDx->SetLight(this->field_394A8, &this->field_39180);
		CDx->SetLight(this->field_394AC, &this->field_391E8);
	}
	else {
		XiZone::zone->GetAreaLightByFourCC(&this->field_39180, 0, 0);
		CDx->SetLight(this->field_394A8, &this->field_39180);
	}
	XiZone::zone->GetAmbientByFourCC(&this->AmbientRenderState, 0, 0);
	CDx->DXDevice->SetRenderState(D3DRS_AMBIENT, this->AmbientRenderState);
	XiZone::zone->GetFogByFourCC(&this->FogColor, &this->FogEnd, &this->FogStart, 0, 0);
	if (this->field_3949C >= 1) {
		this->field_39540.x = this->field_39180.Direction.x;
		this->field_39540.y = this->field_39180.Direction.y;
		this->field_39540.z = this->field_39180.Direction.z;

		//Vertex shader constants
		D3DLIGHT8 v51 = this->field_39180;
		CDx->GammaAffectLight(&v51);

		float v23 = this->FogEnd - this->FogStart;
		float v25 = this->field_3954C / v23;

		this->VertexShaderConstants[0] = { 0.0, 0.0, 0.0, 0.0 };
		this->VertexShaderConstants[3] = { 0.5, 0.5, 0.5, 0.5 };
		this->VertexShaderConstants[5] = { 1.0, 1.0, 1.0, 1.0 };

		this->VertexShaderConstants[1] = { v51.Diffuse.r, v51.Diffuse.g, v51.Diffuse.b, 0.0 };
		this->VertexShaderConstants[4] = { v25, 0.0, this->FogEnd, 0.0 };
		
		XiZone::zone->GetSomeLight(this->VertexShaderConstants + 2);

		CDx->DXDevice->SetVertexShaderConstant(5, &this->VertexShaderConstants, sizeof(this->VertexShaderConstants) / sizeof(this->VertexShaderConstants[0]));

		this->field_3945C = 0;
		this->field_39460 = 0;
		this->field_39464 = 0;
		this->field_39468 = 0;
	}

	CDx->DXDevice->SetRenderState(D3DRS_FOGCOLOR, this->FogColor);
	CDx->DXDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*) (&this->FogStart)));
	CDx->DXDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*) (&this->FogEnd)));

	CDx->GetTransform(D3DTS_VIEW, &this->field_38D9C);
	this->field_38DDC = CYyDb::g_pCYyDb->field_154;
	CDx->SetTransform(D3DTS_PROJECTION, &this->field_38DDC);

	float v25 = XiZone::zone->GetDrawDistance(false);
	this->DrawDistance = v25;
	this->DrawDistanceSq = v25 * v25;
	this->field_39428 = XiZone::zone->GetAnotherSomething(false);

	this->field_355B0 = 512.0 / CYyDb::g_pCYyDb->field_2F0;
	this->field_164 = this->field_355B0 * 0.5;

	DoSomeCalc(&this->field_38E5C, this->field_355B0, this->field_355B4, CYyDb::g_pCYyDb->field_2F4, 0.2, this->DrawDistance);
	D3DXMatrixMultiply(&this->field_38FDC, &this->field_38D9C, &this->field_38E5C);
	D3DXMatrixMultiply(&this->field_38F5C, &this->field_38D9C, &this->field_38DDC);
	

	FFXI::Math::WMatrix v54{};
	v54.Identity();
	v54._11 = this->field_164;
	v54._22 = -this->field_168;
	D3DXMatrixMultiply(&this->field_38F9C, &v54, &this->field_38F5C);

	CDx->DXDevice->SetMaterial(&this->Material);
	CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;
	this->field_390F0 = cammng->Position;
	this->field_3911C = 0;
	this->field_3944C = 0;
	this->field_39448 = 0;
	this->PrimitiveCount = 0;
	this->field_39430 = 0;
	this->field_39134 = -1;
	this->MiniStructIndex = 0;

	for (int i = 0; i < sizeof(this->SubStruct1s) / sizeof(this->SubStruct1s[0]); ++i) {
		CTsZoneMapSubStruct1* ss = this->SubStruct1s + i;
		this->field_39138 = (XiArea*) - 1;
		if (ss->field_0) {
			this->field_39100 = 0;
			this->field_39104 = 0;
			this->field_39108 = 0;
			this->field_3910C = 0;
			this->field_3945C = 0;
			this->field_39460 = 0;
			this->field_39464 = 0;
			this->field_39468 = 0;
			CDx->DXDevice->LightEnable(this->field_394B0, false);
			CDx->DXDevice->LightEnable(this->field_394B4, false);
			CDx->DXDevice->LightEnable(this->field_394B8, false);
			CDx->DXDevice->LightEnable(this->field_394BC, false);
			CDx->DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			CDx->DXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
			this->field_39120 = 0;
			this->field_390FC = 0;
			this->field_39118 = 0;
			this->field_39110 = -1;
			int v28 = 0;
			if (ss->field_0 == 3 && this->field_158) {
				D3DXMatrixMultiply(&this->field_38EDC, &this->field_38E1C, &this->field_38D9C);
				this->field_38EDC.MatrixMultiply(&this->field_38E5C);
				v28 = 1;
			}
			if (this->field_3912C && ss->field_34) {
				throw "NOT IMPLEMENTED";
			}
			else if (ss->field_38) {
				this->field_3944C |= 2;
				int** someData = (int**)(ss->field_38 + 26);
				for (int j = 0; j < 4; ++j) {
					if (someData[j])
						this->DrawHelper(ss->field_64, someData[j], v28);
				}
			}
			else {
				for (unsigned int j = 0; j < ss->MapChunkCount; ++j) {
					MapChunk* chunk = ss->MapChunks + j;
					if (chunk->field_8 > 1) {
						if (chunk->field_F) {
							int v41 = this->MiniStructIndex;
							if (v41 < 512) {
								this->MiniStructs[v41].chunk = chunk;
								this->MiniStructs[v41].data = ss->field_64;
								this->MiniStructs[v41].field_8 = v28;
								++this->MiniStructIndex;
							}
						}
						else if (this->field_3949C = 2)
							this->RenderChunk(ss->field_64, chunk, v28);
						else 
							this->RenderChunk2(ss->field_64, chunk, v28);
						
					}
				}
			}

			if (this->MiniStructIndex > 0) {
				if (this->field_3949C == 2) {
					for (int k = 0; k < this->MiniStructIndex; ++k) {
						MiniStruct* ms = this->MiniStructs + k;
						this->RenderChunk(ms->data, ms->chunk, ms->field_8);
					}
				}
				else {
					for (int k = 0; k < this->MiniStructIndex; ++k) {
						MiniStruct* ms = this->MiniStructs + k;
						this->RenderChunk2(ms->data, ms->chunk, ms->field_8);
					}
				}
			}

			if (this->field_3949C == 1)
				CDx->DXDevice->SetPixelShader(NULL);

			if (this->field_39114) {
				CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
				CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
				CDx->DXDevice->SetRenderState((D3DRENDERSTATETYPE)D3DRS_ZBIAS, CTsZoneMap::ZBias);
			}
			if (ss->UnderscoreAtStructs != nullptr)
				;// exit(0x10175A7B);
		}
	}

	CDx->DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CDx->DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	CDx->DXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, D3DTEXF_NONE);

	CDx->DXDevice->LightEnable(0, false);
	CDx->DXDevice->LightEnable(1, false);
	CDx->DXDevice->LightEnable(2, false);
	CDx->DXDevice->LightEnable(3, false);
	CDx->DXDevice->LightEnable(4, false);
	CDx->DXDevice->LightEnable(5, false);
	CDx->DXDevice->LightEnable(6, false);
	CDx->DXDevice->LightEnable(7, false);

	CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	CDx->DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	CDx->DXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	CDx->DXDevice->SetRenderState(D3DRS_LIGHTING, true);
	CDx->DXDevice->SetRenderState(D3DRS_FOGENABLE, false);
	CDx->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
	CDx->DXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
	CDx->DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (this->field_3949C == 1)
		CDx->DXDevice->SetPixelShader(NULL);

	CDx->DXDevice->SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_CURRENT);
	CDx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CDx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CDx->DXDevice->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CDx->DXDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CDx->DXDevice->SetTextureStageState(3, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CDx->DXDevice->SetTextureStageState(3, D3DTSS_COLOROP, D3DTOP_DISABLE);
}

void FFXI::CYy::CTsZoneMap::DrawHelper(void* a2, int* a3, int a4)
{
	bool quit{ false };
	if (a4)
		quit = this->field_38EDC.SomeDrawCalc((D3DXVECTOR3*)a3);
	else
		quit = this->field_38FDC.SomeDrawCalc((D3DXVECTOR3*)a3);
	if (quit)
		return;

	MapChunk** chunks = (MapChunk**)a3[24];
	if (chunks) {
		int count = a3[25];
		for (int i = 0; i < count; ++i) {
			MapChunk* chunk = *(chunks + i);
			if (chunk->field_8 > 1) {
				if (chunk->field_F) {
					int MiniStructIndex = this->MiniStructIndex;
					if (MiniStructIndex < 512)
					{
						this->MiniStructs[MiniStructIndex].chunk = chunk;
						this->MiniStructs[this->MiniStructIndex].data = a2;
						this->MiniStructs[this->MiniStructIndex++].field_8 = a4;
					}
				}
				else if (this->field_3949C == 2) {
					this->RenderChunk(a2, chunk, a4);
				}
				else {
					this->RenderChunk2(a2, chunk, a4);
				}
			}
		}
	}
	else {
		int** v9 = (int**)(a3 + 26);
		for (int i = 0; i < 4; ++i) {
			if (v9[i])
				DrawHelper(a2, v9[i], a4);
		}
	}
}

void FFXI::CYy::CTsZoneMap::RenderChunk(void* a2, MapChunk* chunk, int a4)
{
	exit(0x10177340);
	FFXI::Math::WMatrix *v424{nullptr}, v426{}, v460{};
	if (a4) {
		v424 = &this->field_38E1C;
		D3DXMatrixMultiply(&v426, &chunk->field_40, &this->field_38E1C);
		v460 = v426;
		D3DXVECTOR3 v8{};
		v424->Vec3TransformDrop4(&v8, &chunk->Translation);
		D3DXVECTOR3 diff = v8 - this->field_390F0;
		double magsq = D3DXVec3LengthSq(&diff);
		if (chunk->field_B) {
			double val = this->field_39428 * chunk->field_D0;
			if (magsq > val) return;
		}
		else {
			if (magsq > this->DrawDistanceSq) return;
		}

		//this->field_38EDC, chunk->field_F4 //sub //TODO
	
	}
}

void FFXI::CYy::CTsZoneMap::RenderChunk2(void* a2, MapChunk* a3, int a4)
{
	CDx* CDx = FFXI::CYy::CDx::instance;
	FFXI::Math::WMatrix v142{}, v143{};
	double magsq{};
	//Maybe checking if the chunk is inside the draw distance?
	if (a4) {
		throw "does this happen";
		D3DXMatrixMultiply(&v142, &a3->field_40, &this->field_38E1C);
		v143 = v142;

		D3DXVECTOR3 v8{};
		this->field_38E1C.Vec3TransformDrop4(&v8, &a3->Translation);
		D3DXVECTOR3 diff = v8 - this->field_390F0;
		magsq = D3DXVec3LengthSq(&diff);
		if (a3->field_B) {
			double val = this->field_39428 * a3->field_D0;
			if (magsq > val) return;
		}
		else {
			if (magsq > this->DrawDistanceSq) return;
		}
		if (this->field_38EDC.SomeDrawCalc(a3->field_F4))
			return;
	}
	else {
		D3DXVECTOR3 diff = a3->Translation - this->field_390F0;
		magsq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
		
		if (a3->field_B) {
			double val = this->field_39428 * a3->field_D0;
			if (magsq > val) {
				return;
			}
		}
		else {
			if (magsq > this->DrawDistanceSq) {
				return;
			}
		}

		if (this->field_38FDC.SomeDrawCalc(a3->field_F4)) {
			return;
		}
	}

	MmbListNode* v35{nullptr};
	if (magsq <= a3->field_CC) {
		if (magsq < a3->field_C8)
			v35 = a3->field_18;
		else
			v35 = a3->field_14;
	}
	else {
		v35 = a3->field_10;
	}

	if (!v35)
		return;

	this->field_39448 += 1;
	if (a3->field_E4 != this->field_39100) { //skipped
		this->field_39100 = a3->field_E4;
		if (a3->field_E4 == 0) {
			CDx->DXDevice->LightEnable(this->field_394B0, false);
			this->field_3945C = nullptr;
		}
		else {
			SomeStruct* v42 = *(SomeStruct**)((char*)a2 + 76 * a3->field_E4 - 72);
			if (v42) {
				if (v42->field_8.Type) {
					if ((v42->field_0 & 0xFF) != 99) {
						this->field_3945C = &v42->field_8;
						CDx->DXDevice->LightEnable(this->field_394B0, true);
						CDx->SetLight(this->field_394B0, &v42->field_8);
					}
				}
			}
		}
	}

	if (a3->field_E8 != this->field_39104) { //skipped
		this->field_39104 = a3->field_E8;
		if (a3->field_E8 == 0) {
			CDx->DXDevice->LightEnable(this->field_394B4, false);
			this->field_39460 = nullptr;
		}
		else {
			SomeStruct* v42 = *(SomeStruct**)((char*)a2 + 76 * a3->field_E8 - 72);
			if (v42) {
				if (v42->field_8.Type) {
					if ((v42->field_0 & 0xFF) != 99) {
						this->field_39460 = &v42->field_8;
						CDx->DXDevice->LightEnable(this->field_394B4, true);
						CDx->SetLight(this->field_394B4, &v42->field_8);
					}
				}
			}
		}
	}

	if (a3->field_EC != this->field_39108) { //skipped
		this->field_39108 = a3->field_EC;
		if (a3->field_EC == 0) {
			CDx->DXDevice->LightEnable(this->field_394B8, false);
			this->field_39464 = nullptr;
		}
		else {
			SomeStruct* v42 = *(SomeStruct**)((char*)a2 + 76 * a3->field_EC - 72);
			if (v42) {
				if (v42->field_8.Type) {
					if ((v42->field_0 & 0xFF) != 99) {
						this->field_39464 = &v42->field_8;
						CDx->DXDevice->LightEnable(this->field_394B8, true);
						CDx->SetLight(this->field_394B8, &v42->field_8);
					}
				}
			}
		}
	}

	if (a3->field_F0 != this->field_3910C) { //skipped
		this->field_3910C = a3->field_F0;
		if (a3->field_F0 == 0) {
			CDx->DXDevice->LightEnable(this->field_394BC, false);
			this->field_39468 = nullptr;
		}
		else {
			SomeStruct* v42 = *(SomeStruct**)((char*)a2 + 76 * a3->field_F0 - 72);
			if (v42) {
				if (v42->field_8.Type) {
					if ((v42->field_0 & 0xFF) != 99) {
						this->field_39468 = &v42->field_8;
						CDx->DXDevice->LightEnable(this->field_394BC, true);
						CDx->SetLight(this->field_394BC, &v42->field_8);
					}
				}
			}
		}
	}

	int v54 = 0;
	int v123 = 0;
	switch (v35->field_1C) {
	case 0:
	case 1: //this boi 1
		CDx->DXDevice->SetVertexShader(D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ);
		if (this->field_3949C == 1) //skip
			CDx->DXDevice->SetPixelShader(0);
		break;
	case 2:
		CDx->DXDevice->SetVertexShader(this->VertexShader1);
		if (this->field_3949C == 1)
			CDx->DXDevice->SetPixelShader(0);
		break;
	case 3:
		throw "not implemented";
		break;
	default:
		break;
	}
	if (this->field_39134 != v54) {//v54 0
		this->field_39134 = v54;//enters into this
		this->field_39138 = (XiArea*)-1;
	}
	if (this->field_39138 != a3->field_E0) {
		this->field_39138 = a3->field_E0;//enters this
		if (a3->field_E0 != nullptr) {//does not enter this
			a3->field_E0->GetFog(&this->field_39334, &this->field_39338, &this->field_3933C, 0);
			FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGCOLOR, this->field_39334);
			if (v54 == 0) {
				if (this->field_394A4 != 0) {
					this->field_39138->GetTwoWeatherLights(&this->field_39260, &this->field_392C8);
					FFXI::CYy::CDx::instance->SetLight(this->field_394A8, &this->field_39260);
					FFXI::CYy::CDx::instance->SetLight(this->field_394AC, &this->field_392C8);
				}
				else {
					this->field_39138->GetLight(&this->field_39260, 0);
					FFXI::CYy::CDx::instance->SetLight(this->field_394A8, &this->field_39260);
				}
				TempLight = this->field_39260;
				this->field_39138->GetAmbient(&this->field_39330, 0);
				FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_AMBIENT, this->field_39330);
				FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)(&this->field_3933C)));
				FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)(&this->field_39338)));
			}
			else {
				a3->field_E0->GetLight(&this->field_39260, 0);
				D3DLIGHT8 l1 = this->field_39260;
				FFXI::CYy::CDx::instance->GammaAffectLight(&l1);
				this->field_39540 = this->field_39260.Direction;
				this->VertexShaderConstants[1].a = 0.0;
				this->VertexShaderConstants[1].r = l1.Diffuse.r;
				this->VertexShaderConstants[1].g = l1.Diffuse.g;
				this->VertexShaderConstants[1].b = l1.Diffuse.b;

				this->field_39138->GetSomeLight(&this->VertexShaderConstants[2]);

				this->VertexShaderConstants[4].r = this->field_3954C / (this->field_39338 - this->field_3933C);
				this->VertexShaderConstants[4].b = this->field_39338;

				FFXI::CYy::CDx::instance->DXDevice->GetVertexShaderConstant(5, this->VertexShaderConstants, 5);
				FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)(&this->field_3933C)));
				FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)(&this->field_39338)));
			}
		}
		else {
			D3DLIGHT8* v82 = &this->field_39180;
			if (v54) {//does not enter this
				this->field_39540 = this->field_39180.Direction;
				D3DLIGHT8 v134 = *v82;
				CDx->GammaAffectLight(&v134);
				this->VertexShaderConstants[1] = v134.Diffuse;
				this->VertexShaderConstants[1].a = 0.0;
				XiZone::zone->GetSomeLight(&this->VertexShaderConstants[2]);
				this->VertexShaderConstants[4].b = this->FogEnd;
				this->VertexShaderConstants[4].r = this->field_3954C / (this->FogEnd - this->FogStart);
				CDx->DXDevice->SetVertexShaderConstant(5, this->VertexShaderConstants, 5);
			}
			else {
				CDx->SetLight(this->field_394A8, &this->field_39180);
				TempLight = *v82;
				if (this->field_394A4)//enters this
					CDx->SetLight(this->field_394AC, &this->field_391E8);
				CDx->DXDevice->SetRenderState(D3DRS_AMBIENT, this->AmbientRenderState);
				//ambientrenderstate = 0x005e5f61 xx 0x00343754
			}
			CDx->DXDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)(&this->FogStart)));//0
			CDx->DXDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)(&this->FogEnd)));//43C88000
			CDx->DXDevice->SetRenderState(D3DRS_FOGCOLOR, this->FogColor);//005C6C78
		}
	}
	if (v54 == 0) {
		if (a4 != 0)
			CDx->SetTransform(D3DTS_WORLD, &v143);
		else 
			CDx->SetTransform(D3DTS_WORLD, &a3->field_40);
	}
	else {
		FFXI::Math::WMatrix v134{};
		FFXI::Math::WMatrix* v92{ nullptr };
		throw "not implemented";
		if (a4) {
			//sub //TODO
			D3DXMatrixMultiply(&v134, &v143, &this->field_38F5C);
		}
		else {
			D3DXMatrixMultiply(&v134, &a3->field_40, &this->field_38F5C);
			v92 = &a3->TransRotScaler;
		}
		//sub //TODO
	}
	if (this->field_39118 != a3->field_E) {//missed this one
		this->field_39118 = a3->field_E;
		CDx->DXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, a3->field_E != 0);
	}
	if (this->field_390FC != a3->field_D4) {//skipped this one
		this->field_390FC = a3->field_D4;
		if (a3->field_D4)
			CDx->DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		else
			CDx->DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		this->field_39120 = 0;
	}
	if (this->field_39110 != a3->field_D6) {//enters this
		this->field_39110 = a3->field_D6;
		if (a3->field_D6 == 0) {//enters this
			CDx->DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
			CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, 1);
			CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, 1);
		}
		else if (a3->field_D6 == 1) {
			CDx->DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
			CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, 1);
			CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, 1);
		}
		else if (a3->field_D6 == 2) {
			CDx->DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
			CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, 3);
			CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, 3);
		}
	}
	if (v35->field_68)//skips this
		this->UpdateKeyframe(a3);

	if (v35->field_44 <= 0)
		return;

	for (int i = 0; i < v35->field_44; ++i) {
		MmbStruct* mmb = v35->field_60 + i;
		if (mmb->ListCount <= 0) //first listcount 5
			continue;

		for (int j = 0; j < mmb->ListCount; ++j) {
			MmbStruct2* node = mmb->List + j;
			
			if (!v123)
				CDx->SetLight(this->field_394A8, &TempLight);//first enters here
			if (node->field_20 != this->field_39114) {//does not enter this
				this->field_39114 = node->field_20;
				if (node->field_20) {
					CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
					CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
					CDx->DXDevice->SetRenderState((D3DRENDERSTATETYPE)D3DRS_ZBIAS, CTsZoneMap::SomeInt2);
				}
				else {
					CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
					CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
					CDx->DXDevice->SetRenderState((D3DRENDERSTATETYPE)D3DRS_ZBIAS, CTsZoneMap::ZBias);
				}
			}
			if (node->field_24 != this->field_39120) {//skips this
				if (node->field_24)
					CDx->DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				else if (this->field_390FC)
					CDx->DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
				else
					CDx->DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			if (v123) {//skips this
				throw "not implemented";
				if (node->field_4) {
					IDirect3DBaseTexture8* tex = node->field_4->RegTex;
					if (!tex)
						tex = node->field_4->CubeTex;

					CDx->DXDevice->SetTexture(0, tex);

					if (!node->field_1C) {
						this->PrimitiveCount += node->PrimitiveCount;
					}
					else {
						if (node->field_B4) {
							if (node->field_BC) {
								throw "implement this";
							}
							else
								CDx->DXDevice->SetTexture(1, NULL);
							CDx->DXDevice->SetStreamSource(0, node->field_B4, 0x30);
							//sub //TODO
							++this->field_39430;
							this->PrimitiveCount += node->PrimitiveCount;
						}
					}
				}
				else {
					CDx->DXDevice->SetTexture(0, NULL);
					if (node->field_28) {
						if (node->field_1C) {
							if (node->field_B4) {
								CDx->DXDevice->SetTexture(1, NULL);
								CDx->DXDevice->SetStreamSource(0, node->field_B4, 0x30);
								//sub //TODO
								++this->field_39430;
							}
						}
						this->PrimitiveCount += node->PrimitiveCount;
					}
				}
			}
			else {
				if (node->field_4) {//enters this
					IDirect3DBaseTexture8* tex = node->field_4->RegTex;
					if (!tex)
						tex = node->field_4->CubeTex;
					CDx->DXDevice->SetTexture(0, tex);
				}
				else {
					CDx->DXDevice->SetTexture(0, NULL);
				}
				if (v35->field_1C == 2) {//skips this
					int buffindex = v35->field_6C * a3->field_15C;
					IDirect3DVertexBuffer8* v138 = node->Buffers[buffindex];
					if (v138) {
						CDx->DXDevice->SetStreamSource(0, v138, 0xC);
						CDx->DXDevice->SetStreamSource(1, node->VertexBuffer, 0x18);
						CDx->DXDevice->SetIndices(node->IndexBuffer, 0);
						CDx->DrawStreamIndexed(v35->field_2C, 0, node->VerticeCount, 0, node->PrimitiveCount);
						this->PrimitiveCount += node->PrimitiveCount;
					}
				} 
				else if (node->field_28 != nullptr) {//enters this
					CDx->DXDevice->SetStreamSource(0, node->field_28, v35->field_24);
					CDx->DrawStream(v35->field_2C, 0, node->PrimitiveCount);
					//20b prim count, type 5
					++this->field_39430;
					this->PrimitiveCount += node->PrimitiveCount;
				}
			}
		}
	}
}

void FFXI::CYy::CTsZoneMap::DoD3mDraw(FFXI::Math::WMatrix* a2, MmbListNode* a3, char a4, bool a5)
{
	CDx* cdx = FFXI::CYy::CDx::instance;
	FFXI::Math::WMatrix v13{};
	D3DXMatrixMultiply(&v13, a2, &this->field_38E5C);
	FFXI::Math::WMatrix v14 = v13;
	int something = v14.SomeDrawCalc2(a3->field_70);
	if (something != 0) {
		return;
	}

	v13 = *a2;
	v13._41 += a3->field_E0.x;
	v13._42 += a3->field_E0.y;
	v13._43 += a3->field_E0.z;
	v13._44 += a3->field_E0.w;
	
	cdx->SetTransform(D3DTS_WORLD, &v13);
	
	cdx->DXDevice->SetRenderState((D3DRENDERSTATETYPE)D3DRS_ZBIAS, CTsZoneMap::SomeInt3);
	cdx->DXDevice->SetVertexShader(a3->VertexFVF);
	cdx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	cdx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	
	int v5 = -1;
	if (a3->field_60 != nullptr) {
		if (a3->field_44 > 0) {
			for (int i = 0; i < a3->field_44; ++i) {
				MmbStruct* mmbs = a3->field_60 + i;
				if (mmbs->ListCount > 0) {
					for (int j = 0; j < mmbs->ListCount; ++j) {
						MmbStruct2* mmbs2 = mmbs->List + j;
						if (mmbs2->field_28 != nullptr) {
							if (mmbs2->field_4 != nullptr) {
								if (v5 != 0) {
									v5 = 0;
									cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
									cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
									if (a4 != 0) {
										cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
										cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE4X);
										cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
										cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TFACTOR);
										cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE4X);
										cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
										cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
										cdx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
									}
									else {
										cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
										cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE4X);
										cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
										cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

										cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
										cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
										cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

										cdx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE4X);
										cdx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
										cdx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
									}
									cdx->DXDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
									cdx->DXDevice->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
								}
								
								IDirect3DBaseTexture8* tex = mmbs2->field_4->RegTex;
								if (tex == nullptr) {
									tex = mmbs2->field_4->CubeTex;
								}
								cdx->DXDevice->SetTexture(0, tex);
							}
							else {
								if (v5 != 1) {
									v5 = 1;
									cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
									cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
									cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
									cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE4X);
									cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
									cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
									cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTA_CURRENT);
									cdx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTA_CURRENT);
								}
								cdx->DXDevice->SetTexture(0, nullptr);
							}
							this->field_38D48 += mmbs2->PrimitiveCount;
							cdx->DXDevice->SetStreamSource(0, mmbs2->field_28, a3->field_24);
							cdx->DrawStream(a3->field_2C, 0, mmbs2->PrimitiveCount);
						}
					}
				}
			}
		}

		if (v5 != 0) {
			cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
			cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		}

		cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE4X);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
		cdx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		cdx->DXDevice->SetRenderState((D3DRENDERSTATETYPE)D3DRS_ZBIAS, CTsZoneMap::SomeInt5);
	}
}

MmbListNode* FFXI::CYy::CTsZoneMap::FindMmb(char* a2)
{
	for (int i = 0; i < sizeof(this->SubStruct1s) / sizeof(this->SubStruct1s[0]); ++i) {
		CTsZoneMapSubStruct1* ss = this->SubStruct1s + i;
		if (ss->field_0) {
			MmbListNode* node = ss->MMBManager.GetByName(a2 + 16, true);
			if (node)
				return node;
		}
	}
	return nullptr;
}

void FFXI::CYy::CTsZoneMap::UpdateKeyframe(MapChunk* a2)
{
	if (!a2->field_154) {
		a2->field_154 = 1;
		a2->field_158 = (float)(rand() % 300);
		int curid = XiZone::zone->GetCurrentWeatherID();
		a2->field_160 = this->WeatherData.GetRandKeyframe(curid);
	}

	//If keyframe is valid
	if (a2->field_160 && *a2->field_160) {
		float v18 = a2->field_158 / 300;
		float v6 = 1.0 - (*a2->field_160)->GetFrameValue(v18);
		if (v6 < 0.0)
			v6 = 0.0;
		a2->field_15C = v6;
	}
	else
		a2->field_15C = 0;

	float v11 = CYyDb::g_pCYyDb->CheckTick() + a2->field_158;
	if (v11 <= 300.0)
		a2->field_158 = v11;
	else {
		int curid = XiZone::zone->GetCurrentWeatherID();
		a2->field_160 = this->WeatherData.GetRandKeyframe(curid);
		a2->field_158 = v11 - 300.0;
	}
}

void FFXI::CYy::CTsZoneMap::CleanSubstruct(int a2)
{
	CTsZoneMapSubStruct1* ss = this->SubStruct1s + a2;
	if (ss->field_0 == 0) return;

	if (ss->MapChunks) {
		CYyObject::Unwrap((char*)ss->MapChunks);
		ss->MapChunks = nullptr;
	}

	if (ss->UnderscoreAtStructs) {
		delete[] ss->UnderscoreAtStructs;
		ss->UnderscoreAtStructs = nullptr;
	}

	ss->MMBManager.Clean(nullptr);
	ss->CleanFourCCList();
	CleanField64(ss->field_64);
	if (ss->ZoneDat) {
		ss->ZoneDat->StopMovers();
		ss->ZoneDat->SomeClean3();
		CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(ss->ZoneDat->PreHeader.PointerToThisInRPL);
		ss->ZoneDat = nullptr;
	}
	if (this->field_39498) {
		if (ss->field_58) {
			ss->field_58->SomeClean3();
			CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(ss->field_58->PreHeader.PointerToThisInRPL);
			ss->field_58 = nullptr;
		}
	}
	memset(ss, 0, sizeof(CTsZoneMapSubStruct1));
}

void FFXI::CYy::CTsZoneMap::SubAndBig(int a2)
{
	if (this->SubStruct1s[a2].field_C != 0) {
		exit(0x10172001);
	}
	this->CleanSubstruct(a2);
	this->field_50 = 0;
}

char FFXI::CYy::CTsZoneMap::OnMove()
{
	if (this->field_34 == 0) {
		this->InitBumpMappingShaders();
		this->field_394A0 = this->field_3949C;
		this->InitSomeStatics();
		this->CreateVertexShaders();
		this->field_34 = 1;
	}
	else if (this->field_34 == 2) {
		if (CTsZoneMap::EventFlag || this->AnotherSubStr.field_C == 0.0) {
			CMoAttachments::AttachMatrix.Identity();
			this->field_158 = 0;
		}
		else {
			float v9{ 0.0 };
			if (CTsZoneMap::SomeTiming >= 0) {
				v9 = CTsZoneMap::SomeTiming;
			}
			this->AnotherSubStr.field_8 = v9;
			this->AnotherSubStr.field_10 = CTsZoneMap::ZoneFlag;

			exit(0x101720D4);
		}
		
		this->PreDraw();

		if (this->field_150)
			this->Draw();

	}

	//sub //TODO
	return true;
}

unsigned char initDecryptTable[] =
{
  0xE2, 0xE5, 0x06, 0xA9, 0xED, 0x26, 0xF4, 0x42, 0x15, 0xF4,
  0x81, 0x7F, 0xDE, 0x9A, 0xDE, 0xD0, 0x1A, 0x98, 0x20, 0x91,
  0x39, 0x49, 0x48, 0xA4, 0x0A, 0x9F, 0x40, 0x69, 0xEC, 0xBD,
  0x81, 0x81, 0x8D, 0xAD, 0x10, 0xB8, 0xC1, 0x88, 0x15, 0x05,
  0x11, 0xB1, 0xAA, 0xF0, 0x0F, 0x1E, 0x34, 0xE6, 0x81, 0xAA,
  0xCD, 0xAC, 0x02, 0x84, 0x33, 0x0A, 0x19, 0x38, 0x9E, 0xE6,
  0x73, 0x4A, 0x11, 0x5D, 0xBF, 0x85, 0x77, 0x08, 0xCD, 0xD9,
  0x96, 0x0D, 0x79, 0x78, 0xCC, 0x35, 0x06, 0x8E, 0xF9, 0xFE,
  0x66, 0xB9, 0x21, 0x03, 0x20, 0x29, 0x1E, 0x27, 0xCA, 0x86,
  0x82, 0xE6, 0x45, 0x07, 0xDD, 0xA9, 0xB6, 0xD5, 0xA2, 0x03,
  0xEC, 0xAD, 0x62, 0x45, 0x2D, 0xCE, 0x79, 0xBD, 0x8F, 0x2D,
  0x10, 0x18, 0xE6, 0x0A, 0x6F, 0xAA, 0x6F, 0x46, 0x84, 0x32,
  0x9F, 0x29, 0x2C, 0xC2, 0xF0, 0xEB, 0x18, 0x6F, 0xF2, 0x3A,
  0xDC, 0xEA, 0x7B, 0x0C, 0x81, 0x2D, 0xCC, 0xEB, 0xA1, 0x51,
  0x77, 0x2C, 0xFB, 0x49, 0xE8, 0x90, 0xF7, 0x90, 0xCE, 0x5C,
  0x01, 0xF3, 0x5C, 0xF4, 0x41, 0xAB, 0x04, 0xE7, 0x16, 0xCC,
  0x3A, 0x05, 0x54, 0x55, 0xDC, 0xED, 0xA4, 0xD6, 0xBF, 0x3F,
  0x9E, 0x08, 0x93, 0xB5, 0x63, 0x38, 0x90, 0xF7, 0x5A, 0xF0,
  0xA2, 0x5F, 0x56, 0xC8, 0x08, 0x70, 0xCB, 0x24, 0x16, 0xDD,
  0xD2, 0x74, 0x95, 0x3A, 0x1A, 0x2A, 0x74, 0xC4, 0x9D, 0xEB,
  0xAF, 0x69, 0xAA, 0x51, 0x39, 0x65, 0x94, 0xA2, 0x4B, 0x1F,
  0x1A, 0x60, 0x52, 0x39, 0xE8, 0x23, 0xEE, 0x58, 0x39, 0x06,
  0x3D, 0x22, 0x6A, 0x2D, 0xD2, 0x91, 0x25, 0xA5, 0x2E, 0x71,
  0x62, 0xA5, 0x0B, 0xC1, 0xE5, 0x6E, 0x43, 0x49, 0x7C, 0x58,
  0x46, 0x19, 0x9F, 0x45, 0x49, 0xC6, 0x40, 0x09, 0xA2, 0x99,
  0x5B, 0x7B, 0x98, 0x7F, 0xA0, 0xD0
};