#include "RidManager.h"
#include "CYyObject.h"
#include "RidListNode.h"
#include "Zoneline.h"
#include "Submodel.h"
#include "RidStruct.h"
#include "Enums.h"
#include <string>
using namespace FFXI::CYy;

int RidManager::ListSize{ 0 };
RidListNode* RidManager::ListHead{ nullptr };
RidListNode* RidManager::ListTail{ nullptr };

D3DXVECTOR3 mults[] = {
	{-0.5, -0.5, -0.5},
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5}
};
RidListNode* CreateNode() {
	RidListNode* node = (RidListNode*)CYyObject::Wrap(sizeof(RidListNode), FFXI::Constants::Enums::MEM_MODE::Ex);
	memset(node, 0, sizeof(RidListNode));

	if (RidManager::ListSize == 0)
		RidManager::ListHead = node;
	else
		RidManager::ListTail->Next = node;

	RidManager::ListTail = node;
	++RidManager::ListSize;

	return node;
}
FFXI::CYy::RidManager::RidManager()
{
	this->Init();
}

FFXI::CYy::RidManager::~RidManager()
{
	this->Clean();
}

void FFXI::CYy::RidManager::Add(char* a2)
{
	RidListNode* node = CreateNode();
	node->data = a2;
	node->field_8 = *((int*)a2 + 12);
	int v3 = 0;
	char* v6 = a2 + 100;
	for (int i = 0; i < node->field_8; ++i) {
		if (*v6 != 'm' && *v6 != 'M' || !*((int*)v6 - 6))
			++v3;
		v6 += 64;
	}

	node->field_10 = v3;
	int osize = v3 * sizeof(RidStruct);
	node->field_C = (RidStruct*)CYyObject::Wrap(osize, FFXI::Constants::Enums::MEM_MODE::Ex);
	memset(node->field_C, 0, osize);

	if (node->field_8 <= 0) return;
	
	char* v5{ nullptr };
	int nodeIndex = 0;
	for (int i = 0; i < node->field_8; ++i) {
		v5 = a2 + 64 * i;
		float* floatData = (float*)v5;

		RidStruct* rs = node->field_C + nodeIndex;

		char v11 = v5[36];
		if (v11 != 'm' && v11 != 'M' || !*((int*)v5 + 3)) {
			if (floatData[6] < 0.0)
				floatData[6] *= -1;

			if (floatData[7] < 0.0)
				floatData[7] *= -1;

			if (floatData[8] < 0.0)
				floatData[8] *= -1;

			rs->field_0.Identity();

			rs->Position.x = floatData[0];
			rs->Position.y = floatData[1];
			rs->Position.z = floatData[2];

			FFXI::Math::WMatrix Translater{};
			D3DXMatrixTranslation(&Translater, -rs->Position.x, -rs->Position.y, -rs->Position.z);
			rs->field_0.MatrixMultiply(&Translater);

			FFXI::Math::WMatrix RotateY{};
			D3DXMatrixRotationY(&RotateY, -floatData[4]);
			rs->field_0.MatrixMultiply(&RotateY);

			float xScale = 1.0, yScale = 1.0, zScale = 1.0;
			if (floatData[6])
				xScale /= floatData[6];
			if (floatData[7])
				yScale /= floatData[7];
			if (floatData[8])
				zScale /= floatData[8];

			FFXI::Math::WMatrix Scaler{};
			D3DXMatrixScaling(&Scaler, xScale, yScale, zScale);
			rs->field_0.MatrixMultiply(&Scaler);

			D3DXMatrixScaling(&Scaler, floatData[6], floatData[7], floatData[8]);
			FFXI::Math::WMatrix a1{};
			a1.Identity();
			a1.MatrixMultiply(&Scaler);

			D3DXMatrixRotationY(&RotateY, floatData[4]);
			a1.MatrixMultiply(&RotateY);

			D3DXMatrixTranslation(&Translater, rs->Position.x, rs->Position.y, rs->Position.z);
			a1.MatrixMultiply(&Translater);

			for (int j = 0; j < sizeof(rs->field_4C) / sizeof(rs->field_4C[0]); ++j)
				a1.Vec3TransformDrop4(rs->field_4C + i, mults + i);
			

			rs->field_40 = floatData[9];
			rs->field_44 = floatData[10];
			rs->field_48 = floatData[11];
			rs->FourCC = 0;
			rs->field_E4 = 0;

			//It appears that e4 will receive the null terminate here
			if (v11 == 'z' || v11 == 'Z' || v11 == 'm' || v11 == 'M')
				strncpy_s((char*)&rs->FourCC, 8, v5 + 36, 4);

			rs->field_E8 = 0;
			rs->field_B8 = (double)*((short*)v5 + 26) * 0.00390625f + floatData[1];
			rs->field_BC = (double)*((short*)v5 + 27) * 0.00390625f + floatData[1];
			rs->field_C0 = 0;
			rs->field_C4 = 0;
			rs->field_C8 = 0;
			rs->field_CC = 0;
			rs->field_D0 = 0;
			rs->field_EC = (*((int*)v5 + 12) << 24) >> 28;
			if ((char)rs->field_40 == '_') {
				//sub //TODO

			}
			else
				rs->field_D8 = 0;
			nodeIndex += 1;
		}

	}
}

void FFXI::CYy::RidManager::Clean()
{
	RidListNode* node = RidManager::ListHead;

	while (node != nullptr) {
		RidListNode* next = node->Next;
		if (node->field_10 > 0) {
			for (int i = 0; i < node->field_10; ++i) {
				if (node->field_C[i].field_D8)
					CYyObject::Unwrap((char*)node->field_C[i].field_D8);
			}
		}
		if (node->field_C)
			CYyObject::Unwrap((char*)node->field_C);

		CYyObject::Unwrap((char*)node);

		node = next;
	}

	if (this->Zonelines)
		CYyObject::Unwrap((char*)this->Zonelines);

	this->Zonelines = nullptr;
	this->ZonelineCount = 0;

	if (this->Submodels)
		CYyObject::Unwrap((char*)this->Submodels);

	this->Submodels = nullptr;
	this->SubmodelCount = 0;

	RidManager::Init();
}

void FFXI::CYy::RidManager::Init()
{
	this->Submodels = nullptr;
	this->SubmodelCount = 0;
	this->Zonelines = nullptr;
	this->ZonelineCount = 0;

	ListHead = nullptr;
	ListTail = nullptr;
	ListSize = 0;
}

RidListNode* FFXI::CYy::RidManager::GetHead()
{
	return RidManager::ListSize != 0 ? RidManager::ListHead : nullptr;
}

void FFXI::CYy::RidManager::InitZonelines() {
	RidListNode* v1 = RidManager::ListHead;
	int counter = 0;
	while (v1) {
		if (v1->field_8 > 0) {
			for (int i = 0; i < v1->field_8; ++i) {
				char* data = v1->data + 100 + 64 * i;
				if (*data == 'Z' || *data == 'z') {
					if (*(int*)(data + 4)) {
						counter += 1;
					}
				}
			}
		}
		v1 = v1->Next;
	}

	this->Zonelines = nullptr;
	this->ZonelineCount = counter;

	if (counter > 0) {
		this->Zonelines = (Zoneline*)CYyObject::Wrap(sizeof(Zoneline) * counter, FFXI::Constants::Enums::MEM_MODE::Ex);
		RidListNode* v7 = RidManager::ListHead;
		int zonelineCounter = 0;
		while (v7) {
			if (v7->field_8 > 0) {
				for (int i = 0; i < v7->field_8; ++i) {
					char* data = v7->data + 64 + 64 * i;
					char prefix = *(data + 36);
					if (prefix == 'Z' || prefix == 'z') {
						if (*(int*)(data + 40)) {
							memcpy_s(this->Zonelines + zonelineCounter, sizeof(Zoneline), data, sizeof(Zoneline));
							zonelineCounter += 1;
						}
					}
				}
			}
			v7 = v7->Next;
		}
	}
}

void FFXI::CYy::RidManager::InitSubModels()
{
	RidListNode* v1 = RidManager::ListHead;
	int counter = 0;
	while (v1) {
		if (v1->field_8 > 0) {
			for (int i = 0; i < v1->field_8; ++i) {
				char* data = v1->data + 100 + 64 * i;
				if (*data == 'M' || *data == 'm') {
					if (*(int*)(data + 4)) {
						counter += 1;
					}
				}
			}
		}
		v1 = v1->Next;
	}

	this->Submodels = nullptr;
	this->SubmodelCount = counter;
	
	if (counter > 0) {
		this->Submodels = (Submodel*)CYyObject::Wrap(sizeof(Submodel) * counter, FFXI::Constants::Enums::MEM_MODE::Ex);
		RidListNode* v7 = RidManager::ListHead;
		int submodelCount = 0;
		while (v7) {
			if (v7->field_8 > 0) {
				for (int i = 0; i < v7->field_8; ++i) {
					char* data = v7->data + 64 + 64 * i;
					char prefix = *(data + 36);
					if (prefix == 'M' || prefix == 'm') {
						if (*(int*)(data + 40)) {
							memcpy_s(this->Submodels + submodelCount, sizeof(Submodel), data, sizeof(Submodel));
							submodelCount += 1;
						}
					}
				}
			}
			v7 = v7->Next;
		}
	}
}