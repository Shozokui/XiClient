#include "CYyGenerator.h"
#include "CYyGeneratorClone.h"
#include "CXiActor.h"
#include "CYyDb.h"
#include "CNtTimeSys.h"
#include "CTsZoneMap.h"
#include "CYyResourceFile.h"
#include "YmCombineWeather.h"
#include "CMoResourceMng.h"
#include "Globals.h"
#include "CMoMmb.h"
#include "CMoDx.h"
#include "MmbListNode.h"
#include "CMoElem.h"
#include "CMoVtx.h"
#include "CMoD3aElem.h"
#include "CMoD3mSpecialElem.h"
#include "CMoDistModelElem.h"
#include "CMoD3mSpecularElem.h"
#include "CMoD3mElem.h"
#include "CMoD3bElem.h"
#include "CMoLfdElem.h"
#include "CMoDistMorphElem.h"
#include "CMoPointLightProgElem.h"
#include "CMoD3a.h"
#include "CMoDistElem.h"
#include "CYySoundElem.h"
#include "CMoKeyframe.h"
#include "ResourceList.h"
#include "GeneratorTask.h"
#include "CMoTaskMng.h"
#include "BaseProcessor.h"
#include "RegistryConfig.h"
#include "CYyCamMng2.h"
#include "XiZone.h"
#include "CXiSkeletonActor.h"
#include <iostream>
#include "CMoSphRes.h"
using namespace FFXI::CYy;

bool CYyGenerator::some_generator_scalar_flag{ false };
unsigned char* emulate_elem_status{ nullptr };

//Local funcs
CMoElem* HandleOne(CYyGenerator* gen, unsigned int* pos, CMoElem* a2, CMoResource** v508) {
	unsigned char modelType = *((unsigned char*)pos + 33);
	unsigned int modelTypeCopy = modelType;
	switch (modelTypeCopy) {
	case FFXI::Constants::Enums::ResourceType::Vum:
	case FFXI::Constants::Enums::ResourceType::Om1:
	case FFXI::Constants::Enums::ResourceType::D3m:
		modelType = FFXI::Constants::Enums::ResourceType::D3m;
		break;
	case FFXI::Constants::Enums::ResourceType::Anm:
	case FFXI::Constants::Enums::ResourceType::D3a:
		modelType = FFXI::Constants::Enums::ResourceType::D3a;
		break;
	case FFXI::Constants::Enums::ResourceType::Mmd:
		modelType = FFXI::Constants::Enums::ResourceType::D3b;
		break;
	}

	CMoElem* elem{ nullptr };
	char* mem{ nullptr };
	unsigned char ExtraSize = *((unsigned char*)pos + 32);
	switch (modelType) {
	case FFXI::Constants::Enums::ResourceType::D3m:
		if ((pos[1] & 0x00200000) != 0) {
			exit(0x1004FFD2);
			mem = CMoElem::ElemAllocate(sizeof(CMoD3mSpecialElem), ExtraSize);
			if (mem != nullptr)
				elem = new (mem) CMoD3mSpecialElem();
		}
		else if ((pos[1] & 0x00100000) != 0) {
			exit(0x10050005);
			mem = CMoElem::ElemAllocate(sizeof(CMoDistModelElem), ExtraSize);
			if (mem != nullptr)
				elem = new (mem) CMoDistModelElem();
		}
		else if ((pos[1] & 0x01000000) != 0) {
			exit(0x1005001C);
			mem = CMoElem::ElemAllocate(sizeof(CMoD3mSpecularElem), ExtraSize);
			if (mem != nullptr)
				elem = new (mem) CMoD3mSpecularElem();
		}
		else {
			mem = CMoElem::ElemAllocate(sizeof(CMoD3mElem), ExtraSize);
			if (mem != nullptr)
				elem = new (mem) CMoD3mElem();
		}
		break;
	case FFXI::Constants::Enums::ResourceType::D3a:
	{
		if (gen->CheckFlag29() == true) {
			exit(0x11);
		}
		else if ((pos[1] & 0x200000) != 0) {
			exit(0x12);
		}
		else {
			mem = CMoElem::ElemAllocate(sizeof(CMoD3aElem), ExtraSize);
			if (mem != nullptr)
				elem = new (mem) CMoD3aElem();
		}
	}
	break;
	case FFXI::Constants::Enums::ResourceType::Distprog:
		if (gen->CheckFlag29() == true) {
			exit(0x1005009C);
		}
		else {
			mem = CMoElem::ElemAllocate(sizeof(CMoDistElem), ExtraSize);
			if (mem != nullptr)
				elem = new (mem) CMoDistElem();
		}
		break;
	case FFXI::Constants::Enums::ResourceType::D3b:
		if ((pos[1] & 0x100000) != 0) {
			exit(0x1004FF91);
			mem = CMoElem::ElemAllocate(sizeof(CMoDistMorphElem), ExtraSize);
			if (mem != nullptr)
				elem = new (mem) CMoDistMorphElem();
		}
		else {
			mem = CMoElem::ElemAllocate(sizeof(CMoD3bElem), ExtraSize);
			if (mem != nullptr)
				elem = new (mem) CMoD3bElem();
		}
		break;
	case FFXI::Constants::Enums::ResourceType::Lfd:
		mem = CMoElem::ElemAllocate(sizeof(CMoLfdElem), ExtraSize);
		if (mem != nullptr)
			elem = new (mem) CMoLfdElem();
		break;
	case FFXI::Constants::Enums::ResourceType::Sep:
		mem = CMoElem::ElemAllocate(sizeof(CYySoundElem), ExtraSize);
		if (mem != nullptr)
			elem = new (mem) CYySoundElem();
		if (v508 != nullptr) {
			CMoResource::DoTheThing(*v508);
		}
		break;
	case FFXI::Constants::Enums::ResourceType::Pointlightprog:
	{
		int v239{};
		if (a2 != nullptr) {
			v239 = ((CMoPointLightProgElem*)a2)->field_194;
		}
		else if (gen->IsKindOf(&CYyGeneratorClone::CYyGeneratorCloneClass) == true) {
			CYyGeneratorClone* clone = (CYyGeneratorClone*)gen;
			v239 = (*clone->field_F4)->Header.FourCC;
		}
		else {
			v239 = gen->Header.FourCC;
		}
		mem = CMoElem::ElemAllocate(sizeof(CMoPointLightProgElem), ExtraSize);
		if (mem != nullptr)
			elem = new (mem) CMoPointLightProgElem(v239, gen->field_C4);
	}
	break;
	default:
		mem = CMoElem::ElemAllocate(sizeof(CMoElem), ExtraSize);
		if (mem != nullptr)
			elem = new (mem) CMoElem();
		std::cout << "Unhandled ELEMGEN1: 0x" << std::hex << (int)modelType << std::endl;
		break;
	}

	elem->MMBFlag = *((unsigned char*)pos + 40);
	elem->TaskMngShift4(0x0B);
	elem->LinkToGenerator(gen);
	BaseProcessor* proc = FFXI::CYyDb::g_pCYyDb->pMoProcessor;
	
	if ((pos[1] & 0x800000) == 0) {
		elem->field_108 = nullptr;
	}
	else {
		elem->field_108 = (FFXI::Math::WMatrix*) CYyObject::WrapPlus12(96, FFXI::Constants::Enums::MEM_MODE::Lower);

		if (gen->field_A4 != nullptr) {
			*elem->field_108 = *gen->field_A4;
			*elem->GetFitPosScale() = gen->PosScale;
			*elem->GetFitModelScale() = gen->ModelScale;
		}
		else if (a2 != nullptr) {
			proc->MatrixIdentity(elem->field_108);
			*elem->GetFitPosScale() = gen->PosScale;
			*elem->GetFitModelScale() = gen->ModelScale;
		}
	}
	
	CMoResource::DoTheThing(gen);
	elem->field_170 = 0;
	elem->ModelType = modelType;
	elem->field_10C = pos[1];
	elem->res = v508;

	char frc[5];
	frc[4] = 0;
	sprintf_s(frc, "%c%c%c%c", (gen->Header.FourCC >> 0) & 0xFF, (gen->Header.FourCC >> 8) & 0xFF, (gen->Header.FourCC >> 16) & 0xFF, (gen->Header.FourCC >> 24) & 0xFF);
	std::cout << "Gen " << frc << " created new " << std::dec << (int)modelType << " using resource ";
	char type = *((unsigned char*)pos + 40);
	if (type == 0) {
		if (v508 != nullptr) {
			std::cout << (char)((*v508)->Header.FourCC >> 0);
			std::cout << (char)((*v508)->Header.FourCC >> 8);
			std::cout << (char)((*v508)->Header.FourCC >> 16);
			std::cout << (char)((*v508)->Header.FourCC >> 24);
			std::cout << "( " << ((*v508)->Header.Type_Size & 0x7F) << " )" << std::endl;
		}
		else
			std::cout << "0" << std::endl;
	}
	else if (type == 1) {
		char nm[17];
		nm[16] = 0;
		memcpy_s(nm, sizeof(nm), ((MmbListNode*)v508)->name, sizeof(((MmbListNode*)v508)->name));
		std::cout << "MMB " << nm << std::endl;
	}
	

	elem->field_120 = 0.0;
	elem->field_124 = 0.0;
	//Can just set this to zero?
	elem->field_17B ^= elem->field_17B & 3;

	elem->field_54.x = *((float*)pos + 5);
	elem->field_54.y = *((float*)pos + 6);
	elem->field_54.z = *((float*)pos + 7);
	elem->field_168 = 1.0;

	if ((pos[1] & 0x4000000) != 0) {
		FFXI::Math::WMatrix* v516{};
		proc->PopFromStack(&v516);
		FFXI::CYyDb::g_pCYyDb->g_pCMoDx->GetViewTransform(v516);
		proc->VirtProcessor1(v516);
		elem->field_54.z *= -1.0;
		proc->VirtProcessor15(&elem->field_54, v516, &elem->field_54);
		proc->PushToStack(v516);
	}

	elem->Life = (float)gen->GetLife();
	unsigned short randLife = *((unsigned short*)pos + 18);
	if (randLife != 0) {
		unsigned int add = FFXI::CYyDb::g_pCYyDb->pCMoResourceMng->rng.uirand(randLife);
		elem->Life += (double)add;
	}

	elem->field_114 = elem->Life;
	elem->field_16C = 72;
	elem->field_16E = 0;
	elem->field_128 = 0.0;

	gen->field_9C += 1;
	return elem;
}

float CurTime() {
	return FFXI::XiDateTime::current_time_t;
}
//~Local funcs
float FFXI::CYy::CYyGenerator::GetSomeGeneratorScalar()
{
	if (CYyGenerator::some_generator_scalar_flag == true) {
		return 1.0;
	}

	return FFXI::Config::MainRegistryConfig::SomeGeneratorScalar;
}
FFXI::CYy::CYyGenerator::CYyGenerator()
{
	memset(&this->field_70, 0, 0x50);
}

FFXI::CYy::CYyGenerator::~CYyGenerator()
{
	ResourceList v5{};
	v5.PrepareFromLastCreated(CYyDb::g_pCYyDb->pCMoResourceMng, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
	while (true) {
		CMoResource* next = v5.GetNext(false);
		if (next == nullptr)
			break;
		if ((next->PreHeader.field_16 & 4) != 0) {
			if (next->Header.RPLHead == this->PreHeader.PointerToThisInRPL) {
				CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(next->PreHeader.PointerToThisInRPL);
				v5.PrepareFromLastCreated(CYyDb::g_pCYyDb->pCMoResourceMng, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
			}
		}
	}

	this->CMoAttachments::ClearCasterTarget();
	this->KillAll(nullptr);
	if (this->field_A8 != nullptr) {
		delete this->field_A8;
		this->field_A8 = nullptr;
	}

	if (this->field_A4 != 0) {
		CYyObject::UnwrapPlus12(this->field_A4);
		this->field_A4 = nullptr;
	}
}

void FFXI::CYy::CYyGenerator::ConstructFromData(char* a2)
{
	int* intData = (int*)a2;
	float* floatData = (float*)a2;
	memset(&this->field_70, 0, 0x50);
	memcpy(&this->Header, a2, sizeof(this->Header));

	this->CMoAttachments::field_4 = intData[4];
	this->CMoAttachments::field_8 = intData[5];
	this->CMoAttachments::Caster = (CXiActor*)intData[6];
	this->CMoAttachments::Target = (CXiActor*)intData[7];

	this->field_9C = 0;
	this->field_70 = floatData[8];
	this->field_74 = intData[9];
	this->field_98 = (unsigned char*)intData[11];

	int size = 16 * (((intData[1] >> 7) & 0x7FFFF) - 6);
	memcpy(&this->field_C0, intData + 24, size);

	this->CMoAttachments::Init();
}

void FFXI::CYy::CYyGenerator::Open()
{
	this->Script1 = (unsigned int*)((int)&this->field_C0 + ((int)this->Script1 - 0x60));
	this->Script2 = (unsigned int*)((int)&this->field_C0 + ((int)this->Script2 - 0x60));
	this->Script3 = (unsigned int*)((int)&this->field_C0 + ((int)this->Script3 - 0x60));
	this->Script4 = (unsigned int*)((int)&this->field_C0 + ((int)this->Script4 - 0x60));

	this->field_DC = 0;
	this->field_B0 = nullptr;

	this->CMoAttachments::ClearCaster(false);
	this->CMoAttachments::ClearTarget(false);

	this->PosScale = { 1.0, 1.0, 1.0 };
	this->ModelScale = { 1.0, 1.0, 1.0 };
	
	if ((this->Header.FourCC & 0xFF) != '~') {
		this->field_DE = 0;
		this->field_DF = 0;
	}
	else {
		CMoResource* model = (CMoResource*)this->GetModel();
		//added this check since getmodel seems to return void*
		if (*((unsigned char*)this->Script2 + 40) == 0) {
			if (model != nullptr && (model->Header.Type_Size & 0x7F) == FFXI::Constants::Enums::ResourceType::Sep) {
				this->field_DE = 0;
				this->field_DF = 0;
			}
			else {
				this->field_DE = (((this->Header.FourCC >> 8) & 0xFF) - '0') & 0x7B | 0x80;
			}
		}
	}

	CMoResource* cur = this;
	CMoResource* prev{ nullptr };
	bool v7 = (this->flags & 0x1000) != 0;
	while (true) {
		prev = cur;
		cur = *cur->Header.RPLHead;
		if (cur->Header.FourCC == 'taew'
			|| (cur->Header.Type_Size & 0x80000000) != 0) {
			v7 = false;
			this->field_DE |= 0x83;
			if (prev->Header.FourCC == 'wyrd') {
				this->field_DE |= 0x40;
			}
			else if (prev->Header.FourCC == 'tsud') {
				this->field_DE |= 0x20;
			}
			break;
		}
		if (cur == *cur->Header.RPLHead) {
			break;
		}
	}
	
	int index = CYyDb::g_pCYyDb->pCMoResourceMng->GetNumFileIndex();
	int fourcc = this->Header.FourCC;
	if (index == 0xDE35) {
		this->field_DF |= 1;
	}
	else {
		if (index == 0x147B7
		|| index == 0x147B8
		|| index == 0x147BF
		|| index == 0x147C0) {
			if (fourcc == '11b0'
				|| fourcc == '21b0'
				|| fourcc == '12b0'
				|| fourcc == '22b0'
				|| fourcc == '13b0'
				|| fourcc == '23b0')
			{
				this->field_DF |= 2u;
			}
		} 
		if (index == 0x147B9) {
			if (fourcc == '14bp') {
				this->field_DF |= 2;
			}
		}
		else {
			if (index == 0x147C1
			|| index == 0x147C2
			|| index == 0x147C3
			|| index == 0x147C4) {
				if (fourcc == '01b0' 
				|| fourcc == '11b0' 
				|| fourcc == '21b0' 
				|| fourcc == '02b0' 
				|| fourcc == '12b0'
				|| fourcc == '22b0') {
					this->field_DF |= 2;
				}
			}
		}
	}

	if (v7 == true) {
		int v18 = (this->CMoAttachments::field_4 & 0xF) + 16 * ((this->CMoAttachments::field_4 >> 16) & 0x01);
		if (v18 == 20
			|| (v18 < 1 || v18 >= 14) && v18 <= 15) {
			this->CMoAttachments::SetCaster(nullptr);
			this->CMoAttachments::SetTarget(nullptr);
			this->Activate();
		}
	}

	if (this->FindFirst(0x7B) == nullptr) {
		this->field_98 = 0;
	}
}

void FFXI::CYy::CYyGenerator::Close()
{
	if (this->field_A8 != nullptr) {
		delete this->field_A8;
		this->field_A8 = nullptr;
	}

	this->SomeClean2Helper();
}

FFXI::Math::WMatrix* FFXI::CYy::CYyGenerator::VRes4()
{
	if (this->field_A4 != nullptr) {
		this->field_A4;
	}

	this->field_A4 = (FFXI::Math::WMatrix*)CYyObject::WrapPlus12(sizeof(FFXI::Math::WMatrix), FFXI::Constants::Enums::MEM_MODE::Lower);
	CYyDb::g_pCYyDb->pMoProcessor->MatrixIdentity(this->field_A4);

	this->PosScale = { 1.0, 1.0, 1.0 };
	this->ModelScale = { 1.0, 1.0, 1.0 };

	return this->field_A4;
}

bool FFXI::CYy::CYyGenerator::VirtAttach3()
{
	this->ClearCaster(true);
	return false;
}

bool FFXI::CYy::CYyGenerator::VirtAttach4()
{
	this->ClearTarget(true);
	return false;
}

CYyGeneratorClone* CYyGenerator::Clone(CXiActor* a2, CXiActor* a3, int a4) {
	char* mem = StorageLists::instance->GetOrUpper(sizeof(CYyGeneratorClone), FFXI::Constants::Enums::MEM_MODE::Lower);
	CMoResourceMng::SomeByte = 1;
	CYyGeneratorClone* clone{ nullptr };
	if (mem != nullptr) {
		clone = new (mem) CYyGeneratorClone();
		clone->field_F0 = 0;
		clone->field_F8 = 0;
	}

	clone->Copy(this);

	clone->PosScale = { 1.0, 1.0, 1.0 };
	clone->ModelScale = { 1.0, 1.0, 1.0 };

	clone->CMoAttachments::Init();
	clone->PreHeader.field_16 |= 5;
	clone->field_9C = 0;
	clone->Header.Type_Size |= 0x4000000;
	clone->field_F4 = this->PreHeader.PointerToThisInRPL;

	CYyDb::g_pCYyDb->pCMoResourceMng->Link(clone);
	clone->CMoAttachments::SetCaster(a2);
	clone->CMoAttachments::SetTarget(a3);
	clone->Header.FourCC = 'nolc';
	clone->field_9C = 0;
	clone->flags &= 0xFFFFF7FF;
	clone->Header.RPLHead = this->PreHeader.PointerToThisInRPL;
	clone->PreHeader.field_14 &= 0x8000u;
	clone->Header.Type_Size &= ~0x8000000u;
	clone->DoSomething2();
	if (a4 != 0) {
		clone->Activate();
	}

	if (this->Script1 == nullptr)
		return clone;

	unsigned int* pos = this->Script1;
	while (pos[0] != 0) {
		if ((pos[0] & 0xFF) == 4) {
			if (pos[1] == 0) {
				this->FindResourceUnder((CMoResource***)(pos + 1), FFXI::Constants::Enums::ResourceType::Keyframe, pos[2]);
			}
			*((float*)pos + 4) = (float)(this->flags & 0x1FF);
		}
		pos += (pos[0] >> 8) & 0x1F;
	}

	return clone;
}

void FFXI::CYy::CYyGenerator::Copy(CYyGenerator* a2)
{
	this->Header = a2->Header;
	this->CMoAttachments::MixFlags(a2);

	this->field_70 = a2->field_70;
	this->field_74 = a2->field_74;

	this->PosScale = a2->PosScale;
	this->ModelScale = a2->ModelScale;

	this->field_90 = a2->field_90;
	this->field_94 = a2->field_94;
	this->field_98 = a2->field_98;
	this->field_9C = a2->field_9C;
	this->field_A0 = a2->field_A0;
	this->field_A4 = nullptr;
	this->field_A8 = 0;
	this->field_AC = 0;
	this->field_B0 = nullptr;
	this->field_B4 = a2->field_B4;
	this->field_B8 = a2->field_B8;
	this->field_BC = a2->field_BC;

	this->field_C0 = a2->field_C0;
	this->field_C4 = a2->field_C4;
	this->field_C8 = a2->field_C8;
	this->field_CC = a2->field_CC;
	this->field_D0 = a2->field_D0;
	this->field_D4 = a2->field_D4;
	this->field_D6 = a2->field_D6;

	//not sure why they do it like this but flag sets for 0x18
	int v5 = this->flags ^ ((unsigned __int16)this->flags ^ (unsigned __int16)a2->flags) & 0x1FF;
	this->flags = v5;
	int v6 = v5 ^ ((unsigned __int16)v5 ^ (unsigned __int16)a2->flags) & 0x200;
	this->flags = v6;
	v6 &= 0xFFFFF3FF;
	int v7 = a2->flags & 0x400 ^ v6;
	this->flags = v7;
	int v8 = v7 ^ ((unsigned __int16)v7 ^ (unsigned __int16)a2->flags) & 0x1000;
	this->flags = v8;
	int v9 = v8 ^ ((unsigned __int16)v8 ^ (unsigned __int16)a2->flags) & 0x2000;
	this->flags = v9;
	int v10 = v9 ^ (v9 ^ a2->flags) & 0xFFFC000;
	this->flags = v10;
	int v11 = v10 ^ (v10 ^ a2->flags) & 0x10000000;
	this->flags = v11;
	int v12 = v11 ^ (v11 ^ a2->flags) & 0x20000000;
	this->flags = v12;
	int v13 = v12 ^ (v12 ^ a2->flags) & 0x40000000;
	this->flags = v13;
	this->flags = a2->flags ^ (a2->flags ^ v13) & 0x7FFFFFFF;

	this->field_DC = a2->field_DC;
	this->field_DE = a2->field_DE;
	this->field_DF = a2->field_DF;

	this->Script1 = a2->Script1;
	this->Script2 = a2->Script2;
	this->Script3 = a2->Script3;
	this->Script4 = a2->Script4;
}

void FFXI::CYy::CYyGenerator::GetPos(D3DXVECTOR3* a2)
{
	float* pos = (float*)this->FindFirst(1);
	a2->x = pos[5];
	a2->y = pos[6];
	a2->z = pos[7];
}

bool FFXI::CYy::CYyGenerator::CheckFlag29()
{
	return (this->flags & 0x20000000) == 0x20000000;
}

void FFXI::CYy::CYyGenerator::GenUnk1()
{
	this->field_9C = 0;
	this->field_CC = 0.0;
	this->field_C8 = 0.0;
	this->flags |= 0x800;

	if ((this->field_DC & 1) != 0) {
		this->field_DC &= ~1u;
		if (this->field_A8 != nullptr) {
			this->field_A8->field_14 = 0;
		}
	}
	else {
		this->field_DC &= ~1u;
		if (this->field_A8 != nullptr) {
			delete this->field_A8;
			this->field_A8 = nullptr;
		}

		char* mem = StorageLists::instance->Get(sizeof(GeneratorTask), FFXI::Constants::Enums::MEM_MODE::Ex);
		CMoTaskMng::DeleteThisTask = true;
		GeneratorTask* gtask = new (mem) GeneratorTask();
		this->field_A8 = gtask;
		gtask->field_3C = 0;
		CYyDb::g_pCYyDb->pCMoTaskMng->DoSomething(gtask);
		gtask->field_34 = (CYyGenerator**)this->PreHeader.PointerToThisInRPL;
		gtask->field_38 = (CYyGenerator**)this->PreHeader.PointerToThisInRPL;
		gtask->Param1 = (int)this->PreHeader.PointerToThisInRPL;
		gtask->Param2 = 'Mg\0\0';
	}
}

void* FFXI::CYy::CYyGenerator::GetModel()
{
	if (this->Script2[2] == 0) {
		int modelType = *((unsigned char*)this->Script2 + 33);
		int modelTypeCopy = modelType;
		switch (modelTypeCopy) {
		case FFXI::Constants::Enums::ResourceType::Vum:
		case FFXI::Constants::Enums::ResourceType::Om1:
		case FFXI::Constants::Enums::ResourceType::D3m:
			modelType = FFXI::Constants::Enums::ResourceType::D3m;
			break;
		case FFXI::Constants::Enums::ResourceType::Anm:
		case FFXI::Constants::Enums::ResourceType::D3a:
		case FFXI::Constants::Enums::ResourceType::Lfd:
			modelType = FFXI::Constants::Enums::ResourceType::D3a;
			break;
		case FFXI::Constants::Enums::ResourceType::Mmd:
			modelType = FFXI::Constants::Enums::ResourceType::D3b;
			break;
		}
		*((unsigned char*)this->Script2 + 40) = 0;
		if (modelType == Constants::Enums::ResourceType::Damvalueprog ||
			modelType == Constants::Enums::ResourceType::Nullprog ||
			modelType == Constants::Enums::ResourceType::Pointlightprog ||
			modelType == Constants::Enums::ResourceType::Ringprog ||
			modelType == Constants::Enums::ResourceType::Distprog) {
			this->Script2[2] = 1;
			return nullptr;
		}

		CMoResource** res{};
		int fourCC = this->Script2[3];
		this->FindResourceUnder(&res, modelType, fourCC);
		this->Script2[2] = (unsigned int)res;
		if (res == nullptr
			&& modelType == FFXI::Constants::Enums::ResourceType::D3m) {
			this->Script2[2] = 0;
			CMoResource** node = this->PreHeader.PointerToThisInRPL;
			while (node != nullptr) {
				CMoMmb** mmb{ nullptr };
				(*node)->FindResourceUnder((CMoResource***) &mmb, Constants::Enums::ResourceType::Mmb, fourCC);
				if (mmb != nullptr && *mmb != nullptr) {
					MmbListNode* mmbln = CYyDb::g_pCYyDb->g_pTsZoneMap->FindMmb((char*)&(*mmb)->field_30);
					this->Script2[2] = (unsigned int)mmbln;
					*((unsigned char*)this->Script2 + 40) = 1;
					break;
				}
				if ((*node)->Header.RPLHead == node)
					break;
				node = (*node)->Header.RPLHead;
			}
		}
	}
	if (this->Script2[2] > 1)
		return *(void**)this->Script2[2];

	return nullptr;
}

unsigned char FFXI::CYy::CYyGenerator::GetModelType()
{
	unsigned char* data = this->FindFirst(1);
	return data[33];
}

unsigned char* FFXI::CYy::CYyGenerator::FindFirst(int a2)
{
	unsigned int* pos = this->Script2;
	if (pos == nullptr)
		return nullptr;

	while ((pos[0] & 0xFF) != a2) {
		if ((pos[0] & 0xFF) == 0)
			return nullptr;
		pos += ((pos[0] >> 8) & 0x1F);
		if (pos == nullptr)
			return nullptr;
	}
	return (unsigned char*)pos;
}

unsigned char* FFXI::CYy::CYyGenerator::GetElemStatus()
{
	unsigned char* retval = this->FindFirst(1);
	if (retval == nullptr) {
		memset(emulate_elem_status, 0, 4);
		return emulate_elem_status;
	}

	return retval + 4;
}

unsigned int Get11FC(unsigned int* a1) {
	return (a1[0] >> 11) & 0xFC;
}

void FFXI::CYy::CYyGenerator::ElemGenerate(CMoElem* a2)
{
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	ISAACRandom* rng = &CYyDb::g_pCYyDb->pCMoResourceMng->rng;
	FFXI::Math::WMatrix* pop{};
	proc->PopFromStack(&pop);

	CMoElem* elem{ nullptr };
	bool v495{ false };
	char* v4{ nullptr };
	D3DXVECTOR3 v492{}, v494{}, v498{};

	unsigned int* pos = this->Script2;
	while (pos != nullptr && (pos[0] & 0xFFu) != 0) {
		float* fpos = (float*)pos;
		switch (pos[0] & 0xFFu) {
		case 1:
			//instantiate new Elem object
		{
			unsigned int model = pos[2];
			if (model > 1) {
				elem = HandleOne(this, pos, a2, (CMoResource**)model);
			}
			else if (model == 0) {
				proc->PushToStack(pop);
				return;
			}
			else {
				elem = HandleOne(this, pos, a2, 0);
			}

			if (elem == nullptr) {
				proc->PushToStack(pop);
				return;
			}

			if (elem->Life == 0.0) {
				v495 = true;
			}

			v4 = (char*)elem + elem->field_176;
			v498 = elem->field_54;
		}
		break;
		case 0x02u:
		{
			unsigned int index = Get11FC(pos);
			if (this->CheckFlag29() == true) {
				memset(v4 + index, 0, 12);
			}
			else {
				memcpy_s(v4 + index, 12, pos + 1, 12);
			}
		}
		break;
		case 0x03u:
		case 0x0Cu:
		case 0x13u:
		{
			unsigned int index = Get11FC(pos);
			float* fdata = (float*)(v4 + index);
			fdata[0] += CMoResourceMng::rng.frand(*(float*)(pos + 1));
			fdata[1] += CMoResourceMng::rng.frand(*(float*)(pos + 2));
			fdata[2] += CMoResourceMng::rng.frand(*(float*)(pos + 3));
		}
			break;
		case 0x06:
			if (this->CheckFlag29() == false) {
				float rmax = fpos[1] + fpos[2];
				float v376 = rng->ufrand(rmax);
				float v383 = rng->frand(FFXI::Constants::Values::ANGLE_PI);
				float v377 = rng->frand(FFXI::Constants::Values::ANGLE_PI);
				proc->VirtProcessor17(pop, v377, v383, 0);

				if (this->field_90 != 0.0) {
					pop->_11 /= this->field_90;
				}

				v492 = { rmax, 0, 0 };
				proc->VirtProcessor15(&v494, pop, &v492);
				elem->field_54 += v494;
			}
			break;
		case 0x07u:
		{
			float rmax = fpos[1] + fpos[2];
			float v376 = rng->ufrand(rmax);
			float v383 = rng->frand(FFXI::Constants::Values::ANGLE_PI);
			float v377 = rng->frand(FFXI::Constants::Values::ANGLE_PI);
			proc->VirtProcessor17(pop, v377, v383, 0);

			FFXI::Math::WMatrix* v505{ nullptr }, * v507{ nullptr };
			proc->PopFromStack(&v505);
			proc->PopFromStack(&v507);
			proc->MatrixIdentity(v505);
			v505->_11 = fpos[3];
			v505->_22 = fpos[4];
			v505->_33 = fpos[5];
			proc->VirtProcessor17(v507, 0, fpos[7], fpos[6]);
			proc->MatrixMultiply(v505, v505, v507);
			proc->MatrixMultiply(pop, pop, v505);
			if (this->field_90 != 0.0) {
				pop->_11 /= this->field_90;
			}

			v492 = { rmax, 0, 0 };
			proc->VirtProcessor15(&v494, pop, &v492);
			elem->field_54 += v494;
			proc->PushToStack(v507);
			proc->PushToStack(v505);
		}
			break;
		case 0x08:
		if (this->CheckFlag29() == false) {
			D3DXVECTOR3 v648 = elem->field_54 - v498;
			proc->Vec3Normalize(&v648);
			v648.x *= fpos[1];
			v648.y *= fpos[1];
			v648.z *= fpos[1];
			unsigned int index = Get11FC(pos);
			*(float*)(v4 + index + 0) += v648.x;
			*(float*)(v4 + index + 4) += v648.y;
			*(float*)(v4 + index + 8) += v648.z;
		}
			break;
		case 0x09:
			elem->field_E0.x = fpos[1];
			elem->field_E0.y = fpos[2];
			elem->field_E0.z = fpos[3];
			break;
		case 0x0B:
			//fallthrough
		case 0x012:
		{
			unsigned int index = Get11FC(pos);
			memcpy_s(v4 + index, 12, pos + 1, 12);
		}
		break;
		case 0x0F:
			elem->field_EC.x = fpos[1];
			elem->field_EC.y = fpos[2];
			elem->field_EC.z = fpos[3];
			break;
		case 0x10u:
			elem->field_EC.x += CMoResourceMng::rng.ufrand(*(float*)(pos + 1));
			elem->field_EC.y += CMoResourceMng::rng.ufrand(*(float*)(pos + 2));
			elem->field_EC.z += CMoResourceMng::rng.ufrand(*(float*)(pos + 3));
			break;
		case 0x11u:
		{
			float rnd = CMoResourceMng::rng.ufrand(*(float*)(pos + 1));
			elem->field_EC.x += rnd;
			elem->field_EC.y += rnd;
			elem->field_EC.z += rnd;
		}
			break;
		case 0x16:
			elem->field_F8.MixSet(pos[1]);
			break;
		case 0x1D:
			if (elem->res != nullptr) {
				CMoD3a* d3a = (CMoD3a*)*elem->res;
				short* data = (short*)(d3a + 1);
				double v371 = elem->Life / (double)data[1];
				elem->field_11C = v371;
				elem->field_118 = v371;
			}
			break;
		case 0x1E:
			elem->field_16C = *((unsigned short*)pos + 2);
			break;
		case 0x1F:
		{
			float randmax = *(float*)(pos + 9);
			double rand = CMoResourceMng::rng.frand(randmax);
			rand += *(float*)(pos + 8);
			if (rand > FFXI::Constants::Values::ANGLE_PI) {
				rand -= FFXI::Constants::Values::ANGLE_2PI;
			}
			if (rand < FFXI::Constants::Values::ANGLE_MINUS_PI) {
				rand += FFXI::Constants::Values::ANGLE_2PI;
			}

			float v479{};
			short val = *(short*)(pos + 11);
			if (val == 0) {
				v479 = CMoResourceMng::rng.frand(FFXI::Constants::Values::ANGLE_PI);
			}
			else {
				val += 1;
				v479 = FFXI::Constants::Values::ANGLE_2PI * ((double)(this->field_9C % val) / (double)val - 0.5);
			}
			proc->VirtProcessor17(pop, 0.0, v479, rand);
			FFXI::Math::WMatrix* pop2{ nullptr }, * pop3{ nullptr };
			proc->PopFromStack(&pop2);
			proc->PopFromStack(&pop3);
			proc->MatrixIdentity(pop2);
			pop2->_11 = *(float*)(pos + 3);
			pop2->_22 = *(float*)(pos + 4);
			pop2->_33 = *(float*)(pos + 5);
			proc->VirtProcessor17(pop3, 0.0, *(float*)(pos + 7), *(float*)(pos + 6));
			proc->MatrixMultiply(pop2, pop2, pop3);
			proc->MatrixMultiply(pop, pop, pop2);
			FFXI::Math::WMatrix* sm = elem->GetSomeMatrix();
			if ((pos[10] & 1) != 0 && sm != nullptr) {
				*pop2 = *sm;
				pop2->_41 = 0.0; pop2->_42 = 0.0; pop2->_43 = 0.0; pop2->_44 = 1.0;
				proc->MatrixMultiply(pop2, pop2, &CYyDb::g_pCYyDb->g_pCMoDx->field_B90);
				proc->VirtProcessor1(pop2);
				proc->MatrixMultiply(pop, pop, pop2);
			}
			if (this->field_90 != 0.0) {
				pop->_11 /= this->field_90;
			}
			float v377 = CMoResourceMng::rng.ufrand(*(float*)(pos + 1) + *(float*)(pos + 2));
			D3DXVECTOR3 v491 = { v377, 0.0, 0.0 };
			D3DXVECTOR3 v493{};
			proc->VirtProcessor15(&v493, pop, &v491);
			elem->field_54 += v493;
			proc->PushToStack(pop3);
			proc->PushToStack(pop2);
		}
			break;
		case 0x27:
		{
			unsigned int index = Get11FC(pos);
			*(unsigned int*)(v4 + index) = pos[1];
			*(unsigned int*)(v4 + index + 4) = pos[3];
			*(float*)(v4 + index + 8) = elem->field_EC.x;
		}
			break;
		case 0x28:
		{
			unsigned int index = Get11FC(pos);
			*(unsigned int*)(v4 + index) = pos[1];
			*(unsigned int*)(v4 + index + 4) = pos[3];
			*(float*)(v4 + index + 8) = elem->field_EC.y;
		}
			break;
		case 0x29: 
		{
			unsigned int index = Get11FC(pos);
			*(unsigned int*)(v4 + index) = pos[1];
			*(unsigned int*)(v4 + index + 4) = pos[3];
			*(float*)(v4 + index + 8) = elem->field_EC.z;
		}
			break;
		case 0x2D:
		{
			unsigned int index = Get11FC(pos);
			unsigned int b3 = elem->field_F8.field_3;
			*(unsigned int*)(v4 + index) = pos[1];
			*(unsigned int*)(v4 + index + 4) = pos[3];
			*(float*)(v4 + index + 8) = (float)((double)b3 / 255.0);
		}
		break;
		case 0x30u:
			elem->field_128 = *(float*)(pos + 1);
			break;
		case 0x31u:
		{
			float randmax = fpos[1];
			float randf = CMoResourceMng::rng.frand(randmax);
			unsigned int index = Get11FC(pos);
			*(float*)(v4 + index + 0) = randf;
			*(float*)(v4 + index + 4) = randf;
			*(float*)(v4 + index + 8) = randf;
		}
			break;
		case 0x32u:
			elem->VirtElem2(*((float*)pos + 2));
			break;
		case 0x33u:
		case 0x36u:
		{
			unsigned int index = Get11FC(pos);
			*(unsigned int*)(v4 + index + 0) = pos[1];
			*(unsigned int*)(v4 + index + 4) = pos[3];
			*(unsigned int*)(v4 + index + 8) = 0;
		}
			break;
		case 0x34u:
		case 0x37u:
		{
			unsigned int index = Get11FC(pos);
			*(unsigned int*)(v4 + index + 0) = pos[1];
			*(unsigned int*)(v4 + index + 4) = pos[3];
			*(unsigned int*)(v4 + index + 8) = 0;
		}
		break;
		case 0x35u:
		{
			unsigned int index = Get11FC(pos);
			*(unsigned int*)(v4 + index + 0) = pos[1];
			*(unsigned int*)(v4 + index + 4) = pos[3];
			*(unsigned int*)(v4 + index + 8) = 0;
		}
		break;
		case 0x41u:
		{
			D3DXVECTOR3 v510 = elem->field_54 - v498;
			proc->Vec3Normalize(&v510);
			float randmax = fpos[1];
			float randf = CMoResourceMng::rng.frand(randmax);
			v510.x *= randf;
			v510.y *= randf;
			v510.z *= randf;
			unsigned int index = (pos[0] >> 13) & 0x3F;
			*((float*)v4 + index + 0) += v510.x;
			*((float*)v4 + index + 1) += v510.x;
			*((float*)v4 + index + 2) += v510.x;
		}
			break;
		case 0x4Bu:
			if (elem->VirtElem1(FFXI::Constants::Enums::ElemType::Lfd) == true) {
				CMoLfdElem* lfd = (CMoLfdElem*)elem;
				lfd->field_194 = fpos[1];
				lfd->field_198 = fpos[2];
				unsigned int val = pos[3];
				lfd->field_19C = val & 0xFF000000;
				lfd->field_19C |= (val << 16) & 0xFF0000;
				lfd->field_19C |= val & 0x0000FF00;
				lfd->field_19C |= (val >> 16) & 0xFF;
			}
			break;
		case 0x4Cu:
			if (elem->VirtElem1(FFXI::Constants::Enums::ElemType::Sound) == true) {
				CYySoundElem* se = (CYySoundElem*)elem;
				if (se->IsNever() == true) {
					this->Unlink(false);
				}
				int c = ((this->CMoAttachments::field_4 & 0xF) + 16 * ((this->CMoAttachments::field_4 >> 16) & 1));
				if (c == 0) {
					se->field_1E9 = 1;
				}
				se->s_far = *(float*)(pos + 1);
				se->s_near = *(float*)(pos + 2);
				se->s_width = 0.0;
				se->field_1D0[0] = 64.0;
				se->field_1D0[1] = 0.0;
				se->field_1D0[2] = 0.0;
				se->field_1D0[3] = 0.0;
				se->field_1BE = 127;
			}
			break;
		case 0x54:
		{
			unsigned int index = (pos[0] >> 13) & 0x3F;
			char* m = v4 + 4 * index;
			CMoVtx** vtx = (CMoVtx**)pos[4];
			if (vtx != nullptr) {
				memcpy_s(m, 4, pos + 1, 4); //Copy keyframe resource
				memcpy_s(m + 4, 4, pos + 3, 4); //Copy ?
				memcpy_s(m + 28, 4, &vtx, 4); //Copy vtx resource
				memset(m + 12, 0, 12);
				(*vtx)->list.field_8 += 1;
			}

			vtx = *(CMoVtx***)(m + 28);
			if (vtx != nullptr && *vtx != nullptr) {
				D3DXVECTOR3 v491{};
				(*vtx)->list.GetSplinePos(&v491, 0.0);
				D3DXVECTOR3 v492{};
				v492.x = v491.x - *(float*)(m + 12);
				v492.y = v491.y - *(float*)(m + 16);
				v492.z = v491.z - *(float*)(m + 20);
				memcpy_s(m + 12, 12, &v492, sizeof(v492));
				elem->field_54 += v492;
			}
		}
			break;
		case 0x58:
			if (elem->VirtElem1(FFXI::Constants::Enums::ElemType::PointLight) == true) {
				CMoPointLightProgElem* pl = (CMoPointLightProgElem*)elem;
				pl->field_19C = *(float*)(pos + 1);
				pl->field_1A0 = *(float*)(pos + 2);
				pl->field_1A8 = *(float*)(pos + 3) + 1.0;
				pl->field_1A0 = *(float*)(pos + 4) + 1.0;
			}
			break;
		case 0x60u:
		case 0x62u:
		case 0x63u:
		case 0x66u:
		case 0x6Cu:
		case 0x6Du:
		case 0x6Fu:
		case 0x70u:
		{
			//Keyframe at pos1
			unsigned int index = Get11FC(pos);
			*(unsigned int*)(v4 + index) = pos[1];
			*(unsigned int*)(v4 + index + 4) = pos[3];
		}
		break;
		case 0x61u:
		case 0x65u:
		case 0x68u:
		case 0x6Eu:
		{
			unsigned int index = Get11FC(pos);
			*(unsigned int*)(v4 + index) = pos[1];
			*(unsigned int*)(v4 + index + 4) = pos[3];
		}
			break;
		case 0x6B:
			if (elem->VirtElem1(FFXI::Constants::Enums::ElemType::Sound) == true) {
				CYySoundElem* se = (CYySoundElem*)elem;
				CMoSphRes** sphres = (CMoSphRes**)pos[2];
				if (sphres == nullptr) {
					se->field_198 = nullptr;
				}
				else {
					se->field_198 = *sphres;
					D3DXVECTOR3 dir{};
					(*sphres)->path.GetDirection(&dir, &CYyDb::g_pCYyDb->CameraManager->field_60);
					se->field_54 = dir + CYyDb::g_pCYyDb->CameraManager->field_60;
				}
			}
			break;
		case 0x72u:
		{
			CXiActor* target = this->CMoAttachments::GetMasterTarget();
			elem->field_128 = *(float*)(pos + 1);
			if (target != nullptr
				&& target->IsKindOf(&CXiSkeletonActor::CXiSkeletonActorClass) == true) {

				float v509 = *(float*)(pos + 2);
				if (v509 == 0.0) {
					v509 = 1.0;
				}
				float v340 = target->GetDepthScale();
				elem->field_128 *= (v340 - 1.0) * v509 + 1.0;
			}
		}
			break;
		case 0x87u:
		{
			unsigned int index = (pos[0] >> 13) & 0x3F;
			char* m = v4 + 4 * index;
			memcpy_s(m, 12, &elem->field_54, sizeof(elem->field_54));
			memset(m + 12, 0, 12);
		}
			break;
		default:
			std::cout << "Unhandled GEN ELEM: 0x" << std::hex << (*pos & 0xFFu) << std::endl;
			break;
		}
		//LABEL_525
		pos += (pos[0] >> 8) & 0x1F;
	}

	//LABEL_527
	if (a2 != nullptr) {
		exit(0x10051427);
	}

	if ((elem->field_10C & 4) == 0
		|| (elem->field_10C & 8) == 0) {
		elem->CalcMatrix();
	}
	else {
		elem->CalcMatrix();
		elem->CalcTrans();
	}

	proc->PushToStack(pop);
}

float GetKeyframeFrame(CMoKeyframe*** a1, float a2) {
	if (*a1 == nullptr) {
		return 0.0;
	}

	CMoKeyframe* kf = **a1;
	if ((*((int*)a1 + 1) & 0xF) != 0) {
		return kf->GetSplineFrameValue(a2, *((float*)a1 + 2));
	}

	return kf->GetFrameValue(a2, *((float*)a1 + 2));
}

unsigned short GetDayOfWeek() {
	if (FFXI::CYyDb::g_pCYyDb->field_9 == 0 || FFXI::XiDateTime::event_flag == true) {
		FFXI::XiDateTime v2{};
		FFXI::XiDateTime::GetCurrentDateTime(&v2);
		return v2.Time / 0x4F1A00 % 8;
	}
	else {
		return FFXI::Network::CNtTimeSys::ntGameTimeGet() % 0x6C00 / 0xD80;
	}
}

unsigned short GetMoonShapeNo() 
{
	unsigned int retval{};
	if (FFXI::CYyDb::g_pCYyDb->field_9 == 0 || FFXI::XiDateTime::event_flag == true) {
		FFXI::XiDateTime v2{};
		FFXI::XiDateTime v3(2592000);
		FFXI::XiDateTime::GetCurrentDateTime(&v2);
		v2.SetTickFrame(v2.Time + v3.Time);
		retval = v2.Time / 0x4F1A00;
	}
	else {
		retval = FFXI::Network::CNtTimeSys::ntGameTimeGet() / 0xD80;
	}

	return retval / 7 % 12;
}

void FFXI::CYy::CYyGenerator::ElemIdle(CMoElem* a2)
{
	a2->field_138 = 1.0;
	
	FFXI::Math::WMatrix* pop{ nullptr };
	CYyDb::g_pCYyDb->pMoProcessor->PopFromStack(&pop);
	char* data = (char*)a2 + a2->field_176;
	float special{};

	unsigned int* pos = this->Script3;
	while (pos != nullptr) {
		switch (pos[0] & 0xFF) {
		case 0:
			//script end
			break;
		case 0x02:
		{
			unsigned int index = Get11FC(pos);
			D3DXVECTOR3 v428{};
			v428.x = *(float*)(data + index + 0);
			v428.y = *(float*)(data + index + 4);
			v428.z = *(float*)(data + index + 8);
			float t = CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
			v428.x *= t;
			v428.y *= t;
			v428.z *= t;
			a2->field_54 += v428;
		}
			break;
		case 0x03:
			if (this->CheckFlag29() == false) {
				unsigned int index = Get11FC(pos);
				float scale = CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
				*(float*)(data + index + 0) += scale * *(float*)(pos + 1);
				*(float*)(data + index + 4) += scale * *(float*)(pos + 2);
				*(float*)(data + index + 8) += scale * *(float*)(pos + 3);
			}
			break;
		case 0x05:
		{
			unsigned int index = Get11FC(pos);
			float scale = CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
			a2->field_E0.x += scale * *(float*)(data + index + 0);
			a2->field_E0.y += scale * *(float*)(data + index + 4);
			a2->field_E0.z += scale * *(float*)(data + index + 8);
			a2->field_187 = 1;
		}
			break;
		case 0x06:
		case 0x09:
		{
			unsigned int index = Get11FC(pos);
			float scale = CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
			*(float*)(data + index) += scale * *(float*)(pos + 1);
			*(float*)(data + index + 4) += scale * *(float*)(pos + 2);
			*(float*)(data + index + 8) += scale * *(float*)(pos + 3);
			a2->field_187 = 1;
		}
			break;
		case 0x08: 
		{
			double time = CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
			unsigned int index = Get11FC(pos);
			a2->field_EC.x += time * *(float*)(data + index);
			a2->field_EC.y += time * *(float*)(data + index + 4);
			a2->field_EC.z += time * *(float*)(data + index + 8);
			a2->field_187 = 1;
		}
			break;
		case 0x0D:
			if (a2->res != nullptr) {
				a2->field_118 -= CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
				if (a2->field_118 < 0.0) {
					if (a2->field_11C != 0.0) {
						a2->field_170 = 0;
						while (a2->field_118 < 0.0) {
							a2->field_118 += a2->field_11C;
							a2->field_170 += 1;
						}
					}
				}
			}
			break;
		case 0x0E:
			special = a2->field_114;
			if (special != 0.0) {
				special = 1.0 - (a2->Life / special);
			}
			break;
		case 0x15:
		{
			unsigned int index = (pos[0] >> 13) & 0x3F;
			float* fptr = (float*)data + index;
			CMoKeyframe** pkf = *(CMoKeyframe***)fptr;
			if (pkf == nullptr) {
				a2->field_EC.x = 0.0;
			}
			else if ((data[4 * index + 4] & 0xF) != 0) {
				a2->field_EC.x = (*pkf)->GetSplineFrameValue(special, fptr[2]);
				
			}
			else {
				a2->field_EC.x = (*pkf)->GetFrameValue(special, fptr[2]);
			}
			a2->field_187 = 1;
		}
			break;
		case 0x16:
		{
			unsigned int index = (pos[0] >> 13) & 0x3F;
			float* fptr = (float*)(data + 4 * index);
			CMoKeyframe** pkf = *(CMoKeyframe***)fptr;
			if (pkf == nullptr) {
				a2->field_EC.y = 0.0;
			}
			else if ((data[4 * index + 4] & 0xF) != 0) {
				a2->field_EC.y = (*pkf)->GetSplineFrameValue(special, fptr[2]);

			}
			else {
				a2->field_EC.y = (*pkf)->GetFrameValue(special, fptr[2]);
			}
			a2->field_187 = 1;
		}
			break;
		case 0x17:
		{
			unsigned int index = (pos[0] >> 13) & 0x3F;
			float* fptr = (float*)data + index;
			CMoKeyframe** pkf = *(CMoKeyframe***)fptr;
			if (pkf == nullptr) {
				a2->field_EC.z = 0.0;
			}
			else if ((data[4 * index + 4] & 0xF) != 0) {
				a2->field_EC.z = (*pkf)->GetSplineFrameValue(special, fptr[2]);

			}
			else {
				a2->field_EC.z = (*pkf)->GetFrameValue(special, fptr[2]);
			}
			a2->field_187 = 1;
		}
		break;
		case 0x1B:
		{
			unsigned int index = (pos[0] >> 13) & 0x3F;
			CMoKeyframe*** kf = (CMoKeyframe***)(data + 4 * index);
			double kff = GetKeyframeFrame(kf, special);
			float f114 = kff * 255.0;
			if (f114 < 0.0) {
				f114 = 0.0;
			}
			a2->field_F8.field_3 = (long long)f114;
		}
		break;
		case 0x1E:
			if (a2->VirtElem1(FFXI::Constants::Enums::ElemType::D3b) == true) {
				CMoD3bElem* d3belem = (CMoD3bElem*)a2;
				unsigned int index = (pos[0] >> 13) & 0x3F;
				float v435{};
				unsigned int flag = *(unsigned int*)(data + 4 * index + 4);
				if ((flag & 0x10) != 0) {
					v435 = XiDateTime::current_time_t;
				}
				else {
					unsigned int v289 = (flag >> 5) & 0x1FF;
					if (v289 != 0) {
						v435 = fmod((double)v289 * special, 1.0);
					}
					else {
						v435 = special;
					}
				}

				CMoKeyframe** kf = *(CMoKeyframe***)(data + 4 * index);
				if (kf != nullptr) {
					float val = *(float*)(data + 4 * index + 4);
					float f114{};
					if ((flag & 0xF) != 0) {
						f114 = (*kf)->GetSplineFrameValue(v435, val);
					}
					else {
						f114 = (*kf)->GetFrameValue(v435, val);
					}
					d3belem->field_194[0] = f114;
				}
				else {
					d3belem->field_194[0] = 0.0;
				}
			}
			break;
		case 0x1F:
			if (a2->VirtElem1(FFXI::Constants::Enums::ElemType::D3b) == true) {
				CMoD3bElem* d3belem = (CMoD3bElem*)a2;
				unsigned int index = (pos[0] >> 13) & 0x3F;
				float v435{};
				unsigned int flag = *(unsigned int*)(data + 4 * index + 4);
				if ((flag & 0x10) != 0) {
					v435 = XiDateTime::current_time_t;
				}
				else {
					unsigned int v289 = (flag >> 5) & 0x1FF;
					if (v289 != 0) {
						v435 = fmod((double)v289 * special, 1.0);
					}
					else {
						v435 = special;
					}
				}

				CMoKeyframe** kf = *(CMoKeyframe***)(data + 4 * index);
				if (kf != nullptr) {
					float val = *(float*)(data + 4 * index + 4);
					float f114{};
					if ((flag & 0xF) != 0) {
						f114 = (*kf)->GetSplineFrameValue(v435, val);
					}
					else {
						f114 = (*kf)->GetFrameValue(v435, val);
					}
					d3belem->field_194[1] = f114;
				}
				else {
					d3belem->field_194[1] = 0.0;
				}
			}
			break;
		case 0x20:
			if (a2->VirtElem1(FFXI::Constants::Enums::ElemType::D3b) == true) {
				CMoD3bElem* d3belem = (CMoD3bElem*)a2;
				unsigned int index = (pos[0] >> 13) & 0x3F;
				float v435{};
				unsigned int flag = *(unsigned int*)(data + 4 * index + 4);
				if ((flag & 0x10) != 0) {
					v435 = XiDateTime::current_time_t;
				}
				else {
					unsigned int v289 = (flag >> 5) & 0x1FF;
					if (v289 != 0) {
						v435 = fmod((double)v289 * special, 1.0);
					}
					else {
						v435 = special;
					}
				}

				CMoKeyframe** kf = *(CMoKeyframe***)(data + 4 * index);
				if (kf != nullptr) {
					float val = *(float*)(data + 4 * index + 4);
					float f114{};
					if ((flag & 0xF) != 0) {
						f114 = (*kf)->GetSplineFrameValue(v435, val);
					}
					else {
						f114 = (*kf)->GetFrameValue(v435, val);
					}
					d3belem->field_194[2] = f114;
				}
				else {
					d3belem->field_194[2] = 0.0;
				}
			}
			break;
		case 0x21:
			if (a2->VirtElem1(FFXI::Constants::Enums::ElemType::D3b) == true) {
				CMoD3bElem* d3belem = (CMoD3bElem*)a2;
				unsigned int index = (pos[0] >> 13) & 0x3F;
				float v435{};
				unsigned int flag = *(unsigned int*)(data + 4 * index + 4);
				if ((flag & 0x10) != 0) {
					v435 = XiDateTime::current_time_t;
				}
				else {
					unsigned int v289 = (flag >> 5) & 0x1FF;
					if (v289 != 0) {
						v435 = fmod((double)v289 * special, 1.0);
					}
					else {
						v435 = special;
					}
				}

				CMoKeyframe** kf = *(CMoKeyframe***)(data + 4 * index);
				if (kf != nullptr) {
					float val = *(float*)(data + 4 * index + 4);
					float f114{};
					if ((flag & 0xF) != 0) {
						f114 = (*kf)->GetSplineFrameValue(v435, val);
					}
					else {
						f114 = (*kf)->GetFrameValue(v435, val);
					}
					d3belem->field_194[3] = f114;
				}
				else {
					d3belem->field_194[3] = 0.0;
				}
			}
			break;
		case 0x22:
			if (a2->VirtElem1(FFXI::Constants::Enums::ElemType::D3b) == true) {
				CMoD3bElem* d3belem = (CMoD3bElem*)a2;
				unsigned int index = (pos[0] >> 13) & 0x3F;
				float v435{};
				unsigned int flag = *(unsigned int*)(data + 4 * index + 4);
				if ((flag & 0x10) != 0) {
					v435 = XiDateTime::current_time_t;
				}
				else {
					unsigned int v289 = (flag >> 5) & 0x1FF;
					if (v289 != 0) {
						v435 = fmod((double)v289 * special, 1.0);
					}
					else {
						v435 = special;
					}
				}

				CMoKeyframe** kf = *(CMoKeyframe***)(data + 4 * index);
				if (kf != nullptr) {
					float val = *(float*)(data + 4 * index + 4);
					float f114{};
					if ((flag & 0xF) != 0) {
						f114 = (*kf)->GetSplineFrameValue(v435, val);
					}
					else {
						f114 = (*kf)->GetFrameValue(v435, val);
					}
					d3belem->field_194[4] = f114;
				}
				else {
					d3belem->field_194[4] = 0.0;
				}
			}
			break;
		case 0x27:
		{
			float scale = CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
			a2->field_120 += scale * *(float*)(pos + 1);
			a2->field_17B |= 1;
		}
			break;
		case 0x28:
		{
			float scale = CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
			a2->field_124 += scale * *(float*)(pos + 1);
			a2->field_17B |= 2;
			break;
		}
		case 0x2E:
		case 0x48:
		{
			float* fpos = (float*)pos;
			double f114 = a2->VirtElem6();
			if (f114 == 0.0) {
				a2->field_138 = 0.0;
			}
			else if ((pos[0] & 0xFF) == 0x48) {
				if (f114 < fpos[1]) {
					a2->field_138 = 0.0;
					if ((pos[5] & 1) != 0) {
						a2->Life = 1.0;
					}
				}
				else if (f114 < fpos[2]) {
				double v63 = (f114 - fpos[2]) / (fpos[1] - fpos[2]);
				a2->field_138 *= 1.0 - v63;
				}
				else if (f114 >= fpos[4]) {
				a2->field_138 = 0.0;
				if ((pos[5] & 1) != 0) {
					a2->Life = 1.0;
				}
				}
				else if (f114 >= fpos[3]) {
				double v63 = (f114 - fpos[3]) / (fpos[4] - fpos[3]);
				a2->field_138 *= 1.0 - v63;
				}
			}
			else {
			if (f114 < 0.0) {
				a2->field_138 = 0.0;
				if ((pos[3] & 1) != 0) {
					a2->Life = 1.0;
				}
			}
			else if (f114 > fpos[2]) {
				a2->field_138 = 0.0;
				if ((pos[3] & 1) != 0) {
					a2->Life = 1.0;
				}
			}
			else if (f114 >= fpos[1]) {
				double v63 = (f114 - fpos[1]) / (fpos[2] - fpos[1]);
				a2->field_138 *= 1.0 - v63;
			}
			}
		}
		break;
		case 0x34:
		{
			unsigned int index = (pos[0] >> 13) & 0x3F;
			char* m = data + 4 * index;
			CMoVtx** vtx = *(CMoVtx***)(m + 28);
			if (vtx != nullptr) {
				CMoKeyframe** pkf = *(CMoKeyframe***)m;
				float prog = special;
				if (pkf != nullptr && *pkf != nullptr) {
					if ((m[4] & 0xF) != 0) {
						prog = (*pkf)->GetSplineFrameValue(special);
					}
					else {
						prog = (*pkf)->GetSplineFrameValue(special);
					}
					if (prog > 1.0) {
						prog = fmod(prog, 1.0);
					}
				}
				D3DXVECTOR3 v428{};
				(*vtx)->list.GetSplinePos(&v428, prog);
				D3DXVECTOR3 v476{};
				v476.x = v428.x - *(float*)(m + 12);
				v476.y = v428.y - *(float*)(m + 16);
				v476.z = v428.z - *(float*)(m + 20);
				memcpy_s(m + 12, 12, &v428, sizeof(v428));
				a2->field_54 += v476;
			}
		}
		break;
		case 0x3C:
		{
			unsigned int index = (pos[0] >> 13) & 0x3F;
			CMoKeyframe** pkf = *(CMoKeyframe***)(data + 4 * index);
			float prog{ 0.0 };
			if (pkf != nullptr) {
				if ((data[4 * index + 4] & 0xF) != 0) {
					prog = (*pkf)->GetSplineFrameValue(XiDateTime::current_time_t);
				}
				else {
					prog = (*pkf)->GetFrameValue(XiDateTime::current_time_t);
				}
			}	
			prog *= 255.0;
			if (prog < 0) {
				prog = 0;
			}
			a2->field_F8.field_2 = (long)prog;
		}
			break;
		case 0x3D:
		{
			unsigned int index = Get11FC(pos);
			CMoKeyframe** pkf = *(CMoKeyframe***)(data + index);
			float prog{ 0.0 };
			if (pkf != nullptr) {
				if ((data[index + 4] & 0xF) != 0) {
					prog = (*pkf)->GetSplineFrameValue(XiDateTime::current_time_t);
				}
				else {
					prog = (*pkf)->GetFrameValue(XiDateTime::current_time_t);
				}
			}
			prog *= 255.0;
			if (prog < 0) {
				prog = 0;
			}
			a2->field_F8.field_1 = (long)prog;
		}
			break;
		case 0x3E:
		{
			unsigned int index = Get11FC(pos);
			CMoKeyframe** pkf = *(CMoKeyframe***)(data + index);
			float prog{ 0.0 };
			if (pkf != nullptr) {
				if ((data[index + 4] & 0xF) != 0) {
					prog = (*pkf)->GetSplineFrameValue(XiDateTime::current_time_t);
				}
				else {
					prog = (*pkf)->GetFrameValue(XiDateTime::current_time_t);
				}
			}
			prog *= 255.0;
			if (prog < 0) {
				prog = 0;
			}
			a2->field_F8.field_0 = (long)prog;
		}
			break;
		case 0x3F:
		{
			unsigned int index = Get11FC(pos);
			unsigned int val = *(unsigned int*)(data + index + 4);
			float v347 = CurTime();
			if ((val & 0x3FE0) != 0) {
				unsigned int v493 = (val >> 5) & 0x1FF;
				v347 *= (float)v493;
				v347 = fmod(v347, 1.0);
			}

			CMoKeyframe** pkeyframe = *(CMoKeyframe***)(data + index);
			float framevalue{ 0.0 };
			if (pkeyframe != nullptr) {
				if ((data[index + 4] & 0xF) == 0) {
					framevalue = (*pkeyframe)->GetFrameValue(v347);
				}
				else {
					framevalue = (*pkeyframe)->GetSplineFrameValue(v347);
				}
			}

			a2->field_138 *= framevalue;
		}
		break;
		case 0x43:
			if (a2->VirtElem1(FFXI::Constants::Enums::ElemType::Sound) == true) {
				CYySoundElem* se = (CYySoundElem*)a2;
				unsigned int index = Get11FC(pos);
				CMoKeyframe*** kf = (CMoKeyframe***)(data + index);
				float f114{ 0.0 };
				if (*kf != nullptr) {
					if ((data[index + 4] & 0xF) != 0) {
						f114 = (**kf)->GetSplineFrameValue(XiDateTime::current_time_t);
					}
					else {
						f114 = (**kf)->GetFrameValue(XiDateTime::current_time_t);
					}
					f114 *= 127.0;
				}
				short val = f114 + 8388608.0;
				val -= 0x4B000000;
				se->field_1BE = val;
			}
			break;
		case 0x45:
			if (a2->res != nullptr) {
				XiDateTime v475{};
				XiDateTime::GetCurrentDate(&v475);
				//XiDateTime::AddHour inlined here
				v475.SetTickFrame(v475.Time + 216000 * 12);
				a2->field_170 = GetMoonShapeNo();
				a2->field_E0.z = 1.570796;//not quite pi/2
			}
			a2->field_187 = 1;
			break;
		case 0x49:
		{
			unsigned int index = Get11FC(pos);
			float f114{ 0.0 };
			CMoKeyframe** pkf = (CMoKeyframe**)(data + index);
			if (pkf != nullptr) {
				if ((data[index + 4] & 0xF) != 0) {
					f114 = (*pkf)->GetSplineFrameValue(XiDateTime::current_time_t);
				}
				else {
					f114 = (*pkf)->GetFrameValue(XiDateTime::current_time_t);
				}
			}
			CMoPointLightProgElem* pl = (CMoPointLightProgElem*)a2;
			pl->field_1A0 = f114;
		}
			break;
		case 0x4E:
		{
			XiDateTime v467{};
			v467.Time = XiDateTime::current_time.Time;
			int v423 = GetDayOfWeek();
			XiDateTime v419(3600);
			unsigned int today = pos[v423 + 2];
			if (v467.Time < v419.Time) {
				int v398 = v423 - 1;
				if (v398 < 0) {
					v398 = 7;
				}
				unsigned int yesterday = pos[v398 + 2];
				float timeScale = (float)v467.Time / 3600.0;

				int todayByte = (today >> 0) & 0xFF;
				int yesterdayByte = (yesterday >> 24) & 0xFF;
				int diff = todayByte - yesterdayByte;
				a2->field_F8.field_3 = (float)diff * timeScale + yesterdayByte;

				todayByte = (today >> 8) & 0xFF;
				yesterdayByte = (yesterday >> 0) & 0xFF;
				diff = todayByte - yesterdayByte;
				a2->field_F8.field_2 = (float)diff * timeScale + yesterdayByte;


				todayByte = (today >> 16) & 0xFF;
				yesterdayByte = (yesterday >> 8) & 0xFF;
				diff = todayByte - yesterdayByte;
				a2->field_F8.field_1 = (float)diff * timeScale + yesterdayByte;


				todayByte = (today >> 24) & 0xFF;
				yesterdayByte = (yesterday >> 16) & 0xFF;
				diff = todayByte - yesterdayByte;
				a2->field_F8.field_0 = (float)diff * timeScale + yesterdayByte;
			}
			else {
				a2->field_F8.MixSet(today);
			}
		}
			break;
		case 0x4F:
		{
			XiDateTime v465{};
			v465.Time = XiDateTime::current_time.Time;
			int v423 = GetMoonShapeNo();
			XiDateTime v419(3600);
			unsigned int today = pos[v423 + 2];
			float scales[4]{};
			if (v465.Time < v419.Time) {
				int v388 = v423 - 1;
				if (v388 < 0) {
					v388 = 11;
				}
				unsigned int yesterday = pos[v388 + 2];
				float timeScale = (float)v465.Time / 3600.0;

				int todayByte = (today >> 0) & 0xFF;
				int yesterdayByte = (yesterday >> 0) & 0xFF;
				int diff = todayByte - yesterdayByte;
				scales[0] = (float)diff * timeScale + yesterdayByte;

				todayByte = (today >> 8) & 0xFF;
				yesterdayByte = (yesterday >> 8) & 0xFF;
				diff = todayByte - yesterdayByte;
				scales[1] = (float)diff * timeScale + yesterdayByte;

				todayByte = (today >> 16) & 0xFF;
				yesterdayByte = (yesterday >> 16) & 0xFF;
				diff = todayByte - yesterdayByte;
				scales[2] = (float)diff * timeScale + yesterdayByte;

				todayByte = (today >> 24) & 0xFF;
				yesterdayByte = (yesterday >> 24) & 0xFF;
				diff = todayByte - yesterdayByte;
				scales[3] = (float)diff * timeScale + yesterdayByte;
			}
			else {
				scales[0] = (float)((today >> 0) & 0xFF);
				scales[1] = (float)((today >> 8) & 0xFF);
				scales[2] = (float)((today >> 16) & 0xFF);
				scales[3] = (float)((today >> 24) & 0xFF);
			}

			scales[0] /= 128.0;
			scales[1] /= 128.0;
			scales[2] /= 128.0;
			scales[3] /= 128.0;
			a2->field_F8.Scale(scales);
		}
			break;
		case 0x50:
		{
			CYyGenerator* gen = *a2->field_FC;
			CYyObject* val = (CYyObject*)gen->CMoAttachments::field_34;
			if (val != nullptr) {
				if (val->IsKindOf(&YmCombineWeather::YmCombineWeatherClass) == true) {
					YmCombineWeather* cw = (YmCombineWeather*)val;
					a2->field_138 *= cw->field_1C;
				}
			}
		}
			break;
		case 0x64: 
		{
			unsigned int index = (pos[0] >> 13) & 0x3F;
			D3DXVECTOR3 v457 = a2->field_54;
			float f1 = *(float*)(data + 4 * index + 0);
			float f2 = *(float*)(data + 4 * index + 4);
			float f3 = *(float*)(data + 4 * index + 8);
			v457.x -= f1;
			v457.y -= f2;
			v457.z -= f3;
			float v72 = -atan2(*(float*)(data + 4 * index + 20), *(float*)(data + 4 * index + 12));
			float v73 = sin(v72);
			float v74 = cos(v72);
			double f114 = atan2(v457.x * v73 + v457.z * v74, v457.x * v74 - v457.z * v73);
			f1 = *((float*)pos + 1);
			f2 = *((float*)pos + 2);
			f3 = *((float*)pos + 3);
			if (f1 != 0.0) {
				a2->field_E0.x = f114 * f1;
				if (a2->field_E0.x > FFXI::Constants::Values::ANGLE_PI) {
					a2->field_E0.x -= FFXI::Constants::Values::ANGLE_2PI;
				}
				if (a2->field_E0.x < FFXI::Constants::Values::ANGLE_MINUS_PI) {
					a2->field_E0.x += FFXI::Constants::Values::ANGLE_2PI;
				}
			}
			if (f2 != 0.0) {
				a2->field_E0.y = f114 * f2;
				if (a2->field_E0.y > FFXI::Constants::Values::ANGLE_PI) {
					a2->field_E0.y -= FFXI::Constants::Values::ANGLE_2PI;
				}
				if (a2->field_E0.y < FFXI::Constants::Values::ANGLE_MINUS_PI) {
					a2->field_E0.y += FFXI::Constants::Values::ANGLE_2PI;
				}
			}
			if (f3 != 0.0) {
				a2->field_E0.z = f114 * f3;
				if (a2->field_E0.z > FFXI::Constants::Values::ANGLE_PI) {
					a2->field_E0.z -= FFXI::Constants::Values::ANGLE_2PI;
				}
				if (a2->field_E0.z < FFXI::Constants::Values::ANGLE_MINUS_PI) {
					a2->field_E0.z += FFXI::Constants::Values::ANGLE_2PI;
				}
			}
			memcpy_s(data + 4 * index, 12, &a2->field_54, sizeof(a2->field_54));
			memcpy_s(data + 4 * index + 12, 12, &v457, sizeof(v457));
			a2->field_187 = 1;
		}
			break;
		default:
			std::cout << "Unhandled Elem Idle case 0x" << std::hex << (pos[0] & 0xFF) << std::endl;
			break;
		}
		if ((pos[0] & 0xFF) == 0) {
			break;
		}
		pos += (pos[0] >> 8) & 0x1F;
	}

	CYyDb::g_pCYyDb->pMoProcessor->PushToStack(pop);
}

void FFXI::CYy::CYyGenerator::ElemDie(CMoElem* a2)
{
	unsigned int* pos = this->Script4;
	char* v4 = (char*)a2 + a2->field_176;
	while (pos != nullptr) {
		unsigned char v5 = pos[0] & 0xFF;
		switch (v5) {
		case 0:
			//Script End
			break;
		case 4:
		{
			unsigned int index = (pos[0] >> 13) & 0x3F;
			CMoVtx** vtx = *(CMoVtx***)(v4 + 4 * index + 28);
			if (vtx != nullptr) {
				(*vtx)->list.RemoveRef();
			}
		}
			break;
		case 5:
			a2->Life = a2->field_114;
			break;
		default:
			std::cout << "Unhandled ElemDie case 0x" << std::hex << (int)v5 << std::endl;
			break;
		}
		if ((pos[0] & 0xFF) == 0) {
			break;
		}
		pos += (pos[0] >> 8) & 0x1F;
	}
}

bool FFXI::CYy::CYyGenerator::IsNever()
{
	return ((this->flags & 0x400) != 0 || this->GetLife() == 0);
}

int FFXI::CYy::CYyGenerator::GetLife()
{
	return *((unsigned short*)this->Script2 + 17);
}

void FFXI::CYy::CYyGenerator::Unlink(bool a2)
{
	this->flags &= 0xFFFFF7FF;
	if (this->field_A8 != nullptr) {
		if (a2 == true) {
			delete this->field_A8;
			this->field_A8 = nullptr;
		}
		else {
			this->field_DC |= 1;
			this->field_A8->field_14 = 1;
		}
	}
}

void FFXI::CYy::CYyGenerator::Preset()
{
	this->GetModel();
	this->PosScale = { 1.0, 1.0, 1.0 };
	this->ModelScale = { 1.0, 1.0, 1.0 };

	unsigned int* data = this->Script1;
	while (data != nullptr) {
		int code = data[0] & 0xFF;
		switch (code) {
		case 0x05:		case 0x06:		case 0x07:		case 0x08:
		case 0x09:		case 0x0B:		case 0x0C:		case 0x0D:
		case 0x0E:		case 0x0F:		case 0x10:		case 0x12:
		case 0x13:		case 0x14:
		case 0x17:
			if (data[1] == 0) {
				this->FindResourceUnder((CMoResource***)(data + 1), FFXI::Constants::Enums::ResourceType::Keyframe, data[2]);
			}
			break;
		default:
			break;
		}
		if (code == 0) {
			break;
		}
		int skip = (data[0] >> 8) & 0x1F;
		data += skip;
	}

	data = this->Script2;
	while (data != nullptr) {
		int code = data[0] & 0xFF;
		switch (code) {
		case 0x21:		case 0x22:		case 0x23:		case 0x24:
		case 0x25:		case 0x26:		case 0x27:		case 0x28:
		case 0x29:		case 0x2A:		case 0x2B:		case 0x2C:
		case 0x2D:		case 0x2E:		case 0x2F:		case 0x33:
		case 0x34:		case 0x35:		case 0x36:		case 0x37:
		case 0x38:		case 0x39:		case 0x50:		case 0x51:
		case 0x52:		case 0x59:		case 0x5A:		case 0x5B:
		case 0x5C:		case 0x5D:		case 0x5E:		case 0x5F:
		case 0x60:		case 0x61:		case 0x62:		case 0x63:
		case 0x64:		case 0x65:		case 0x66:		case 0x68:
		case 0x69:		case 0x6C:		case 0x6D:		case 0x6E:
		case 0x6F:		case 0x70:		case 0x74:		case 0x75:
		case 0x76:		case 0x77:		case 0x78:		case 0x7C:
		case 0x7D:		case 0x80:		case 0x81:		case 0x82:
		case 0x83:		case 0x84:		case 0x85:		case 0x8B:
		case 0x8C:		case 0x8D:		case 0x93:		case 0x95:
		case 0x96:
		case 0x97:
			if (data[1] == 0) {
				this->FindResourceUnder((CMoResource***)(data + 1), FFXI::Constants::Enums::ResourceType::Keyframe, data[2]);
			}
			break;
		case 0x3C:
		case 0x44:
		case 0x53:
		case 0x6A:
			if (data[1] == 0) {
				this->FindResourceUnder((CMoResource***)(data + 1), FFXI::Constants::Enums::ResourceType::Generater, data[2]);
			}
			break;
		case 0x4E:
		case 0x4F:
			if (data[1] == 0) {
				this->FindResourceUnder((CMoResource***)(data + 1), FFXI::Constants::Enums::ResourceType::Vtx, data[2]);
			}
			this->field_A0 = data[1];
			break;
		case 0x54:
			if (data[4] == 0) {
				this->FindResourceUnder((CMoResource***)(data + 4), FFXI::Constants::Enums::ResourceType::Vtx, data[5]);
			}
			if (data[1] == 0 && data[2] != 0) {
				this->FindResourceUnder((CMoResource***)(data + 1), FFXI::Constants::Enums::ResourceType::Keyframe, data[2]);
			}
			this->field_A0 = data[1];
			break;
		case 0x55:
			if (data[5] == 0 && data[4] != 0) {
				this->FindResourceUnder((CMoResource***)(data + 5), FFXI::Constants::Enums::ResourceType::Texinfo, data[4]);
			}
			break;
		case 0x6B:
			if (data[2] == 0) {
				this->FindResourceUnder((CMoResource***)(data + 2), FFXI::Constants::Enums::ResourceType::Sph, data[1]);
			}
		default:
			break;
		}
		if (code == 0) {
			break;
		}

		int skip = (data[0] >> 8) & 0x1F;
		data += skip;
	}

	data = this->Script3;
	while (data != nullptr) {
		int code = data[0] & 0xFF;
		switch (code) {
		case 0x47:
			if (data[3] == 0) {
				this->FindResourceUnder((CMoResource***)(data + 3), FFXI::Constants::Enums::ResourceType::Texinfo, data[2]);
			}
			break;
		case 0x65:
			if (data[1] == 0) {
				this->FindResourceUnder((CMoResource***)(data + 1), FFXI::Constants::Enums::ResourceType::Scheduler, data[2]);
			}
			if (data[2] == 0 && data[1] != 0) {
				this->FindResourceUnder((CMoResource***)(data + 2), FFXI::Constants::Enums::ResourceType::Generater, data[1]);
			}
			break;
		default:
			break;
		}

		if (code == 0) {
			break;
		}

		int skip = (data[0] >> 8) & 0x1F;
		data += skip;
	}

	data = this->Script4;
	while (data != nullptr) {
		int code = data[0] & 0xFF;
		switch (code) {
		case 1:
			if (data[1] == 0) {
				this->FindResourceUnder((CMoResource***)(data + 1), FFXI::Constants::Enums::ResourceType::Generater, data[2]);
			}
			break;
		case 2:
			if (data[1] == 0) {
				this->FindResourceUnder((CMoResource***)(data + 1), FFXI::Constants::Enums::ResourceType::Scheduler, data[2]);
			}
			break;
		default:
			break;
		}
		if (code == 0) {
			break;
		}

		int skip = (data[0] >> 8) & 0x1F;
		data += skip;
	}
}

bool FFXI::CYy::CYyGenerator::AttachCalc()
{
	this->field_90 = 0;
	unsigned int thing = (this->CMoAttachments::field_4 & 0xF) + 16 * ((this->CMoAttachments::field_4 >> 16) & 1);
	if (thing == 0)
		return false;

	if (this->field_98 == nullptr) {
		this->field_90 = this->CMoAttachments::MakeAttachMatrix(this->VRes4(), 0, 0);
	}
	else {
		exit(0x10052EE8);
	}

	CXiActor* actor{ nullptr };
	switch ((this->CMoAttachments::field_4 >> 20) & 0xF) {
	case 1u:
	case 2u:
	case 3u:
	case 4u:
		actor = this->CMoAttachments::GetMasterCaster();
		break;
	case 5u:
	case 6u:
	case 7u:
	case 8u:
		actor = this->CMoAttachments::GetMasterTarget();
		break;
	default:
		break;
	}

	if (actor == nullptr) {
		this->PosScale = { 1.0, 1.0, 1.0 };
	}
	else {
		float v26 = this->field_70;
		if (v26 == 0.0) {
			v26 = 1.0;
		}

		float widthScale = actor->GetWidthScale();
		float heightScale = actor->GetHeightScale();

		unsigned int someflag = this->CMoAttachments::field_4 >> 20;
		if ((someflag & 3) == 0) {
			float v16{};
			if (heightScale > widthScale) {
				v16 = heightScale - 1.0;
			}
			else {
				v16 = widthScale - 1.0;
			}

			this->PosScale.x = v16 * v26 + 1.0;
			this->PosScale.y = this->PosScale.x;
			this->PosScale.z = this->PosScale.x;
		}
		else {
			if ((someflag & 2) == 0) {
				this->PosScale.y = 1.0;
			}
			else {
				this->PosScale.y = heightScale;
			}

			if ((someflag & 1) == 0) {
				this->PosScale.x = 1.0;
				this->PosScale.z = 1.0;
			}
			else {
				this->PosScale.x = widthScale;
				this->PosScale.z = widthScale;
			}
		}
	}

	switch ((this->CMoAttachments::field_4 >> 24) & 0xF) {
	case 1u:
	case 2u:
	case 3u:
	case 4u:
		actor = this->CMoAttachments::GetMasterCaster();
		break;
	case 5u:
	case 6u:
	case 7u:
	case 8u:
		actor = this->CMoAttachments::GetMasterTarget();
		break;
	default:
		break;
	}

	if (actor == nullptr) {
		this->ModelScale = { 1.0, 1.0, 1.0 };
	}
	else {
		float v27{};
		if (this->field_74 == 0.0) {
			v27 = 1.0;
		}
		else {
			v27 = this->field_74;
		}

		float widthScale = actor->GetWidthScale();
		float heightScale = actor->GetHeightScale();

		unsigned int v19 = this->CMoAttachments::field_4 >> 24;
		if ((v19 & 3) == 0) {
			float v24{};
			if (heightScale > widthScale) {
				v24 = heightScale - 1.0;
			}
			else {
				v24 = widthScale - 1.0;
			}

			this->ModelScale.x = v24 * v27 + 1.0;
			this->ModelScale.y = this->ModelScale.x;
			this->ModelScale.z = this->ModelScale.x;
		}
		else {
			if ((v19 & 2) == 0) {
				this->ModelScale.y = 1.0;
			}
			else {
				this->ModelScale.y = heightScale;
			}

			if ((v19 & 1) == 0) {
				this->ModelScale.x = 1.0;
				this->ModelScale.z = 1.0;
			}
			else {
				this->ModelScale.x = widthScale;
				this->ModelScale.z = widthScale;
			}
		}
	}

	if (this->field_90 != 0.0) {
		this->PosScale.x = this->field_90;
	}

	return true;
}

void FFXI::CYy::CYyGenerator::Activate()
{
	if ((this->field_DC & 1) != 0
		|| (this->flags & 0x800) != 0) {
		if (this->IsNever()) {
			this->KillAll(nullptr);
			int v2 = (this->CMoAttachments::field_4 & 0xF) + 16 * ((this->CMoAttachments::field_4 >> 16) & 1);
			if (v2 != 20) {
				if ((v2 >= 1 && v2 < 14)
					|| v2 > 15) {
					this->DeleteClone(this->CMoAttachments::GetMasterCaster(), this->CMoAttachments::GetMasterTarget(), 0, nullptr);
				}
			}
		}
		this->field_9C = 0;
		this->field_CC = 0;
		this->field_C8 = 0;
	}

	this->flags |= 0x800;

	if ((this->field_DC & 1) != 0) {
		if (this->field_A8 != nullptr) {
			this->field_A8->field_14 = 0;
		}

		this->field_DC &= ~1u;
	}
	else {
		if (this->field_A8 != nullptr) {
			delete this->field_A8;
			this->field_A8 = nullptr;
		}

		char* mem = StorageLists::instance->Get(sizeof(GeneratorTask), FFXI::Constants::Enums::MEM_MODE::Ex);
		CMoTaskMng::DeleteThisTask = true;
		GeneratorTask* gent{ nullptr };
		if (mem != nullptr) {
			gent = new (mem) GeneratorTask();
			gent->field_3C = 0;
		}

		this->field_A8 = gent;
		gent->field_34 = (CYyGenerator**)this->PreHeader.PointerToThisInRPL;
		gent->field_38 = (CYyGenerator**)this->PreHeader.PointerToThisInRPL;
		gent->Param1 = (int)this->PreHeader.PointerToThisInRPL;
		gent->Param2 = 'Mg\0\0';
		this->AttachCalc();

		int v10 = (this->CMoAttachments::field_4 & 0xF) + 16 * ((this->CMoAttachments::field_4 >> 16) & 1);
		if (v10 == 20) {
			CYyDb::g_pCYyDb->pCMoTaskMng->DoSomething(gent);
		}
		else if ((v10 < 1 || v10 >= 14) && v10 <= 15) {
			CYyDb::g_pCYyDb->pCMoTaskMng->DoSomething(gent);
		}
		else {
			CYyDb::g_pCYyDb->pCMoTaskMng->SomeShift4(gent, 9);
		}
	}
}

void FFXI::CYy::CYyGenerator::MaybeDeactivate()
{
	CMoResource::DoTheThing(this);
	this->KillAll(nullptr);
	this->PreHeader.field_14 |= 0x8000;
	this->SomeClean3();
	this->PreHeader.field_14 &= ~0x8000;
	this->Unlink(false);
}

void FFXI::CYy::CYyGenerator::Tracking(FFXI::Math::WMatrix* a2)
{
	if (a2 == nullptr) {
		return;
	}

	unsigned int* v3 = this->IdleFindFirst(0x11);
	this->Tracking(v3, a2);
}

void FFXI::CYy::CYyGenerator::Tracking(unsigned int* a2, FFXI::Math::WMatrix* a3)
{
	FFXI::Math::WMatrix* pop{ nullptr };
	CYyDb::g_pCYyDb->pMoProcessor->PopFromStack(&pop);
	long long v19 = (unsigned char)(a2[1] >> 2);
	double v7 = (double)v19 / 255.0;

	if (this->field_98 == nullptr) {
		this->field_90 = this->::CMoAttachments::MakeAttachMatrix(pop, 0, 0);
	}
	else {
		D3DXVECTOR4 v19{}, v20{};
		memcpy_s(&v20, 12, this->field_98 + 4, 12);
		v20.w = 1.0;

		memcpy_s(&v19, 12, this->field_98 + 16, 12);
		v19.w = 1.0;
		this->field_90 = this->::CMoAttachments::MakeAttachMatrix(pop, &v20, &v19);
	}

	if (this->field_90 != 0.0) {
		this->PosScale.x = this->field_90;
	}

	if ((a2[1] & 1) != 0) {
		a3->_41 += (pop->_41 - a3->_41) * v7;
		a3->_42 += (pop->_42 - a3->_42) * v7;
		a3->_43 += (pop->_43 - a3->_43) * v7;
		a3->_44 += (pop->_44 - a3->_44) * v7;
	}

	if ((a2[1] & 2) != 0) {
		a3->_11 += (pop->_11 - a3->_11) * v7;
		a3->_12 += (pop->_12 - a3->_12) * v7;
		a3->_13 += (pop->_13 - a3->_13) * v7;
		a3->_14 += (pop->_14 - a3->_14) * v7;

		a3->_21 += (pop->_21 - a3->_21) * v7;
		a3->_22 += (pop->_22 - a3->_22) * v7;
		a3->_23 += (pop->_23 - a3->_23) * v7;
		a3->_24 += (pop->_24 - a3->_24) * v7;
		
		a3->_31 += (pop->_31 - a3->_31) * v7;
		a3->_32 += (pop->_32 - a3->_32) * v7;
		a3->_33 += (pop->_33 - a3->_33) * v7;
		a3->_34 += (pop->_34 - a3->_34) * v7;
	}
	CYyDb::g_pCYyDb->pMoProcessor->PushToStack(pop);
}

bool FFXI::CYy::CYyGenerator::Idle(CMoElem* a2)
{
	char v171{ 0 };
	if ((this->field_DC & 1) != 0) {
		return false;
	}

	float progress = 0.0;
	if (this->field_D0 != 0.0) {
		progress = 1.0 - (this->field_D0 - this->field_C8) / this->field_D0;
		if (progress > 1.0) {
			progress = 1.0;
		}
	}

	CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;
	BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
	FFXI::Math::WMatrix* pop{ nullptr };
	proc->PopFromStack(&pop);
	
	unsigned int* pos = this->Script1;
	while (pos && (pos[0] & 0xFFu) != 0) {
		switch (pos[0] & 0xFFu) {
		case 0x0A:
			//Check distance to camera
		{
			D3DXVECTOR3 v191{}, v192{};
			this->GetPos(&v191);
			if (this->field_A4 != nullptr) {
				proc->VirtProcessor15(&v192, this->field_A4, &v191);
			}
			else {
				v192 = v191;
			}

			v191 = cammng->Position - v192;
			char v172{ 1 };
			float distance = sqrt(proc->Vec3Dot(&v191, &v191));
			float maxdistance = ((float*)pos)[1];
			if (maxdistance == 0.0) {
				float zonedraw = XiZone::zone->GetDrawDistance(false);
				if (distance < zonedraw) {
					v172 = 0;
				}
			}
			else if (distance <= maxdistance) {
				v172 = 0;
			}

			float mindraw = ((float*)pos)[2];
			v171 = (distance < mindraw) | v172;

			if (v171 == 0) {
				break;
			}

			if ((pos[3] & 1) != 0) {
				this->Unlink(false);
			}
			pos = nullptr;
			continue;
		}
		case 0x11:
			this->flags |= 0x200u;
			if (this->field_A4 != nullptr) {
				this->Tracking(pos, this->field_A4);
			}
			break;
		default:
			std::cout << "Unhandled GEN IDLE: 0x" << std::hex << (*pos & 0xFFu) << std::endl;
			break;
		}

		//LABEL 188
		pos += (pos[0] >> 8) & 0x1F;
	}
	bool retval{};
	if (this->field_D0 == 0.0 || this->field_D0 > this->field_C8) {
		if (this->IsNever() == true) {
			if (this->field_9C == 0) {
				this->ElemGenerate(a2);
			}
		}
		else {
			double v121 = CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
			if (v121 > 10.0) {
				v121 = 10.0;
			}

			float v185 = CYyGenerator::GetSomeGeneratorScalar();
			if ((this->field_DE & 2) == 0) {
				this->field_CC -= v121;
			}
			else {
				this->field_CC -= CYyDb::g_pCYyDb->g_pCMoDx->field_EB0 * v185;
			}

			float v189 = this->field_D0;
			while (this->field_CC < 0.0) {
				if (v171 == 0) {
					//the flags or statement here simplifies to true, so not included here
					if ((this->flags & 0x20000000) == 0x20000000) {
						this->ElemGenerate(a2);
					}
					else {
						int counter = 0;
						while (true) {
							double v161 = (double)(this->flags & 0x1FF);

							if ((this->field_DE & 1) != 0) {
								v161 *= v185 * 0.30000001;
							}

							if (counter > (int)(long long)v161)
								break;

							this->ElemGenerate(a2);
							counter += 1;
						}
					}
					
				}

				double v158 = (double)(int)(CMoResourceMng::rng.uirand(this->field_D4) + this->field_D6 + 1);
				this->field_CC += v158;
				v189 -= v158;

				if (this->field_D0 != 0.0 && v189 <= this->field_C8) {
					break;
				}
			}
		}

		retval = false;

		this->field_C8 += CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
		if (this->field_D0 != 0.0) {
			if (this->field_C8 >= this->field_D0) {
				retval = true;
			}
		}
	}
	else {
		this->field_C8 += CYyDb::g_pCYyDb->g_pCMoDx->field_EB0;
		retval = true;
	}

	proc->PushToStack(pop);
	return retval;
}

unsigned int* FFXI::CYy::CYyGenerator::IdleFindFirst(unsigned int a2)
{
	unsigned int* pos = this->Script1;
	while (pos != nullptr) {
		if ((pos[0] & 0xFF) == a2) {
			return pos;
		}

		if ((pos[0] & 0xFF) == 0) {
			return nullptr;
		}

		pos += (pos[0] >> 8) & 0x1F;
	}

	return nullptr;
}

void CYyGenerator::KillAll(CYyGenerator** a2) {
	if ((this->PreHeader.field_16 & 2) != 0)
		return;

	CMoResource::DoTheThing(this);
	
	while (this->field_AC != 0) {
		exit(0x10052960);
	}

	if ((this->PreHeader.field_16 & 4) != 0) {
		CYyGeneratorClone* clone = (CYyGeneratorClone*)this;
		if (clone->field_F8 != 0) {
			exit(0x10052981);
			clone->field_F8 = 0;
			clone->SomeClean3();
		}
	}

	CYyGenerator* v5{ nullptr };
	if (a2 != nullptr) {
		v5 = *a2;
	}

	while (this->field_B0 != nullptr) {
		if (v5 != nullptr) {
			unsigned short backup = v5->field_D6;
			v5->field_D6 = (long long)(CYyDb::g_pCYyDb->g_pCMoDx->field_EB0 + 1.0);
			v5->field_CC = 0.0;
			v5->GenUnk1();
			v5->Idle(this->field_B0);
			v5->Unlink(false);
			v5->field_D6 = backup;
		}
		if (this->field_B0 != nullptr) {
			delete this->field_B0;
		}
	}

	this->SomeClean3();
}

void FFXI::CYy::CYyGenerator::DeleteClone(CXiActor* a2, CXiActor* a3, int a4, CYyGenerator** a5)
{
	CMoResource* head = this;
	if (this->CYyObject::IsKindOf(&CYyGeneratorClone::CYyGeneratorCloneClass) == true) {
		if (this->Header.RPLHead != nullptr) {
			head = *this->Header.RPLHead;
		}
		else {
			head = nullptr;
		}
	}

	if (a2 != nullptr) {
		exit(0x10052A99);
	}

	ResourceList* v34{};
	v34->PrepareFromLastCreated(CYyDb::g_pCYyDb->pCMoResourceMng, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
	while (true) {
		CMoResource* next = v34->GetNext(false);
		if (next == nullptr) {
			break;
		}

		if ((next->PreHeader.field_16 & 4) != 0) {
			CYyGeneratorClone* clone = (CYyGeneratorClone*)next;
			CMoResource* res{ nullptr };
			if (clone->field_F4 != nullptr) {
				res = *clone->field_F4;
			}

			if (res == head) {
				if (a4 == false) {
					CMoResource::DoTheThing(clone);
					clone->KillAll(a5);
					clone->SomeClean3();
					CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(clone->PreHeader.PointerToThisInRPL);
					v34->PrepareFromLastCreated(CYyDb::g_pCYyDb->pCMoResourceMng, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
				}
				else {
					clone->Unlink(false);
					float* fptr = (float*)(clone->field_C0);
					if (fptr[4] != 0.0) {
						if (fptr[4] > fptr[2]) {
							fptr[2] = fptr[4];
							if ((clone->PreHeader.field_14 & 0x7FFF) == 0) {
								CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(clone->PreHeader.PointerToThisInRPL);
								v34->PrepareFromLastCreated(CYyDb::g_pCYyDb->pCMoResourceMng, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
							}
						}
					}
				}
			}
		}
	}
}
