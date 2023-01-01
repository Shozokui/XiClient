#include "CXiDancerActor.h"
#include "XiDancerActorPara.h"
#include "Globals.h"
#include "RegistryConfig.h"
using namespace FFXI::CYy;

int DancerActorIndex{0};

const CYyClass CXiDancerActor::CXiDancerActorClass{
	"CXiDancerActor", sizeof(CXiDancerActor), &CXiCollisionActor::CXiCollisionActorClass
};

const CYyClass* FFXI::CYy::CXiDancerActor::GetRuntimeClass()
{
	return &CXiDancerActor::CXiDancerActorClass;
}

FFXI::CYy::CXiDancerActor::CXiDancerActor(D3DXVECTOR4* a2, D3DXVECTOR4* a3, XiDancerActorPara* a4, int a5)
	:CXiSkeletonActor(0)
{
	this->AtelBuffSet(a2, a3, a4, a5);
	this->SetUp(-1);
}

void FFXI::CYy::CXiDancerActor::AtelBuffSet(D3DXVECTOR4* a2, D3DXVECTOR4* a3, XiDancerActorPara* a4, int a5)
{
	int v5 = *(int*)this & 0xFFFFFF | 0x4000000;
	if (a5 >= 0) {
		this->AtelBuffer = XiAtelBuff::AtelBuffers[a5];
		this->AtelBuffer->GlobalBufferIndex = a5;
		this->AtelBuffer->field_78 = (a5 ^ v5) & 0xFFFFFF ^ v5;
		this->AtelBuffer->Actor = this;
		this->AtelBuffInit(this->AtelBuffer, a2, a3, a4);
		this->AtelBuffer->field_120 |= 0x200;
	}
	else {
		this->AtelBuffer = &this->field_A0C;
		this->AtelBuffer->GlobalBufferIndex = ++DancerActorIndex;
		this->AtelBuffer->field_78 = (v5 ^ DancerActorIndex) & 0xFFFFFF ^ v5;
		this->AtelBuffInit(this->AtelBuffer, a2, a3, a4);
	}
}

void FFXI::CYy::CXiDancerActor::AtelBuffInit(XiAtelBuff* a2, D3DXVECTOR4* a3, D3DXVECTOR4* a4, XiDancerActorPara* a5)
{
	a2->SetName(a5->Name);

	a2->field_4 = *a3;
	a2->field_14 = *a4;

	this->Set5FC(a3);
	this->Set61C(a4);

	a2->ActorType = 0;

	if (a2->RaceGender != a5->RaceGender) {
		a2->field_120 |= 1;
		a2->RaceGender = a5->RaceGender;
	}

	if (a2->Equipment[0] != a5->Equipment[0]) {
		a2->Equipment[0] = a5->Equipment[0];
		a2->field_F4 |= 0x01;
	}

	if (a2->Equipment[1] != a5->Equipment[1]) {
		a2->Equipment[1] = a5->Equipment[1];
		a2->field_F4 |= 0x02;
	}

	if (a2->Equipment[2] != a5->Equipment[2]) {
		a2->Equipment[2] = a5->Equipment[2];
		a2->field_F4 |= 0x04;
	}

	if (a2->Equipment[3] != a5->Equipment[3]) {
		a2->Equipment[3] = a5->Equipment[3];
		a2->field_F4 |= 0x08;
	}

	if (a2->Equipment[4] != a5->Equipment[4]) {
		a2->Equipment[4] = a5->Equipment[4];
		a2->field_F4 |= 0x10;
	}

	if (a2->Equipment[5] != a5->Equipment[5]) {
		a2->Equipment[5] = a5->Equipment[5];
		a2->field_F4 |= 0x20;
	}

	if (a2->Equipment[6] != a5->Equipment[6]) {
		a2->Equipment[6] = a5->Equipment[6];
		a2->field_F4 |= 0x40;
	}

	if (a2->Equipment[7] != a5->Equipment[7]) {
		a2->Equipment[7] = a5->Equipment[7];
		a2->field_F4 |= 0x80;
	}

	if (a2->Equipment[8] != a5->Equipment[8]) {
		a2->Equipment[8] = a5->Equipment[8];
		a2->field_F4 |= 0x100;
	}

	a2->MonsterNumber = a5->RaceGender;
	memcpy(a2->field_24E, a5->Equipment, sizeof(a2->field_24E));

	a2->field_120 |= 0x100000;
}

void FFXI::CYy::CXiDancerActor::OnDraw()
{
	this->CXiSkeletonActor::OnDraw();
}

char FFXI::CYy::CXiDancerActor::OnMove()
{
	if (FFXI::Config::RegistryConfig::g_pOptions->Other.field_24 != 0) {
		if (this->AtelBuffer != nullptr) {
			this->field_34 = this->AtelBuffer->field_4;
			this->field_44 = this->AtelBuffer->field_14;
		}
	}
	return this->CXiSkeletonActor::OnMove();
}
