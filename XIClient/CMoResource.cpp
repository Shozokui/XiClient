#include "CMoResource.h"
#include "CMoResourceMng.h"
#include "CYyDb.h"
#include "WMatrix.h"
#include "Globals.h"
#include "Enums.h"
#include "Values.h"
#include "XiDateTime.h"
#include <cstring>
#include <iostream>
#include "CYyMeb.h"
#include "CYyResourceFile.h"
#include "CMoKeyframe.h"
#include "CYyBmp2.h"
#include "CYyMsb.h"
#include "CMoD3a.h"
#include "CMoD3b.h"
#include "CMoD3m.h"
#include "CMoVtx.h"
#include "CMoSphRes.h"
#include "CMoRid.h"
#include "CMoWeather.h"
#include "CYySepRes.h"
#include <iostream>
#include "CYyScheduler.h"
#include "CYyGenerator.h"
#include "CMoCamera.h"
#include "CMoMmb.h"
#include "CMoMzb.h"
#include "CMoCib.h"
#include "CMoSk2.h"
#include "CMoMo2.h"
#include "CMoOs2.h"
#include "ResourceList.h"
using namespace FFXI::CYy;
using namespace FFXI::Constants;

const CYyClass CMoResource::CMoResourceClass{
	"CMoResource", sizeof(CMoResource), &CYyObject::CYyObjectClass
};
unsigned int CMoResource::SomeCounter{ 0 };
const CYyClass* FFXI::CYy::CMoResource::GetRuntimeClass()
{
	return &CMoResourceClass;
}

FFXI::CYy::CMoResource::~CMoResource()
{
	this->CMoResource::Close();
	CYyDb::pCMoResourceMng->RemoveRes(this->PreHeader.PointerToThisInRPL);
}

void FFXI::CYy::CMoResource::VObj2(int* param)
{
	if (param[0] == 6) {
		CMoResource::UnlinkFromManager(&this->PreHeader.PointerToThisInRPL);
	}
}

void FFXI::CYy::CMoResource::VObj5(void* param)
{
	CMoResource* ptr = reinterpret_cast<CMoResource*>(param);
	*(this->PreHeader.PointerToThisInRPL) = ptr;
}

void FFXI::CYy::CMoResource::ConstructFromData(char* p_buf)
{
	ResourceHeader header{};
	header.Parse(p_buf);
	unsigned int size = 16 * ((header.Type_Size >> 7) & 0x7FFFF);
	memcpy(&this->Header, p_buf, size);
}

void FFXI::CYy::CMoResource::Open()
{
	//nullsub
}

void FFXI::CYy::CMoResource::Close()
{
	short result = this->PreHeader.field_16;
	if ((result & 2) != 0) return;
	result |= 2;
	this->PreHeader.field_16 = result;
	this->SomeClean2();
	if ((this->PreHeader.field_16 & 1) == 0) return;
	this->PreHeader.field_16 &= 0xFFFE;
	CYyDb::pCMoResourceMng->RemoveRes(this->PreHeader.PointerToThisInRPL);
	delete this;
}

FFXI::Math::WMatrix* FFXI::CYy::CMoResource::VRes4()
{
	return &Globals::emulate_matrix;
}

int FFXI::CYy::CMoResource::GetSomething()
{
	return this->DoSomething3()->PreHeader.field_18;
}

CMoResource* FFXI::CYy::CMoResource::OpenResource(char* p_buf, CMoResource* a2)
{
	ResourceHeader header{};
	header.Parse(p_buf);
	int size = 16 * (((unsigned int)header.Type_Size >> 7) & 0x7FFFF) - 16;
	bool v6{ false }, v8{ false };
	char v7{ 0 };
	char* mem{ nullptr };
	CMoResource* res{ nullptr };
	while (true) {
		int maxsize = StorageLists::instance->FindLargestOpenSlot(Constants::Enums::MEM_MODE::Lower);
		int mrc = CYyDb::pCMoResourceMng->MaybeResourceCount;

		if (maxsize < (2 * size) || mrc > 0xCCCC) {
			int v9 = 4;
			do {
				CMoResource* v0{ nullptr };
				if (CMoResourceMng::LastResourceCreatedByType[Enums::ResourceType::Rmp])
					v0 = *CMoResourceMng::LastResourceCreatedByType[Enums::ResourceType::Rmp];
				if (v0) {
					while (true) {
						CMoResource* res2 = v0->DoSomething3();
						if ((*res2->PreHeader.PointerToThisInRPL)->PreHeader.field_14 == 0) {
							CMoResource::UnlinkFromManager(&res2->PreHeader.PointerToThisInRPL);
							break;
						}
						if (v0->PreHeader.LastResourceCreatedMine)
							v0 = (*v0->PreHeader.LastResourceCreatedMine);
						else
							v0 = nullptr;
						if (!v0) break;
					}
				}
				--v9;
			} while (v9);
		}

		switch (header.Type_Size & 0x7F) {
		case Constants::Enums::ResourceType::Rmp:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CYyResourceFile), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CYyResourceFile();
			break;
		case Constants::Enums::ResourceType::Generater:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CYyGenerator) + 56, Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CYyGenerator();
			break;
		case Constants::Enums::ResourceType::Camera:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoCamera) + 8, Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoCamera();
			break;
		case Constants::Enums::ResourceType::Scheduler:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CYyScheduler), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CYyScheduler();
			break;
		case Constants::Enums::ResourceType::Keyframe:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoKeyframe), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoKeyframe();
			break;
		case Constants::Enums::ResourceType::Bmp2:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CYyBmp2), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CYyBmp2();
			break;
		case Constants::Enums::ResourceType::Mzb:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoMzb), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoMzb();
			break;
		case Constants::Enums::ResourceType::D3m:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoD3m), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoD3m();
			break;
		case Constants::Enums::ResourceType::D3s:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CYyBmp2), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CYyBmp2();
			res->Header.Type_Size = (res->Header.Type_Size & 0xFFFFFF80) ^ 0x1B;
			break;
		case Constants::Enums::ResourceType::D3a:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoD3a), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoD3a();
			break;
		case Constants::Enums::ResourceType::D3b:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoD3b), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoD3b();
			break;
		case Constants::Enums::ResourceType::Sk2:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoSk2), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoSk2();
			break;
		case Constants::Enums::ResourceType::Os2:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoOs2), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoOs2();
			break;
			break;
		case Constants::Enums::ResourceType::Mo2:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoMo2), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoMo2();
			break;
		case Constants::Enums::ResourceType::Mmb:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoMmb), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoMmb();
			break;
		case Constants::Enums::ResourceType::Weather:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoWeather), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoWeather();
			break;
		case Constants::Enums::ResourceType::Meb:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CYyMeb) + 1, Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CYyMeb();
			break;
		case Constants::Enums::ResourceType::Msb:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CYyMsb) + 1, Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CYyMsb();
			break;
		case Constants::Enums::ResourceType::Rid:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoRid), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoRid();
			break;
		case Constants::Enums::ResourceType::Sep:
			//FALLTHROUGH
		case Constants::Enums::ResourceType::Spw:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CYySepRes), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CYySepRes();
			break;
		case Constants::Enums::ResourceType::Vtx:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoVtx), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoVtx();
			break;
		case Constants::Enums::ResourceType::Cib:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoCib), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoCib();
			break;
		case Constants::Enums::ResourceType::Sph:
			mem = StorageLists::instance->GetOrUpper(size + sizeof(CMoSphRes), Constants::Enums::MEM_MODE::Lower);
			CMoResourceMng::SomeByte = 1;
			if (!mem) continue;
			res = new (mem) CMoSphRes();
			break;
		default:
			//std::cout << "unhandled res type: " << (header.Type_Size & 0x7F) << std::endl;
			//char ok[256];
			//sprintf_s(ok, 256, "unhandled res type: %d", header.Type_Size & 0x7F);
			//MessageBoxA(NULL, ok, "", 0);
			mem = StorageLists::instance->GetOrUpper(size + 48, Constants::Enums::MEM_MODE::Lower);
			if (!mem) continue;
			CMoResourceMng::SomeByte = 1;
			res = new (mem) CMoResource();
		}
		
		if (!res) continue;
		res->ConstructFromData(p_buf);
		if (a2) {
			res->Header.RPLHead = a2->PreHeader.PointerToThisInRPL;
		}
		res->PreHeader.field_16 = res->PreHeader.field_16 & 0xFFF0 | 1;
		return res;
	}
}

CMoResource*** FFXI::CYy::CMoResource::SomeAssign(CMoResource*** a1, CMoResource*** a2, CMoResource** a3)
{
	*a1 = a3;
	*a2 = a3;
	return a2;
}

FFXI::CYy::CMoResource::CMoResource()
{
	this->PreHeader.Init();
	this->PreHeader.PointerToThisInRPL = 0;
	this->PreHeader.field_16 = this->PreHeader.field_16 & 0xFFFE | CMoResourceMng::SomeByte & 1;
	CMoResourceMng::SomeByte = 0;
	this->Header.RPLHead = 0;
}

CMoResource* FFXI::CYy::CMoResource::GetHead()
{
	CMoResource** RPLHead = this->Header.RPLHead;
	if (RPLHead)
		return *RPLHead;
	return nullptr;
}

void FFXI::CYy::CMoResource::DoSomething2()
{
	if ((this->Header.Type_Size & 0x8000000) != 0) return;

	CYyDb::pCMoResourceMng->Link(this);

	unsigned int v3 = this->Header.Type_Size & 0x7F;
	if (v3 >= Constants::Values::COUNT_RESOURCE_TYPES)
		v3 = 4;
	CMoResource** v4 = CMoResourceMng::LastResourceCreatedByType[v3];
	if (v4)
		(*v4)->PreHeader.WeirdPointer1 = this->PreHeader.PointerToThisInRPL;
	this->PreHeader.LastResourceCreatedMine = CMoResourceMng::LastResourceCreatedByType[v3];
	CMoResourceMng::LastResourceCreatedByType[v3] = this->PreHeader.PointerToThisInRPL;
	this->Header.LastResourceCreated = CMoResourceMng::LastResourceCreated;
	CMoResourceMng::LastResourceCreated = this->PreHeader.PointerToThisInRPL;
	this->Header.Type_Size |= 0x8000000;
}

CMoResource* FFXI::CYy::CMoResource::DoSomething3()
{
	CMoResource* result;
	CMoResource** RPLHead;
	CMoResource* v3;
	CMoResource** v4;

	for (result = this; ; result = *v4)
	{
		RPLHead = result->Header.RPLHead;
		v3 = RPLHead ? *RPLHead : 0;
		if (v3 == result)
			break;
		v4 = result->Header.RPLHead;
		if (!v4)
			return 0;
	}
	return result;
}

void FFXI::CYy::CMoResource::DoSomething4(char a2)
{
	this->DoSomething2();
	unsigned int v2 = this->Header.Type_Size & 0x7F;
	if ((a2 & 1) != 0) {
		if (v2 == Enums::ResourceType::Spw ||
			v2 == Enums::ResourceType::Sep ||
			v2 == Enums::ResourceType::D3s ||
			v2 == Enums::ResourceType::Bp ||
			v2 == Enums::ResourceType::Bmp2 ||
			v2 == Enums::ResourceType::Tim2) {
			this->Open();
			this->PreHeader.field_16 |= 8u;
			return;
		}
	}
	if ((a2 & 2) != 0) {
		if (v2 == Enums::ResourceType::Texinfo ||
			v2 == Enums::ResourceType::Mmb ||
			v2 == Enums::ResourceType::Rid ||
			v2 == Enums::ResourceType::Os2 ||
			v2 == Enums::ResourceType::Sk2 ||
			v2 == Enums::ResourceType::Mo2 ||
			v2 == Enums::ResourceType::D3m ||
			v2 == Enums::ResourceType::D3a ||
			v2 == Enums::ResourceType::D3b ||
			v2 == Enums::ResourceType::Lfd ||
			v2 == Enums::ResourceType::Msb ||
			v2 == Enums::ResourceType::Meb ||
			v2 == Enums::ResourceType::Mtd ||
			v2 == Enums::ResourceType::Mgb ||
			v2 == Enums::ResourceType::Mtb ||
			v2 == Enums::ResourceType::Vtx) {
			this->Open();
			this->PreHeader.field_16 |= 8u;
			return;
		}
	}
	if ((a2 & 4) != 0) {
		if ((this->PreHeader.field_16 & 8) == 0) {
			this->Open();
			this->PreHeader.field_16 |= 8u;
		}
	}
}

void FFXI::CYy::CMoResource::SomeClean2()
{
	this->SomeClean2Helper();
	this->Close();
}

void FFXI::CYy::CMoResource::SomeClean3()
{
	CMoResource* res = this;
	while (CYyDb::pCMoResourceMng->InList(res->PreHeader.PointerToThisInRPL) == true) {
		CMoResource** RPLHead = res->Header.RPLHead;
		CMoResource* v4 = RPLHead ? *RPLHead : nullptr;

		if (v4 == res) {
			if ((res->PreHeader.field_14 & 0x7FFF) != 0) {
				res->PreHeader.field_14 ^= (res->PreHeader.field_14 ^ (res->PreHeader.field_14 - 1)) & 0x7FFF;
				if ((res->PreHeader.field_14 & 0x7FFF) == 0) {
					res->PreHeader.field_14 &= ~0x8000u;
				}
			}

			return;
		}
		else {
			res->PreHeader.field_14 ^= (res->PreHeader.field_14 ^ (res->PreHeader.field_14 - 1)) & 0x7FFF;
			res = v4;
		}
	}
}

void FFXI::CYy::CMoResource::SomeClean2Helper()
{
	if ((this->Header.Type_Size & 0x8000000) == 0) {
		return;
	}

	CMoResource* SomeRes{ nullptr };
	{//One timer func. Inline it.
		CMoResource* v2{ nullptr };
		if (CMoResourceMng::LastResourceCreated)
			v2 = *CMoResourceMng::LastResourceCreated;
		while (v2 != nullptr && (v2 != this)) {
			SomeRes = v2;
			if (v2->Header.LastResourceCreated == nullptr)
				v2 = nullptr;
			else
				v2 = *v2->Header.LastResourceCreated;
		}
	}

	if (SomeRes != nullptr) {
		SomeRes->Header.LastResourceCreated = this->Header.LastResourceCreated;
	}
	else {
		CMoResourceMng::LastResourceCreated = this->Header.LastResourceCreated;
	}

	this->Header.LastResourceCreated = nullptr;

	if (this->PreHeader.WeirdPointer1 != nullptr) {
		(*this->PreHeader.WeirdPointer1)->PreHeader.LastResourceCreatedMine = this->PreHeader.LastResourceCreatedMine;
	}

	if (this->PreHeader.LastResourceCreatedMine != nullptr) {
		(*this->PreHeader.LastResourceCreatedMine)->PreHeader.WeirdPointer1 = this->PreHeader.WeirdPointer1;
	}

	int v7 = this->Header.Type_Size & 0x7F;
	if (v7 >= Constants::Values::COUNT_RESOURCE_TYPES)
		v7 = 4;

	if (CMoResourceMng::LastResourceCreatedByType[v7] == this->PreHeader.PointerToThisInRPL)
		CMoResourceMng::LastResourceCreatedByType[v7] = this->PreHeader.LastResourceCreatedMine;

	if (this->PreHeader.PreviousRPL != nullptr) {
		(*this->PreHeader.PreviousRPL)->PreHeader.NextRPL = this->PreHeader.NextRPL;
	}
	
	if (this->PreHeader.NextRPL != nullptr) {
		(*this->PreHeader.NextRPL)->PreHeader.PreviousRPL = this->PreHeader.PreviousRPL;
	}

	this->Header.Type_Size &= ~0x8000000u;
}

void __fastcall FFXI::CYy::CMoResource::DoTheThing(CMoResource* a1)
{
	while (true) {
		a1->PreHeader.field_14 ^= (a1->PreHeader.field_14 ^ (a1->PreHeader.field_14 + 1)) & 0x7FFF;
		CMoResource** RPLHead = a1->Header.RPLHead;
		CMoResource* head = RPLHead ? *RPLHead : nullptr;
		if (head == a1)
			break;
		a1 = head;
	}
}

void FFXI::CYy::CMoResource::UnlinkFromManager(CMoResource*** a1)
{
	CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(*a1);
}

CMoResource*** FFXI::CYy::CMoResource::FindResourceUnder(CMoResource*** a2, int a3, int a4)
{
	FindResourceNextUnder(a2, a3, a4, ++CMoResource::SomeCounter);
	return a2;
}

void FFXI::CYy::CMoResource::FindResourceNextUnder(CMoResource*** a2, int a3, int a4, int a5)
{
	CYyResourceFile* head{ nullptr };
	if ((this->Header.Type_Size & 0x7F) == Constants::Enums::ResourceType::Rmp)
		head = (CYyResourceFile*)this;
	else
		head = (CYyResourceFile*)this->GetHead();

	CYyResourceFile* v7 = head;
	while (head) {
		CMoResource** TerminateNode = v7->TerminateNode;
		CMoResource* v9{ nullptr };
		if (TerminateNode)
			v9 = *TerminateNode;

		if (head == v9) {
			if (v7->Header.RPLHead)
				head = (CYyResourceFile*)*v7->Header.RPLHead;
			else
				head = nullptr;

			if (head == v7)
				break;
			v7 = head;
		}
		else {
			if ((head->Header.Type_Size & 0x7F) == Constants::Enums::ResourceType::Rmp) {
				if (head->field_38 == a5) {
					if (head->TerminateNode)
						head = (CYyResourceFile*)*head->TerminateNode;
					else
						head = nullptr;
				}
			}
			if (a3) {
				if ((head->Header.Type_Size & 0x7F) == a3) {
					if (!a4) {
						*a2 = head->PreHeader.PointerToThisInRPL;
						return;
					}
					if (head->Header.FourCC == a4) {
						*a2 = head->PreHeader.PointerToThisInRPL;
						return;
					}
				}
			}
			else if (head->Header.FourCC == a4) {
				*a2 = head->PreHeader.PointerToThisInRPL;
				return;
			}
		}
		if (head->PreHeader.NextRPL)
			head = (CYyResourceFile*)*head->PreHeader.NextRPL;
		else
			head = nullptr;;
	}

	if ((this->Header.Type_Size & 0x7F) == Constants::Enums::Rmp)
		head = (CYyResourceFile*)this;
	else
		head = (CYyResourceFile*)this->GetHead();

	CYyResourceFile** RPLhead = (CYyResourceFile**)head->Header.RPLHead;
	head->field_38 = a5;
	CYyResourceFile* v15{ nullptr };
	if (RPLhead)
		v15 = *RPLhead;
	if (head == v15) {
		if (CYyDb::pCMoResourceMng->Unknown3 == (CYyResourceFile**)this->PreHeader.PointerToThisInRPL)
			*a2 = nullptr;
		else
			(*CYyDb::pCMoResourceMng->Unknown3)->FindResourceNextUnder(a2, a3, a4, a5);
	}
	else if (RPLhead)
		(*RPLhead)->FindResourceNextUnder(a2, a3, a4, a5);
	else
		((CYyResourceFile*)nullptr)->FindResourceNextUnder(a2, a3, a4, a5);
}

CMoResource*** FFXI::CYy::CMoResource::AnotherResourceSearcher(CMoResource*** a2, int a3, int a4)
{
	CMoResource* head = this;
	int v4 = ((head->Header.Type_Size & 0x7F) != 1) - 1;
	while (head) {
		int v5 = head->Header.Type_Size & 0x7F;
		if (v5 == FFXI::Constants::Enums::ResourceType::Rmp)
			++v4;
		if (v5 == 0 && --v4 < 0)
			break;

		if (a3) {
			if (v5 == a3) {
				if (!a4) {
					*a2 = head->PreHeader.PointerToThisInRPL;
					return a2;
				}
				if (head->Header.FourCC == a4) {
					*a2 = head->PreHeader.PointerToThisInRPL;
					return a2;
				}
			}
		}
		else if (head->Header.FourCC == a4) {
			*a2 = head->PreHeader.PointerToThisInRPL;
			return a2;
		}
		if (head->PreHeader.NextRPL)
			head = *(head->PreHeader.NextRPL);
		else
			head = nullptr;
	}
	
	*a2 = nullptr;
	return a2;
}

bool FFXI::CYy::CMoResource::FindResourceWithCallback(int a2, int a3, bool(__cdecl* a4)(CMoResource**, void*), void* a5)
{
	int v8 = ((this->Header.Type_Size & 0x7F) != FFXI::Constants::Enums::ResourceType::Rmp) - 1;
	CMoResource* v5 = this;
	while (v5) {
		int resourceType = v5->Header.Type_Size & 0x7F;
		if (resourceType == FFXI::Constants::Enums::ResourceType::Rmp)
			++v8;
		if (resourceType == FFXI::Constants::Enums::ResourceType::Terminate && --v8 < 0)
			break;
		if (a2) {
			if (resourceType == a2) {
				if (!a3) {
					bool result = a4(v5->PreHeader.PointerToThisInRPL, a5);
					if (result)
						return result;
				}
			}
		}
		if (v5->Header.FourCC == a3) {
			bool result = a4(v5->PreHeader.PointerToThisInRPL, a5);
			if (result)
				return result;
		}
		
		if (v5->PreHeader.NextRPL)
			v5 = *v5->PreHeader.NextRPL;
		else
			v5 = nullptr;
	}

	return false;
}

void FFXI::CYy::CMoResource::GetActivateTime(XiDateTime* a2)
{
	int byte0 = (this->Header.FourCC >> 0) & 0xFF;
	int byte1 = (this->Header.FourCC >> 8) & 0xFF;
	int byte2 = (this->Header.FourCC >> 16) & 0xFF;
	int byte3 = (this->Header.FourCC >> 24) & 0xFF;

	XiDateTime v4{};
	v4.SetHour(byte1 + 10 * byte0 - 0x210);
	v4.SetMinute(byte3 + 10 * byte2 - 0x210);
	a2->Time = v4.Time;
}

void FFXI::CYy::CMoResource::CheckGenerators()
{
	ResourceList v2{};
	v2.PrepareFromResource(this, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
	CYyGenerator* gen = (CYyGenerator*)v2.GetNext(false);
	while (gen) {
		gen->Preset();
		gen = (CYyGenerator*)v2.GetNext(false);
	}
}
