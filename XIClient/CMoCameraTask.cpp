#include "CMoCameraTask.h"
#include "CYyCamMng2.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoDx.h"
#include "CXiActor.h"
#include "CMoPathObject.h"
#include "PlaceHolders.h"
#include "BaseProcessor.h"
#include "CYyCamMng2.h"
#include "CMoCamera.h"
#include "Values.h"
#include <iostream>
using namespace FFXI::CYy;

const CYyClass CMoCameraTask::CMoCameraTaskClass = {
	"CMoCameraTask", sizeof(CMoCameraTask), &CMoTask::CMoTaskClass
};

//LOCAL FUNCS
float Smooth(float a1, int a2) {
	switch (a2) {
	case 1:
		return sin(a1 * FFXI::Constants::Values::ANGLE_PI_OVER_2);
	case 2:
		return 1.0 - cos(a1 * FFXI::Constants::Values::ANGLE_PI_OVER_2);
	case 3: {
		float v4 = a1 * FFXI::Constants::Values::ANGLE_2PI;
		if (v4 <= FFXI::Constants::Values::ANGLE_PI)
			return sin(v4 * 0.5) * 0.5;

		return 1.0 - (sin(v4 * 0.5) * 0.5);
	}
	case 4:
	{
		float v5 = cos(a1 * FFXI::Constants::Values::ANGLE_PI);
		return 1.0 - (v5 * 0.5 + 0.5);
	}
	default:
		return a1;
	}
}
//~LOCAL FUNCS
FFXI::CYy::CMoCameraTask::CMoCameraTask(CMoCamera* a2, float a3, CXiActor* a4, CXiActor* a5)
{
	CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	this->CMoAttachments::MixFlags(a2);
	this->CMoAttachments::SetCaster(a4);
	this->CMoAttachments::SetTarget(a5);
	this->Camera = a2;
	this->field_78 = a3;
	this->field_7C = a3;
	CMoResource::DoTheThing(a2);

	this->field_C4 = nullptr;
	this->Path = nullptr;

	D3DXVECTOR3 v26{}, v27{};
	this->Camera->CMoAttachments::MakeAttachMatrix(&this->field_80, 0, 0);
	unsigned int flags = *(unsigned int*)this->Camera->Data;
	if ((flags & 0x100000) != 0)
		CYyCamMng2::GetSomeVecs(&v26, &v27);
	
	int index = proc->MatrixPointerIndex;
	FFXI::Math::WMatrix* mat = proc->MatrixPointers[index];
	proc->MatrixPointerIndex += 1;

	if ((flags & 0x100000) != 0 || (flags & 0x80000) != 0) {
		*mat = this->field_80;
		proc->VirtProcessor1(mat);
	}

	int sum = (flags & 0xFF) + ((flags >> 19) & 1) + ((flags >> 20) & 1);
	if (sum > 2)
		this->Path = CMoPathObject::Create((unsigned int*)this->Camera->Data);
	else if (sum == 2) {
		this->field_C4 = (D3DXVECTOR4*)CYyObject::Wrap(6 * sizeof(D3DXVECTOR4), FFXI::Constants::Enums::MEM_MODE::Ex);
		int v12 = 0;
		if ((flags & 0x80000) != 0) {
			proc->VirtProcessor14((D3DXVECTOR3*)this->field_C4, mat, &cammng->field_8C);
			proc->VirtProcessor14((D3DXVECTOR3*)(this->field_C4 + 1), mat, &cammng->field_98);
			this->field_C4[2].x = cammng->field_A4;
			this->field_C4[2].y = CYyDb::g_pCYyDb->field_2F8;
			v12 = 3;
		}

		unsigned char count = flags & 0xFF;
		for (int i = 0; i < count; ++i) {
			float* input = (float*)(this->Camera->Data + 16 + 48 * i);
			this->field_C4[v12].x = input[0];
			this->field_C4[v12].y = input[1];
			this->field_C4[v12].z = input[2];
			this->field_C4[v12].w = input[3];


			this->field_C4[v12 + 1].x = input[4];
			this->field_C4[v12 + 1].y = input[5];
			this->field_C4[v12 + 1].z = input[6];
			this->field_C4[v12 + 1].w = input[7];

			this->field_C4[v12 + 2].x = input[7];
			this->field_C4[v12 + 2].y = input[3];
			v12 += 3;
		}

		if ((flags & 0x100000) != 0) {
			proc->VirtProcessor14((D3DXVECTOR3*)(this->field_C4 + v12), mat, &v26);
			proc->VirtProcessor14((D3DXVECTOR3*)(this->field_C4 + v12 + 1), mat, &v27);
			this->field_C4[v12 + 2].x = 0.0;
			if (Placeholder::is_first_person_view)
				this->field_C4[v12 + 2].y = 280.0;
			else
				this->field_C4[v12 + 2].y = 350.0;
		}
	}
	proc->MatrixPointerIndex -= 1;
	proc->MatrixPointers[proc->MatrixPointerIndex] = mat;
}

FFXI::CYy::CMoCameraTask::~CMoCameraTask()
{
	this->Camera->SomeClean3();

	if (this->Path) {
		this->Path->field_4.~CMoSpline();
		this->Path->field_2C.~CMoSpline();
		this->Path->field_54.~CMoSpline();
		StorageLists::instance->Delete(this->Path);
	}
			
	if (this->field_C4)
		CYyObject::Unwrap(this->field_C4);

	if (CYyCamMng2::CurrentCameraTask == this)
		CYyCamMng2::CurrentCameraTask = nullptr;
}

float FFXI::CYy::CMoCameraTask::GetT(float a2)
{
	int* intData = (int*)(this->Camera->Data);
	if (intData[1] >= 0 && intData[1] <= 4)
		return Smooth(a2, intData[1]);

	exit(0x1003C346);

	CMoResource** unused{ nullptr };
	//if (!this->Camera->field_78)
		;// this->Camera->field_78 = this->Camera->FindResourceUnder(&unused, FFXI::Constants::Enums::ResourceType::Keyframe, this->Camera->field_74);
	return 0.0f;
}

const CYyClass* FFXI::CYy::CMoCameraTask::GetRuntimeClass()
{
	return &CMoCameraTask::CMoCameraTaskClass;
}

char FFXI::CYy::CMoCameraTask::OnMove()
{
	CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	this->field_78 -= CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
	
	if (this->field_78 < 0.0)
		this->field_78 = 0.0;

	unsigned int flags = *(unsigned int*)this->Camera->Data;
	float v3 = (float)((flags >> 8) & 0xFF) * 0.0039215689;

	if (v3 > 0.0) {
		FFXI::Math::WMatrix* v9 = proc->MatrixPointers[proc->MatrixPointerIndex];
		proc->MatrixPointerIndex += 1;
		this->CMoAttachments::MakeAttachMatrix(v9, 0, 0);

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				float v14 = v9->m[i][j] - this->field_80.m[i][j];
				this->field_80.m[i][j] += v14 * v3;
			}
		}
		proc->MatrixPointerIndex -= 1;
		proc->MatrixPointers[proc->MatrixPointerIndex] = v9;
	}

	float progress = 1.0 - this->field_78 / this->field_7C;
	float v17 = this->GetT(progress);
	
	unsigned int sum = (unsigned __int8)flags + ((flags >> 19) & 1) + ((flags >> 20) & 1);
	if (sum > 2) {
		D3DXVECTOR3 v36{}, v37{}, v38{};
		this->Path->GetSplines(v17, &v36, &v37, &v38);
		proc->VirtProcessor15(&cammng->field_8C, &this->field_80, &v36);
		proc->VirtProcessor15(&cammng->field_98, &this->field_80, &v37);
		cammng->field_A4 = v38.y;
		CYyDb::g_pCYyDb->SetField2F8(v38.x);

		cammng->field_88 = 1;
		cammng->field_89 = 1;
		cammng->field_8A = 1;
	}
	else if (sum == 2 && this->field_C4)
	{
		float a2b = 1.0 - v17;
		proc->VirtProcessor16(&cammng->field_8C, (D3DXVECTOR3*)(this->field_C4 + 3), (D3DXVECTOR3*)(this->field_C4), a2b);
		cammng->field_88 = 1;
				
		proc->VirtProcessor16(&cammng->field_98, (D3DXVECTOR3*)(this->field_C4 + 4), (D3DXVECTOR3*)(this->field_C4 + 1), a2b);
		cammng->field_89 = 1;

		proc->VirtProcessor16(&proc->field_910, (D3DXVECTOR3*)(this->field_C4 + 5), (D3DXVECTOR3*)(this->field_C4 + 2), a2b);
		cammng->field_8A = 1;

		cammng->field_A4 = proc->field_910.x;
		CYyDb::g_pCYyDb->SetField2F8(proc->field_910.y);
	} else
		this->Camera->SomeFunc(this);
	
	if (this->field_78 > 0.0)
		return 0;

	delete this;
	return 1;
}
