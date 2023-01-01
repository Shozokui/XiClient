#include "CMoAttachments.h"
#include "Globals.h"
#include "CYyDb.h"
#include "BaseProcessor.h"
#include "CXiActor.h"
#include "XiZone.h"
#include "CMoAttachmentsSubStruct.h"
#include "CXiDollActor.h"
using namespace FFXI::CYy;

const CYyClass CMoAttachments::CMoAttachmentsClass{
	"CMoAttachments", sizeof(CMoAttachments), nullptr
};

FFXI::Math::WMatrix FFXI::CYy::CMoAttachments::AttachMatrix{};

void FFXI::CYy::CMoAttachments::VirtAttach1(CMoAttachments* a2)
{
	CXiActor* mcast = this->GetMasterTarget();
	mcast->VirtActor241(a2);
}

void FFXI::CYy::CMoAttachments::VirtAttach2(CMoAttachments* a2)
{
	CXiActor* mtarg = this->GetMasterTarget();
	mtarg->VirtActor241(a2);
}

bool FFXI::CYy::CMoAttachments::VirtAttach3()
{
	return false;
}

bool FFXI::CYy::CMoAttachments::VirtAttach4()
{
	return false;
}

CYyObject* FFXI::CYy::CMoAttachments::GetCYyObject()
{
	return nullptr;
}

const CYyClass* FFXI::CYy::CMoAttachments::AttachedTo()
{
	return &CMoAttachments::CMoAttachmentsClass;
}

FFXI::CYy::CMoAttachments::CMoAttachments()
{
	Init();
}

FFXI::CYy::CMoAttachments::~CMoAttachments()
{

}

void FFXI::CYy::CMoAttachments::Init()
{
	this->Caster = 0;
	this->Target = 0;
	this->CastAttach = 0;
	this->TargAttach = 0;
	this->CasterSubStruct.SetValues(0, 0x04000000);
	this->TargetSubStruct.SetValues(0, 0x04000000);

	this->field_34 = 0;
}

float FFXI::CYy::CMoAttachments::MakeAttachMatrix(FFXI::Math::WMatrix* a2, D3DXVECTOR4* a3, D3DXVECTOR4* a4)
{
	float retval = 0.0;
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;

	int index = proc->MatrixPointerIndex;
	FFXI::Math::WMatrix* f1 = proc->MatrixPointers[index];
	FFXI::Math::WMatrix* f2 = proc->MatrixPointers[index + 1];
	proc->MatrixPointerIndex += 2;

	CXiActor* caster = this->GetMasterCaster();
	CXiActor* target = this->GetMasterTarget();
	CXiActor* v11{ nullptr };
	if (caster == nullptr) {
		v11 = target;
	}
	else {
		v11 = caster->Get154Actor();
		if (v11 == nullptr) {
			v11 = target;
		}
	}

	unsigned int value = (this->field_4 & 0xF) + 16 * ((this->field_4 >> 16) & 1);
	switch (value) {
	case 1:
		throw "not implemented";
		break;
	case 2:
		throw "not implemented";
		break;
	case 3:
		throw "not implemented";
		break;
	case 4:
		throw "not implemented";
		break;
	case 5:
		throw "not implemented";
		break;
	case 6:
		throw "not implemented";
		break;
	case 7:
		throw "not implemented";
		break;
	case 8:
		throw "not implemented";
		break;
	case 9:
		throw "not implemented";
		break;
	case 10:
		throw "not implemented";
		break;
	case 11:
		throw "not implemented";
		break;
	case 12:
		throw "not implemented";
		break;
	case 13:
		throw "not implemented";
		break;
	case 14: 
	{
		*a2 = CMoAttachments::AttachMatrix;
		a2->_41 = 0.0;
		a2->_42 = 0.0;
		a2->_43 = 0.0;
		a2->_44 = 1.0;
		D3DVECTOR v55{};
		proc->VirtProcessor15(&v55, a2, &XiZone::zone->field_B4);
		proc->MatrixIdentity(a2);
		a2->_41 = v55.x;
		a2->_42 = v55.y;
		a2->_43 = v55.z;
	}
		break;
	case 15:
	{
		*a2 = CMoAttachments::AttachMatrix;
		a2->_41 = 0.0;
		a2->_42 = 0.0;
		a2->_43 = 0.0;
		a2->_44 = 1.0;
		D3DVECTOR v55{};
		proc->VirtProcessor15(&v55, a2, &XiZone::zone->field_D4);
		proc->MatrixIdentity(a2);
		a2->_41 = v55.x;
		a2->_42 = v55.y;
		a2->_43 = v55.z;
	}
		break;
	case 16:
		throw "not implemented";
		break;
	case 17:
		throw "not implemented";
		break;
	case 18:
		throw "not implemented";
		break;
	case 19:
		throw "not implemented";
		break;
	case 20:
		throw "not implemented";
		break;
	case 21:
		throw "not implemented";
		break;
	case 22:
		throw "not implemented";
		break;
	case 23:
		throw "not implemented";
		break;
	case 24:
		throw "not implemented";
		break;
	case 25:
		throw "not implemented";
		break;
	case 26:
		throw "not implemented";
		break;
	case 27:
		throw "not implemented";
		break;
	default:
		proc->MatrixIdentity(a2);
		break;
	}

	proc->MatrixPointers[index + 1] = f2;
	proc->MatrixPointers[index] = f1;
	proc->MatrixPointerIndex -= 2;
	return retval;
}

void CMoAttachments::ClearCasterTarget() {
	if (this->GetMasterCaster() != nullptr) {
		this->ClearCaster(true);
	}

	if (this->GetMasterTarget() != nullptr) {
		this->ClearTarget(true);
	}

	this->Init();
}
bool FFXI::CYy::CMoAttachments::CheckFlags()
{
	int v1{};

	v1 = (this->field_4 & 0xF) + 16 * ((this->field_4 >> 16) & 1);
	return v1 != 20 && (v1 >= 1 && v1 < 14 || v1 > 15);
}
CXiActor* FFXI::CYy::CMoAttachments::GetMasterTarget()
{
	if (this->TargetSubStruct.field_8 == 0x4000000) {
		return this->Target;
	}

	if (this->TargetSubStruct.GetSearchActor() == this->Target)
		return this->Target;

	return nullptr;
}

CXiActor* FFXI::CYy::CMoAttachments::GetMasterCaster()
{
	if (this->TargetSubStruct.field_8 == 0x4000000) {
		return this->Caster;
	}

	if (this->TargetSubStruct.GetSearchActor() == this->Caster)
		return this->Caster;

	return nullptr;
}

CXiActor* FFXI::CYy::CMoAttachments::GetCasterCheckDoll()
{
	CXiActor* caster = this->GetMasterCaster();
	if (caster == nullptr)
		return nullptr;

	while (caster->IsKindOf(&CXiDollActor::CXiDollActorClass) == true) {
		exit(0x1003AA8E);

		//sub //TODO doll actor
	}

	return caster;
}

CXiActor* FFXI::CYy::CMoAttachments::GetTargetCheckDoll()
{
	CXiActor* target = this->GetMasterCaster();
	if (target == nullptr)
		return nullptr;

	while (target->IsKindOf(&CXiDollActor::CXiDollActorClass) == true) {
		exit(0x1003AA8E);

		//sub //TODO doll actor
	}

	return target;
}

void FFXI::CYy::CMoAttachments::SetCaster(CXiActor* a2)
{
	CXiActor* caster = this->GetMasterCaster();
	if (caster == a2)
		return;

	if (caster != nullptr) {
		caster->VirtActor240(this);
	}

	this->CasterSubStruct.field_4 = 0;
	this->CasterSubStruct.field_8 = 0x4000000;
	this->Caster = a2;

	if (a2 != nullptr) {
		if (a2->AtelBuffer != nullptr) {
			unsigned int thing = (a2->field_88 >> 3) & 1;
			if (thing == 0) {
				this->CasterSubStruct.DoSomething(this->Caster);
			}
			this->CastAttach = a2->CasterAttachments;
			a2->CasterAttachments = this;
		}
	}
}

void FFXI::CYy::CMoAttachments::SetTarget(CXiActor* a2)
{
	CXiActor* target = this->GetMasterTarget();
	if (target == a2)
		return;

	if (target != nullptr) {
		target->VirtActor241(this);
	}

	this->TargetSubStruct.field_4 = 0;
	this->TargetSubStruct.field_8 = 0x4000000;
	this->Target = a2;

	if (a2 != nullptr) {
		if (a2->AtelBuffer != nullptr) {
			unsigned int thing = (a2->field_88 >> 3) & 1;
			if (thing == 0) {
				this->TargetSubStruct.DoSomething(this->Target);
			}
			this->TargAttach = a2->TargetAttachments;
			a2->TargetAttachments = this;
		}
	}
}

void FFXI::CYy::CMoAttachments::ClearCaster(bool a2)
{
	if (a2 == true) {
		CXiActor* caster = this->GetMasterCaster();
		if (caster != nullptr) {
			caster->VirtActor240(this);
		}
	}
	this->Caster = nullptr;
	this->CasterSubStruct.field_4 = 0;
	this->CasterSubStruct.field_8 = 0x4000000;
}

void FFXI::CYy::CMoAttachments::ClearTarget(bool a2)
{
	if (a2 == true) {
		CXiActor* target = this->GetMasterTarget();
		if (target != nullptr) {
			target->VirtActor241(this);
		}
	}
	this->Target = nullptr;
	this->TargetSubStruct.field_4 = 0;
	this->TargetSubStruct.field_8 = 0x4000000;
}

void FFXI::CYy::CMoAttachments::MixFlags(CMoAttachments* a2)
{
	int v2{}, v3{}, v4{}, v5{}, v6{}, v7{}, v8{};

	v2 = this->field_4 ^ ((unsigned __int8)this->field_4 ^ (unsigned __int8)a2->field_4) & 0xF;
	this->field_4 = v2;
	v3 = v2 ^ ((unsigned __int16)v2 ^ (unsigned __int16)a2->field_4) & 0x3F0;
	this->field_4 = v3;
	v4 = v3 ^ ((unsigned __int16)v3 ^ (unsigned __int16)a2->field_4) & 0xFC00;
	this->field_4 = v4;
	v5 = v4 & 0xFFFEFFFF ^ (a2->field_4 & 0x10000);
	this->field_4 = v5;
	v6 = v5 ^ (v5 ^ a2->field_4) & 0x40000;
	this->field_4 = v6;
	v7 = v6 ^ (v6 ^ a2->field_4) & 0x80000;
	this->field_4 = v7;
	v8 = v7 ^ (v7 ^ a2->field_4) & 0xF00000;
	this->field_4 = v8;
	this->field_4 = v8 & 0xF0FFFFFF ^ (a2->field_4 & 0xF000000);
}
