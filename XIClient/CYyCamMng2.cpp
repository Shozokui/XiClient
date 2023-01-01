#include "CYyCamMng2.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoCameraTask.h"
#include "PlaceHolders.h"
#include "CMoResourceMng.h"
using namespace FFXI::CYy;

const CYyClass CYyCamMng2::CYyCamMng2Class{
	"CYyCamMng2", sizeof(CYyCamMng2), &CYyObject::CYyObjectClass
};

char CYyCamMng2::SomeByte1{ 0 };
char CYyCamMng2::SomeByte2{ 0 };
int CYyCamMng2::SomeDword1{ 0 };
int CYyCamMng2::CameraInitTimer{ 0 };
CMoCameraTask* CYyCamMng2::CurrentCameraTask{ nullptr };

const CYyClass* FFXI::CYy::CYyCamMng2::GetRuntimeClass()
{
	return &CYyCamMng2Class;
}

void FFXI::CYy::CYyCamMng2::VObj2(int* a2)
{
	if (a2[0] != 2) {
		if (a2[0] != 5) {
			return;
		}
	}

	if (Globals::g_pCYyCamMng2 == CYyDb::g_pCYyDb->CameraManager)
		CYyDb::g_pCYyDb->CameraManager = nullptr;
	
	if (Globals::g_pCYyCamMng2) {
		delete Globals::g_pCYyCamMng2;
		Globals::g_pCYyCamMng2 = nullptr;
	}
}

void FFXI::CYy::CYyCamMng2::InitCameraManager()
{
	char* mem = StorageLists::instance->Get(sizeof(CYyCamMng2), Constants::Enums::MEM_MODE::Ex);
	if (mem) {
		Globals::g_pCYyCamMng2 = new (mem) CYyCamMng2();
		Globals::g_pCYyCamMng2->Init();
		CYyDb::g_pCYyDb->LinkCameraManager(Globals::g_pCYyCamMng2);
	}
}

void FFXI::CYy::CYyCamMng2::SetSomeValues(char a1, char a2, int a3)
{
	CYyCamMng2::SomeByte1 = a1;
	CYyCamMng2::SomeByte2 = a2;
	CYyCamMng2::SomeDword1 = a3;
}

void FFXI::CYy::CYyCamMng2::ResetCameraInitTimer()
{
	CameraInitTimer = 60;
}

void FFXI::CYy::CYyCamMng2::GetSomeVecs(D3DXVECTOR3* a1, D3DXVECTOR3* a2)
{
	if (Placeholder::GetControlActor) {
		throw "IMPLEMENT THIS";
	}
	else {
		*a1 = CYyDb::g_pCYyDb->CameraManager->Position;
		*a2 = CYyDb::g_pCYyDb->CameraManager->field_50;

	}
}

void FFXI::CYy::CYyCamMng2::Init()
{
	this->SomeInit();
	this->field_B4 = 3.0;
	this->field_B8 = 6.0;
}

void FFXI::CYy::CYyCamMng2::SomeInit()
{
	this->Position = {0.0, -1.7, 0.0};
	CYyDb::g_pCYyDb->SetField2F8(350.0);
	this->field_50 = this->Position;
	this->field_50.z += 1;
	this->field_5C = 0.0;
	this->field_A8 = {0.0, -1.0, 0.0};
	this->field_6C = this->field_50;
	this->field_60 = this->Position;
	this->field_84 = 0;
	this->field_80 = 0;
	this->field_7C = 0;
	this->field_88 = 0;
	this->field_89 = 0;
	this->field_8A = 0;
	this->field_F4 = 0;
}

void FFXI::CYy::CYyCamMng2::Update()
{
	if (this->field_88) {
		this->SetPos(&this->field_8C);
		this->field_88 = 0;
	}
	if (this->field_89) {
		this->SetAt(&this->field_98);
		this->field_89 = 0;
	}
	if (this->field_8A) {
		this->field_5C = this->field_A4;
		this->field_8A = 0;
	}
	float v2 = CYyDb::g_pCYyDb->CheckTick();
	if (v2 >= 8.0)
		v2 = 8.0;
	if (this->field_7C == 0.0 && this->field_80 == 0.0)
		return;

	double v4 = (double)v2 * this->field_80;

	if (v4 <= 0.0) {
		this->field_7C += v4;
		if (this->field_7C <= 0.0)
			return;
	}
	else {
		this->field_7C -= v4;
		if (this->field_7C >= 0.0)
			return;
	}

	double v12 = this->field_7C;
	if (v12 < 0.0)
		v12 = -v12;

	if (v12 < 0.000099999997) {
		double v16 = this->field_80;
		if (v16 < 0.0)
			v16 = -v16;
		if (v16 < 0.000099999997)
		{
			this->field_80 = 0.0;
			this->field_7C = 0.0;
			return;
		}
	}

	double v20 = (double)this->field_7C * 0.86000001;
	this->field_7C = v20;
	this->field_80 = v20 * 0.125;
	if (this->field_80 < 0.0)
		this->field_80 = -this->field_80;

	this->field_84 = CYyDb::g_pCYyDb->pCMoResourceMng->rng.frand(10.0) * 0.1 + 1.0;
}

void FFXI::CYy::CYyCamMng2::SetPos(D3DXVECTOR3* a2)
{
	this->Position = *a2;
}

void FFXI::CYy::CYyCamMng2::SetAt(D3DXVECTOR3* a2)
{
	D3DXVECTOR3 a1 = *a2 - this->Position;
	float magsq = a1.x * a1.x + a1.y * a1.y + a1.z * a1.z;
	if (magsq != 0)
		this->field_50 = *a2;
}

bool FFXI::CYy::CYyCamMng2::Check()
{
	if (this->field_7C != 0.0)
		return true;
	if (this->field_60.x != this->Position.x)
		return true;
	if (this->field_60.y != this->Position.y)
		return true;
	if (this->field_60.z != this->Position.z)
		return true;
	if (this->field_6C.x != this->field_50.x)
		return true;
	if (this->field_6C.y != this->field_50.y)
		return true;
	if (this->field_6C.z != this->field_50.z)
		return true;
	return this->field_78 != this->field_5C;
}

FFXI::Math::WMatrix* FFXI::CYy::CYyCamMng2::MakeMatrix()
{
	D3DXVECTOR3 a1{}, v7{}, v8{};
	FFXI::Math::WMatrix v9{}, psrc{};
	float angle{};

	v8 = this->field_50;
	D3DXVec3Subtract(&a1, &this->field_50, &this->Position);
	v9.Identity();
	angle = atan2(-a1.x, -a1.z);
	v9.RotateZ(this->field_5C);
	v9.RotateY(angle);
	v9.RotateX(0.0);
	v7 = this->field_A8;
	v9.Vec3TransformNormalSelf(&v7);
	Globals::Vec3Normalize(&a1);
	if (this->field_7C != 0.0) {
		a1.y += this->field_84 * this->field_7C;
		Globals::Vec3Normalize(&a1);
	}
	D3DXVec3Add(&v8, &this->Position, &a1);
	D3DXMatrixLookAtRH(&psrc, &this->Position, &v8, &v7);
	if (psrc.CheckMatrix())
		this->field_4 = psrc;
	return &this->field_4;
}

FFXI::Math::WMatrix* FFXI::CYy::CYyCamMng2::GetView()
{
	this->MakeMatrix();
	this->field_60 = this->Position;
	this->field_6C = this->field_50;
	return &this->field_4;
}

FFXI::CYy::CYyCamMng2::CYyCamMng2()
{
	this->field_F0 = 0;
	this->Init();
	//todo stuff here
	CameraInitTimer = 0;
	CYyCamMng2::SetSomeValues(0, 1, 0);
}

FFXI::CYy::CYyCamMng2::~CYyCamMng2()
{
	//nullsub
}
