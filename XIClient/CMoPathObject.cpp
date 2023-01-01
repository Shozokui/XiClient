#include "CMoPathObject.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CYyCamMng2.h"
#include "StorageLists.h"
using namespace FFXI::CYy;

CMoPathObject* FFXI::CYy::CMoPathObject::Create(unsigned int* a1)
{
	char* mem = StorageLists::instance->Get(sizeof(CMoPathObject), FFXI::Constants::Enums::MEM_MODE::Ex);
	if (!mem) return nullptr;

	CMoPathObject* ret = new (mem) CMoPathObject();
	unsigned int flags = *a1;
	unsigned int sum = (flags & 0xFF) + ((flags >> 19) & 1) + ((flags >> 20) & 1);
	
	ret->field_4.AllocTable(flags, 7, 0, 0);
	ret->field_2C.AllocTable(flags, 7, 0, 0);
	ret->field_54.AllocTable(flags, 7, 0, 0);

	float* v14 = (float*)((char*)ret->field_2C.field_C[1] + 4);
	float* v15 = (float*)((char*)ret->field_2C.field_C[2] + 4);
	float* v16 = (float*)((char*)ret->field_54.field_C[0] + 4);
	float* v4 = (float*)((char*)ret->field_4.field_C[0] + 4);
	float* v17 = (float*)((char*)ret->field_54.field_C[1] + 4);
	float* v5 = (float*)((char*)ret->field_4.field_C[1] + 4);
	float* v18 = (float*)((char*)ret->field_54.field_C[2] + 4);
	float* v7 = (float*)((char*)ret->field_4.field_C[2] + 4);
	float* v8 = (float*)((char*)ret->field_2C.field_C[0] + 4);
	float* v13 = v8;
	int v19 = 0;
	CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;

	//Maybe if the path starts from current camera?
	if ((flags & 0x80000) != 0) {
		*v4++ = cammng->Position.x;
		*v5++ = cammng->Position.y;
		*v7++ = cammng->Position.z;
		*v13 = cammng->field_50.x;
		*v14++ = cammng->field_50.y;
		*v15++ = cammng->field_50.z;
		*v16++ = CYyDb::g_pCYyDb->field_2F4;
		v8 += 1;
		*v17 = cammng->field_5C;
		*v18++ = 0.0;
		v19 = 1;
	}

	int count = (unsigned char)flags;
	float* v10 = (float*)(a1 + 5);
	for (int i = 0; i < count; ++i) {
		++v8;
		*v4 = *(v10 - 1);
		*v5 = *v10;
		*v7 = v10[1];
		*(v8 - 1) = v10[3];
		++v4;
		++v5;
		*v14 = v10[4];
		float v11 = v10[5];
		++v7;
		++v14;
		v10 += 12;
		*v15++ = v11;
		*v16++ = *(v10 - 10);
		*v17++ = *(v10 - 6);
		*v18++ = (float)v19++;
	}

	ret->field_4.CalcTable();
	ret->field_2C.CalcTable();
	ret->field_54.CalcTable();
	return ret;
}

void FFXI::CYy::CMoPathObject::GetSplines(float a2, D3DXVECTOR3* a3, D3DXVECTOR3* a4, D3DXVECTOR3* a5)
{
	this->field_4.Get(a2, a3);
	this->field_2C.Get(a2, a4);
	this->field_54.Get(a2, a5);
}
