#include "CYySkl.h"
#include "CMoSk2.h"
#include "WMatrix.h"
#include "CMoResourceMng.h"
#include "KzFQuat.h"
#include "CYyMotionMan.h"
#include "MotionManStaticStruct.h"
#include "Values.h"

using namespace FFXI::CYy;

const CYyClass CYySkl::CYySklClass{
	"CYySkl", sizeof(CYySkl), &CYyObject::CYyObjectClass
};

CMoSk2* CYySkl::g_sk2{ nullptr };
FFXI::Math::WMatrix* CYySkl::g_mat{ nullptr };
unsigned char* CYySkl::g_obj{ nullptr };

int CYySkl::g_count{ 0 };
int CYySkl::g_arr[256] = { 0 };

const CYyClass* FFXI::CYy::CYySkl::GetRuntimeClass()
{
	return &CYySkl::CYySklClass;
}

FFXI::CYy::CYySkl::CYySkl()
{
	this->Resource = nullptr;
	this->field_8 = nullptr;
	this->field_C = 0;
	this->field_10 = 0;
	this->field_14 = nullptr;
}

FFXI::CYy::CYySkl::~CYySkl()
{
	if (this->field_8)
		CYyObject::Unwrap(this->field_8);

	if (this->field_14)
		CYyObject::Unwrap(this->field_14);
}

void CYySkl::Init() {
	if (this->Resource != nullptr) {
		unsigned short somecount = *(unsigned short*)((*this->Resource)->Data + 2);
		this->field_8 = CYyObject::Wrap((somecount + 1) * sizeof(FFXI::Math::WMatrix), FFXI::Constants::Enums::MEM_MODE::Elem);
		if (this->field_8 != nullptr) {
			int mtxstart = (int)this->field_8 + 15;
			mtxstart &= 0xFFFFFFF0;
			this->field_C = (FFXI::Math::WMatrix*)mtxstart;
			if (somecount > 0) {
				for (int i = 0; i < somecount; ++i) {
					this->field_C[i].Identity();
				}
			}

			this->field_14 = (unsigned char*)CYyObject::Wrap(somecount, FFXI::Constants::Enums::MEM_MODE::Elem);
			if (this->field_14 != nullptr)
				return;
		}
	}

	if (this->field_8 != nullptr)
		CYyObject::Unwrap(this->field_8);

	if (this->field_14 != nullptr)
		CYyObject::Unwrap(this->field_14);
}

bool FFXI::CYy::CYySkl::ResetFlagArray()
{
	if (this->Resource == nullptr || this->field_14 == nullptr)
		return false;

	unsigned short somecount = *(unsigned short*)((*this->Resource)->Data + 2);
	for (int i = 0; i < somecount; ++i) {
		this->field_14[i] = 0;
	}
	return true;
}

short* FFXI::CYy::CYySkl::GetSomeShortPtr(unsigned int a2)
{
	short* retval = this->GetShortPtrBase();
	if (retval)
		return retval + 13 * a2 + 2;

	return nullptr;
}

float* FFXI::CYy::CYySkl::GetSomeFloatPtr() {
	unsigned short* base = (unsigned short*)this->GetShortPtrBase();
	if (base == nullptr)
		return nullptr;

	return (float*)(base + 13 * base[0] + 2);
}

short* FFXI::CYy::CYySkl::GetShortPtrBase()
{
	if (this->Resource == nullptr) 
		return nullptr;
	if (CMoResourceMng::CheckResourceThing((CMoResource***)&this->Resource) == false)
		return nullptr;

	unsigned short somecount = *(unsigned short*)((*this->Resource)->Data + 2);
	return (short*)((*this->Resource)->Data + (30 * somecount + 4));
}

void FFXI::CYy::CYySkl::RecurseSetFlags(CMoSk2* a2, int a3, int a4)
{
	int index = *(unsigned char*)(a2->Data + 30 * a4 + 4);
	this->field_14[index] = 0x80;
	if (index != a4)
		this->RecurseSetFlags(a2, a3, index);

	//doesn't seem like this global is used anywhere
}

void FFXI::CYy::CYySkl::DoingSomething(FFXI::Math::WMatrix* a2, int a3, int a4, char* a5)
{
	if (this->Resource == nullptr)
		return;

	if (a3 == 0)
		a3 = *(unsigned short*)(CYySkl::g_sk2->Data + 2) - 1;

	if (a4 == 0) {
		if (a5 != nullptr) {
			for (int i = 0; i < 2; ++i) {
				if (a5[2 + i] != -1) {
					CYySkl::g_arr[a5[2 + i]] = 0;

				}
			}
		}
	}
	else if (a4 == 1) {

	}
	else if (a4 == 2) {

	}
}

bool FFXI::CYy::CYySkl::SetStatics()
{
	if (this->Resource == nullptr)
		return false;

	if (this->field_8 == nullptr)
		return false;

	if (this->field_14 == nullptr)
		return false;

	if (this->Resource == nullptr)
		CYySkl::g_sk2 = nullptr;
	else
		CYySkl::g_sk2 = *this->Resource;

	//g_sk2 can be nullptr here as per client
	CYySkl::g_count = *(unsigned short*)(CYySkl::g_sk2->Data + 2);
	CYySkl::g_mat = this->field_C;
	CYySkl::g_obj = this->field_14;

	for (int i = 0; i < CYySkl::g_count; ++i) {
		CYySkl::g_obj[i] &= 0x80;
		CYySkl::g_arr[i] = 1;
	}
	return true;
}

void FFXI::CYy::CYySkl::SetStatics2()
{
	if (this->Resource == nullptr)
		return;

	FFXI::Math::KzFQuat v10{};
	unsigned short count = *(unsigned short*)(CYySkl::g_sk2->Data + 2);
	for (unsigned int i = 0; i < count; ++i) {
		float* floatdata = (float*)(CYySkl::g_sk2->Data + 6 + 30 * i);
		v10.x = floatdata[0];
		v10.y = floatdata[1];
		v10.z = floatdata[2];
		v10.w = floatdata[3];
		FFXI::Math::KzFQuat multresult{};
		CYyMotionMan::g_mmss_arr[i].field_0.Multiply(&multresult, &v10);
		CYyMotionMan::g_mmss_arr[i].field_0 = multresult;
		CYyMotionMan::g_mmss_arr[i].field_0.Normalize();
		CYyMotionMan::g_mmss_arr[i].field_10.x += floatdata[4];
		CYyMotionMan::g_mmss_arr[i].field_10.y += floatdata[5];
		CYyMotionMan::g_mmss_arr[i].field_10.z += floatdata[6];
		if (i == 0) {
			FFXI::Math::WMatrix v12{};
			v12.Identity();
			v12.RotateY(3 * FFXI::Constants::Values::ANGLE_PI_OVER_2);
			v12.Vec3TransformDrop4Self(&CYyMotionMan::g_mmss_arr[0].field_10);
		}
	}
}
