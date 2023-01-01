#include "MmbManager.h"
#include "MmbListNode.h"
#include "CYyObject.h"
#include "RegistryConfig.h"
#include "MmbStruct.h"
#include "MmbStruct2.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CYyTexMng.h"
#include "CDx.h"
#include "CTsZoneMap.h"
#include <iostream>
#include "PrimMng.h"
#include "TkManager.h"
#include "_49SubList.h"
#include "_49SubListNode.h"
#include "CTkNode.h"
#include "CIwTopMenu.h"
#include "CTkMenuCtrlData.h"
#include "CTkDrawCtrlFrame.h"
using namespace FFXI::CYy;

unsigned char initDecrypt[] =
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

unsigned char initDecrypt2[] =
{
  0xB8, 0xC5, 0xF7, 0x84, 0xE4, 0x5A, 0x23, 0x7B, 0xC8, 0x90,
  0x1D, 0xF6, 0x5D, 0x09, 0x51, 0xC1, 0x07, 0x24, 0xEF, 0x5B,
  0x1D, 0x73, 0x90, 0x08, 0xA5, 0x70, 0x1C, 0x22, 0x5F, 0x6B,
  0xEB, 0xB0, 0x06, 0xC7, 0x2A, 0x3A, 0xD2, 0x66, 0x81, 0xDB,
  0x41, 0x62, 0xF2, 0x97, 0x17, 0xFE, 0x05, 0xEF, 0xA3, 0xDC,
  0x22, 0xB3, 0x45, 0x70, 0x3E, 0x18, 0x2D, 0xB4, 0xBA, 0x0A,
  0x65, 0x1D, 0x87, 0xC3, 0x12, 0xCE, 0x8F, 0x9D, 0xF7, 0x0D,
  0x50, 0x24, 0x3A, 0xF3, 0xCA, 0x70, 0x6B, 0x67, 0x9C, 0xB2,
  0xC2, 0x4D, 0x6A, 0x0C, 0xA8, 0xFA, 0x81, 0xA6, 0x79, 0xEB,
  0xBE, 0xFE, 0x89, 0xB7, 0xAC, 0x7F, 0x65, 0x43, 0xEC, 0x56,
  0x5B, 0x35, 0xDA, 0x81, 0x3C, 0xAB, 0x6D, 0x28, 0x60, 0x2C,
  0x5F, 0x31, 0xEB, 0xDF, 0x8E, 0x0F, 0x4F, 0xFA, 0xA3, 0xDA,
  0x12, 0x7E, 0xF1, 0xA5, 0xD2, 0x22, 0xA0, 0x0C, 0x86, 0x8C,
  0x0A, 0x0C, 0x06, 0xC7, 0x65, 0x18, 0xCE, 0xF2, 0xA3, 0x68,
  0xFE, 0x35, 0x96, 0x95, 0xA6, 0xFA, 0x58, 0x63, 0x41, 0x59,
  0xEA, 0xDD, 0x7F, 0xD3, 0x1B, 0xA8, 0x48, 0x44, 0xAB, 0x91,
  0xFD, 0x13, 0xB1, 0x68, 0x01, 0xAC, 0x3A, 0x11, 0x78, 0x30,
  0x33, 0xD8, 0x4E, 0x6A, 0x89, 0x05, 0x7B, 0x06, 0x8E, 0xB0,
  0x86, 0xFD, 0x9F, 0xD7, 0x48, 0x54, 0x04, 0xAE, 0xF3, 0x06,
  0x17, 0x36, 0x53, 0x3F, 0xA8, 0x11, 0x53, 0xCA, 0xA1, 0x95,
  0xC2, 0xCD, 0xE6, 0x1F, 0x57, 0xB4, 0x7F, 0xAA, 0xF3, 0x6B,
  0xF9, 0xA0, 0x27, 0xD0, 0x09, 0xEF, 0xF6, 0x68, 0x73, 0x60,
  0xDC, 0x50, 0x2A, 0x25, 0x0F, 0x77, 0xB9, 0xB0, 0x04, 0x0B,
  0xE1, 0xCC, 0x35, 0x31, 0x84, 0xE6, 0x22, 0xF9, 0xC2, 0xAB,
  0x95, 0x91, 0x61, 0xD9, 0x2B, 0xB9, 0x72, 0x4E, 0x10, 0x76,
  0x31, 0x66, 0x0A, 0x0B, 0x2E, 0x83
};
int counter = 0;
//Local FUNCs
void Decrypt2(int a1, int a2, int a3, int* a4) {
	if ((a1 & 0xFFFF0000) != 0xFFFF0000) return;
	int decrypt = initDecrypt2[a3];
	int v5 = (a2 - 8) & 0xFFFFFFF0;
	int v8 = v5 / 2;
	if (v8 > 0) {
		int v4 = a3;
		for (int i = 0; i < v8; i += 8) {
			if (decrypt & 1) {
				int v10 = a4[i / 4];
				int* v11 = (int*)((char*)&a4[i / 4] + v8);
				a4[i / 4] = *v11;
				int v12 = v11[1];
				*v11 = v10;
				int v13 = a4[i / 4 + 1];
				a4[i / 4 + 1] = v12;
				v11[1] = v13;
			}
			v4 += 9;
			decrypt += v4;
		}
	}
}
//~Local FUNCs
MmbListNode* FFXI::CYy::MmbManager::Create()
{
	MmbListNode* node = (MmbListNode*)CYyObject::Wrap(sizeof(MmbListNode), FFXI::Constants::Enums::MEM_MODE::Ex);
	
	memset(node, 0, sizeof(MmbListNode));
	if (this->ListCount) {
		this->ListTail->NextNode = node;
		this->ListTail = node;
	}
	else {
		this->ListHead = node;
		this->ListTail = node;
	}
	this->ListCount += 1;
	return node;
}
FFXI::CYy::MmbManager::MmbManager()
{
	this->ListHead = nullptr;
	this->ListTail = nullptr;
	this->ListCount = 0;
}

FFXI::CYy::MmbManager::~MmbManager()
{
	this->ListHead = nullptr;
	this->ListTail = nullptr;
	this->ListCount = 0;
}

MmbListNode* FFXI::CYy::MmbManager::GetByName(const char* a2, bool a3)
{
	MmbListNode* result = this->ListHead;
	if (a3) {
		while (result) {
			if ((result->field_DC & 0xFF00) != 0
				&& !strncmp(a2, result->name, sizeof(result->name)))
				return result;
			result = result->NextNode;
		}
		result = this->ListHead;
		while (result) {
			if ((result->field_DC & 0xFF00) == 0
				&& !strncmp(a2, result->name, sizeof(result->name)))
				return result;
			result = result->NextNode;
		}
	}
	if (!result) return nullptr;
	while (strncmp(a2, result->name, 0x10)) {
		result = result->NextNode;
		if (!result)
			return nullptr;
	}
	return result;
}

MmbListNode* FFXI::CYy::MmbManager::AddFromData(CYyResourceFile** a2, int* a3, int a4, CYyResourceFile* a5, int a6, D3DXVECTOR4* a7, int a8)
{
	CDx* CDx = FFXI::CYy::CDx::instance;
	//Decrypt
	int v8 = *((unsigned char*)a3 + 3);
	int v9 = a3[0] & 0xFFFFFF;
	int index = *((unsigned char*)a3 + 5) ^ 0xF0;
	if (v8 >= 5) {
		int v10 = v9 - 8;
		unsigned char* v11 = (unsigned char*)(a3 + 2);
		int decrypt = initDecrypt[index];
		int v13 = 0;
		if (v10 > 0) {
			for (int i = 0; i < v10; ++i) {
				int v16 = v13 + 1;
				int v18 = decrypt + v16;
				int v19 = ((unsigned char)decrypt | ((unsigned char)decrypt << 8)) >> (v18 & 7);
				v13 = v16 + 1;
				decrypt = v13 + v18;
				v11[i] ^= v19;
			}
		}
	}

	MmbListNode* node = this->Create();
	if (!a8)
		node->field_DD = 1;
	node->data = a3;
	node->field_E0 = *a7;
	node->field_40 = 0;
	node->field_68 = 0;
	node->VertexFVF = D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEXTUREFORMAT4;
	node->field_1C = (unsigned char)a3[1];
	if (node->field_1C == 1) {
		node->field_2C = D3DPT_TRIANGLESTRIP;
		node->field_24 = 36;
		node->field_28 = 36;
		node->field_38 = 36;
		node->field_3C = 5;
	}
	else if (node->field_1C == 2) {
		node->field_24 = 36;
		node->field_28 = 48;
		node->field_2C = D3DPT_TRIANGLELIST;
		node->field_38 = 36;
		node->field_3C = 4;
		if (Config::MainRegistryConfig::EnvironmentalAnimationQuality == 0) {
			node->field_64 = 1;
			node->field_68 = 0;
			node->field_6C = 0.0;
		}
		else if (Config::MainRegistryConfig::EnvironmentalAnimationQuality == 2) {
			node->field_64 = 32;
			node->field_68 = 1;
			node->field_6C = 31.0;
		}
		else {
			node->field_64 = 16;
			node->field_68 = 1;
			node->field_6C = 15.0;
		}
	}
	else {
		node->field_2C = D3DPT_TRIANGLELIST;
		node->field_24 = 36;
		node->field_28 = 36;
		node->field_38 = 36;
		node->field_3C = 4;
	}

	node->field_30 = 0x144;
	node->field_34 = 28;
	Decrypt2(a3[1], v9, index, a3 + 2);
	int byteCount = a3[0] & 0xFFFFFF;
	byteCount -= 8;
	int sum = 0;
	unsigned char* ok = (unsigned char*)(a3 + 2);
	for (int i = 0; i <= byteCount; ++i) {
		sum += ok[i];
	}
	memcpy_s(node->name, sizeof(node->name), node->data + 4, sizeof(node->name));
	/*std::cout << "MMB: ";
	for (int z=0;z<strlen(node->name) ;++z)
		std::cout << node->name[z];
	std::cout << std::endl;*/
	node->field_18 = a4;
	node->field_44 = a3[8];
	memcpy_s(node->field_48, sizeof(node->field_48), a3 + 9, sizeof(node->field_48));
	
	int v29 = 9;
	if (v8 >= 2)
		v29 = node->field_44;

	node->field_70[0].x = node->field_48[0];
	node->field_70[0].y = node->field_48[2];
	node->field_70[0].z = node->field_48[5];
	node->field_70[1].x = node->field_48[1];
	node->field_70[1].y = node->field_48[2];
	node->field_70[1].z = node->field_48[5];
	node->field_70[2].x = node->field_48[1];
	node->field_70[2].y = node->field_48[2];
	node->field_70[2].z = node->field_48[4];
	node->field_70[3].x = node->field_48[0];
	node->field_70[3].y = node->field_48[2];
	node->field_70[3].z = node->field_48[4];
	node->field_70[4].x = node->field_48[0];
	node->field_70[4].y = node->field_48[3];
	node->field_70[4].z = node->field_48[5];
	node->field_70[5].x = node->field_48[1];
	node->field_70[5].y = node->field_48[3];
	node->field_70[5].z = node->field_48[5];
	node->field_70[6].x = node->field_48[1];
	node->field_70[6].y = node->field_48[3];
	node->field_70[6].z = node->field_48[4];
	node->field_70[7].x = node->field_48[0];
	node->field_70[7].y = node->field_48[3];
	node->field_70[7].z = node->field_48[4];

	double v31 = node->field_48[3] - node->field_48[2];
	double v32 = node->field_48[5] - node->field_48[4];
	double v33 = node->field_48[1] - node->field_48[0];
	double v34 = v32 * v32 + v31 * v31 + v33 * v33;
	if (v34 == 0.0)
		node->field_D4 = 0.0;
	else
		node->field_D4 = sqrt(v34) * 0.5;

	MmbStruct* objlist = (MmbStruct*)CYyObject::Wrap(sizeof(MmbStruct) * v29, FFXI::Constants::Enums::MEM_MODE::Ex);
	node->field_60 = objlist;

	if (CYyDb::g_pCYyDb->g_pTsZoneMap->field_39498) {
		throw "BUMP MAPPING NOT IMPLEMENTED";
	}
	
	for (int i = 0; i < v29; ++i) {
		MmbStruct* ms = node->field_60 + i;
		int v40 = a3[i + 15];
		if (v40) {
			int* v121 = (int*)((char*)node->data + v40);
			ms->ListCount = v121[0];
			ms->List = (MmbStruct2*)CYyObject::Wrap(sizeof(MmbStruct2) * ms->ListCount, FFXI::Constants::Enums::MEM_MODE::Ex);
			memcpy_s(ms->field_8, sizeof(ms->field_8), v121 + 1, sizeof(ms->field_8));
			ms->field_20 = v121[7];
			if (ms->ListCount > 0) {
				int* v112 = v121 + 8;
				for (int j = 0; j < ms->ListCount; ++j) {
					MmbStruct2* ms2 = ms->List + j;
					CYyResourceFile* v47{ nullptr };
					int* v46 = v112;
					char v118[17];
					memcpy_s(v118, 17, v112, 16);
					v118[16] = 0;

					if (a2)
						v47 = *a2;

					CYyTex* v48 = CYyDb::g_pCYyDb->pCYyTexMng->FindTextureUnder(v118, (CMoResource*)v47);
					if (!v48)
						v48 = CYyDb::g_pCYyDb->pCYyTexMng->FindTexture(v118);
					ms2->field_4 = v48;

					if (CYyDb::g_pCYyDb->g_pTsZoneMap->field_39498 && a5 && node->field_1C == 3) {
						//sub //TODO
						throw "bump mapping not implemented";
					}
					ms2->VerticeCount = v46[4] & 0x0FFFFFFF;
					ms2->field_C = (float*)(v46 + 5);
					ms2->field_20 = (v46[4] & 0x80000000) != 0;
					ms2->field_22 = (v46[4] & 0x40000000) != 0;
					ms2->field_24 = (v46[4] & 0x20000000) != 0;
					ms2->field_26 = (v46[4] & 0x10000000) != 0;
					
					int v50 = node->field_28 * ms2->VerticeCount;
					char* v52 = (char*)v46 + v50 + 20;
					int v51 = *(int*)v52;
					
					ms2->IndexCount = v51;
					ms2->IndexData = (unsigned short*)(v52 + 4);

					int v54{}, v55{};
					if (node->field_1C == 1 || node->field_1C == 3) {
						v54 = ms2->IndexCount;
						v55 = v54 - 2;
					}
					else {
						v54 = v51;
						v55 = v51 / 3;
					}

					ms2->PrimitiveCount = v55;
					ms2->field_28 = 0;
					ms2->field_2C = 0;
					v112 = (int*)(v52 + 4 + 2 * ((v54 + 1) & 0xFFFFFFFE));
					ms2->IndexBuffer = 0;
					ms2->VertexBuffer = 0;
					ms2->field_B4 = 0;
					memset(ms2->Buffers, 0, sizeof(ms2->Buffers));
					if (ms2->VerticeCount) {
						int v59 = node->field_1C;
						if (v59 == 2) {
							if (CDx->DXDevice->CreateVertexBuffer(
								24 * ms2->VerticeCount,
								CDx->VertexBufferUsage,
								0,
								CDx->VertexBufferPool,
								&ms2->VertexBuffer
							) >= D3D_OK) {
								node->field_40 += 24 * ms2->VerticeCount;
								DWORD* v106{ nullptr };
								while (ms2->VertexBuffer->Lock(0, 0, (BYTE**)&v106, 0) < D3D_OK)
									;
								if (ms2->VerticeCount > 0) {
									float* v61 = ms2->field_C + 6;
									for (int k = 0; k < ms2->VerticeCount; ++k) {
										memcpy_s(v106, 24, v61, 24);
										v106 += 6;
										v61 += 12;
									}
								}
								ms2->VertexBuffer->Unlock();
							}

							for (int k = 0; k < node->field_64; ++k) {
								IDirect3DVertexBuffer8** buff = ms2->Buffers + k;
								if (CDx->DXDevice->CreateVertexBuffer(
									12 * ms2->VerticeCount,
									CDx->VertexBufferUsage,
									0,
									CDx->VertexBufferPool,
									buff) >= D3D_OK) {
									node->field_40 += 12 * ms2->VerticeCount;
									float* v108{ nullptr };
									if ((*buff)->Lock(0, 0, (BYTE**)&v108, 0) < D3D_OK) {
										(*buff)->Release();
										*buff = nullptr;
									}
									else {
										double v65{ 0.0 };
										if (k)
											v65 = (double)k / node->field_6C;
										float* v66 = ms2->field_C;
										for (int l = 0; l < ms2->VerticeCount; ++l) {
											v108[0] = v65 * v66[3] + v66[0];
											v108[1] = v65 * v66[4] + v66[1];
											v108[2] = v65 * v66[5] + v66[2];
											v66 += 12;
											v108 += 3;
										}
										(*buff)->Unlock();
									}
								}
								else
									*buff = nullptr;
							}

							if (CDx->DXDevice->CreateIndexBuffer(
								2 * ms2->IndexCount,
								CDx->VertexBufferUsage,
								D3DFMT_INDEX16,
								CDx->VertexBufferPool,
								&ms2->IndexBuffer) >= D3D_OK) {
								node->field_40 += 2 * ms2->IndexCount;
								char* v116{ nullptr };
								while (ms2->IndexBuffer->Lock(0, 0, (BYTE**)&v116, 0) < D3D_OK)
									;
								memcpy_s(v116, 2 * ms2->IndexCount, ms2->IndexData, 2 * ms2->IndexCount);
								ms2->IndexBuffer->Unlock();
							}
						}
						else if (v59 == 3) {
							throw "not implemented";
							if (CDx->DXDevice->CreateVertexBuffer(
								ms2->IndexCount * node->field_24,
								CDx->VertexBufferUsage,
								node->VertexFVF,
								CDx->VertexBufferPool,
								&ms2->field_28) >= D3D_OK) {
								node->field_40 += node->field_24 * ms2->IndexCount;
								int* v104{ nullptr };
								while (ms2->field_28->Lock(0, 0, (BYTE**)&v104, 0) < D3D_OK)
									;
								for (int k = 0; k < ms2->IndexCount; ++k) {
									unsigned short v73 = ms2->IndexData[k];
									memcpy_s(v104, 0x24, ms2->field_C + 9 * ms2->IndexData[k], 0x24);
									v104 += 9;
								}
								ms2->field_28->Unlock();
							}
							int v123 = 0;
							if (ms2->IndexCount > 2) {
								unsigned short* v78 = ms2->IndexData + 2;
								int v76 = ms2->IndexData[0];
								int v77 = ms2->IndexData[1];

								for (int k = 2; k < ms2->IndexCount; ++k) {
									unsigned short v80 = *v78;
									if (v76 != v77 && v77 != v80 && v80 != v76)
										++v123;
									++v78;
									v76 = v77;
									v77 = v80;
								}
							}
							ms2->field_1C = v123;
							if (v123) {
								throw "NOT FINISHED";
								if (CDx->DXDevice->CreateVertexBuffer(
									144 * v123,
									CDx->VertexBufferUsage,
									0,
									CDx->VertexBufferPool,
									&ms2->field_B4) >= D3D_OK) {
									node->field_40 += 144 * v123;
									float* v105{ nullptr };
									while (ms2->field_B4->Lock(0, 0, (BYTE**)&v105, 0) < D3D_OK)
										;
									if (ms2->IndexCount > 2) {
										unsigned short* v81 = ms2->IndexData;
										float* v104 = ms2->field_C;
										int v83 = v81[0];
										int v84 = v81[1];
										int v124 = v84;
										int v111 = 0;
										for (int k = 2; k < ms2->IndexCount; ++k) {
											unsigned short v86 = ms2->IndexData[k];
											int v110 = v86;
											if (v83 != v84 && v84 != v86 && v86 != v83) {
												int v88 = v110;
												memcpy_s(v105, 0x24, v104 + 9 * v83, 0x24);
												int v91 = 9 * v124;
												int v92 = 9 * v88;
												if (v111) {
													//sub
												}
												else {
													//sub
												}
												v105 += 12;
												if (v111) {
													memcpy_s(v105, 0x24, v104 + 9 * v86, 0x24);
													//sub
													v105 += 12;
													memcpy_s(v105, 0x24, v104 + 9 * v84, 0x24);
													//sub
												}
												else {
													memcpy_s(v105, 0x24, v104 + 9 * v84, 0x24);
													//sub
													v105 += 12;
													memcpy_s(v105, 0x24, v104 + 9 * v86, 0x24);
													//sub
												}
												v105 += 12;
											}
											v111 = 1 - v111;
											v83 = v84;
											v84 = v86;
											v124 = v86;
										}
									}
								}
							}
						}
						else {
						//std::cout << "LOADING MMB TYPE 1: << ";
						//for (int za = 0; za < 16; ++za)
						//	std::cout << node->name[za];
						//std::cout << " (tex: ";
						//for (int zb = 0; zb < 16; ++zb)
						//	std::cout << v118[zb];
						//std::cout << std::endl;
							if (CDx->DXDevice->CreateVertexBuffer(
								ms2->IndexCount * node->field_24,
								CDx->VertexBufferUsage,
								node->VertexFVF,
								CDx->VertexBufferPool,
								&ms2->field_28) >= D3D_OK) {
								node->field_40 += node->field_24 * ms2->IndexCount;
								float* v104{ nullptr };
								while (ms2->field_28->Lock(0, 0, (BYTE**)&v104, 0) < D3D_OK)
									;
								for (int k = 0; k < ms2->IndexCount; ++k) {
									unsigned short v97 = ms2->IndexData[k];
									memcpy_s(v104, 0x24, ms2->field_C + 9 * v97, 0x24);
									v104 += 9;
								}
								ms2->field_28->Unlock();
							}
						}
					}
				}
			}
		}
	}
	return node;
}

int FFXI::CYy::MmbManager::GetSomething()
{
	int result{ 0 };
	MmbListNode* v1 = this->ListHead;
	while (v1) {
		result += v1->field_40;
		v1 = v1->NextNode;
	}
	return result;
}

void FFXI::CYy::MmbManager::Clean(MmbListNode* a2)
{
	MmbListNode* node = this->ListHead;
	MmbListNode* v19{ nullptr };
	while (node) {
		MmbListNode* next = node->NextNode;
		if (!a2 || a2 == node) {
			for (int i = 0; i < node->field_44; ++i) {
				MmbStruct* mmb = node->field_60 + i;
				if (mmb->List) {
					if (mmb->ListCount > 0) {
						for (int j = 0; j < mmb->ListCount; ++j) {
							MmbStruct2* submmb = mmb->List + j;
							if (submmb->VerticeCount) {
								if (submmb->field_28)
									submmb->field_28->Release();
								if (submmb->VertexBuffer)
									submmb->VertexBuffer->Release();
								if (submmb->field_2C)
									submmb->field_2C->Release();
								if (submmb->IndexBuffer)
									submmb->IndexBuffer->Release();
								for (int k = 0; k < sizeof(submmb->Buffers) / sizeof(submmb->Buffers[0]); ++k) {
									if (submmb->Buffers[k])
										submmb->Buffers[k]->Release();
								}
								if (submmb->field_B4)
									submmb->field_B4->Release();
							}
						}
					}
					CYyObject::Unwrap((char*)mmb->List);
					mmb->List = nullptr;
				}
			}
			if (node->field_60) {
				CYyObject::Unwrap((char*)node->field_60);
				node->field_60 = nullptr;
			}
			CYyObject::Unwrap((char*)node);
			if (a2) {
				if (v19)
					v19->NextNode = next;
				else
					this->ListHead = next;
				if (this->ListTail == a2)
					this->ListTail = v19;
				this->ListCount -= 1;
				return;
			}
		}
		v19 = node;
		node = next;
	}

	if (!a2)
	{
		this->ListHead = nullptr;
		this->ListTail = nullptr;
		this->ListCount = 0;
	}
}
