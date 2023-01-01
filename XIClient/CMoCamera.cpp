#include "CMoCamera.h"
#include "Globals.h"
#include "CYyDb.h"
#include "BaseProcessor.h"
#include "StorageLists.h"
#include "CMoCameraTask.h"
#include "CYyCamMng2.h"
#include "CMoTaskMng.h"
#include "CXiActor.h"
using namespace FFXI::CYy;

const CYyClass CMoCamera::CMoCameraClass = {
	"CMoCamera", sizeof(CMoCamera), &CYyObject::CYyObjectClass
};

const CYyClass* FFXI::CYy::CMoCamera::GetRuntimeClass()
{
	return &CMoCamera::CMoCameraClass;
}

void FFXI::CYy::CMoCamera::ConstructFromData(char* a2)
{
	int* intData = (int*)a2;

	this->Header.FourCC = intData[0];
	this->Header.Type_Size = intData[1];
	this->Header.RPLHead = nullptr;
	this->Header.LastResourceCreated = nullptr;

	this->CMoAttachments::field_4 = intData[4];
	this->CMoAttachments::field_8 = intData[5];
	this->CMoAttachments::Caster = (CXiActor*)intData[6];
	this->CMoAttachments::Target = (CXiActor*)intData[7];

	this->CMoAttachments::Init();

	int size = 16 * (((this->Header.Type_Size >> 7) & 0x7FFFF) - 2);
	memcpy_s(this->Data, size, a2 + 32, size);
}

void FFXI::CYy::CMoCamera::Open()
{
	//nullsub
}

void FFXI::CYy::CMoCamera::Close()
{
	//nullsub
}

const CYyClass* FFXI::CYy::CMoCamera::AttachedTo()
{
	return this->GetRuntimeClass();
}

CMoCameraTask* FFXI::CYy::CMoCamera::Execute(float a2, CXiActor* a3, CXiActor* a4)
{
	CXiActor* actor = CXiActor::top;
	while (actor != nullptr) {
		actor->VirtActor89();
		actor = actor->PreviousActor;
	}

	actor = CXiActor::top;
	while (actor != nullptr) {
		actor->VirtActor90();
		actor = actor->PreviousActor;
	}

	if (CYyCamMng2::CurrentCameraTask) {
		delete CYyCamMng2::CurrentCameraTask;
		CYyCamMng2::CurrentCameraTask = nullptr;
	}
	unsigned int flags = *(unsigned int*)(this->Data);
	unsigned int one = flags & 0xFF;
	unsigned int two = (flags >> 19) & 1;
	unsigned int thr = (flags >> 20) & 1;
	if ((one + two + thr) == 1 && a2 == 0.0) {
		this->SomeFunc(nullptr);
	}
	else {
		CMoTaskMng::DeleteThisTask = true;
		char* mem = StorageLists::instance->Get(sizeof(CMoCameraTask), Constants::Enums::MEM_MODE::Ex);
		if (mem)
			CYyCamMng2::CurrentCameraTask = new (mem) CMoCameraTask(this, a2, a3, a4);
		return CYyCamMng2::CurrentCameraTask;
	}
	return nullptr;
}

void FFXI::CYy::CMoCamera::SomeFunc(CMoCameraTask* a2)
{
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;

	if (this->Data[0] != 0) {
		CYyCamMng2::GetSomeVecs(&cammng->field_8C, &cammng->field_98);
		cammng->field_88 = 1;
		cammng->field_89 = 1;
	}
	else {
		int index = proc->MatrixPointerIndex;
		FFXI::Math::WMatrix* v5 = proc->MatrixPointers[index];
		proc->MatrixPointerIndex += 1;
		CMoAttachments* att = this;
		if (a2 != nullptr) {
			att = a2;
		}

		att->MakeAttachMatrix(v5, 0, 0);

		float* fptr = (float*)this->Data;
		D3DXVECTOR3 vec{};
		vec.x = fptr[8];
		vec.y = fptr[9];
		vec.z = fptr[10];
		proc->VirtProcessor14(&vec, v5, &vec);
		cammng->field_98 = vec;

		vec.x = fptr[3];
		vec.y = fptr[4];
		vec.z = fptr[5];
		proc->VirtProcessor14(&vec, v5, &vec);
		cammng->field_8C = vec;

		cammng->field_88 = 1;
		cammng->field_89 = 1;
		cammng->field_8A = 1;
		cammng->field_A4 = fptr[11];
		CYyDb::g_pCYyDb->SetField2F8(fptr[7]);
		proc->MatrixPointerIndex -= 1;
		proc->MatrixPointers[proc->MatrixPointerIndex] = v5;
	}
}
