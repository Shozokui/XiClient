#include "CMoResourceMng.h"
#include "CTkMsbDataList.h"
#include "CYyDb.h"
#include "XIFileManager.h"
#include "TkManager.h"
#include "CYyResourceFile.h"
#include "CYyBmp2.h"
#include "CYyMsb.h"
#include "a32or27thing.h"
#include "_49SubList.h"
#include "Globals.h"
#include "Enums.h"
#include "CYyScheduler.h"
#include "CMoTaskMng.h"
#include "CYyFileLoadTask.h"
#include "CXiActor.h"
#include <iostream>
using namespace FFXI::CYy;

const CYyClass CMoResourceMng::CMoResourceMngClass{
	"CMoResourceMng", sizeof(CMoResourceMng), &CYyObject::CYyObjectClass
};

CMoResource** CMoResourceMng::LastResourceCreatedByType[Constants::Values::COUNT_RESOURCE_TYPES] = { nullptr };
CMoResource** CMoResourceMng::LastResourceCreated{ nullptr };
int CMoResourceMng::ActiveFileLoadTaskCount{ 0 };
int CMoResourceMng::NumFileIndex{ 0 };
ISAACRandom CMoResourceMng::rng{};
char CMoResourceMng::SomeByte{ 0 };

CYyResourceFile** CMoResourceMng::file27{ nullptr };
CYyScheduler** CMoResourceMng::damv{ nullptr };
CYyScheduler** CMoResourceMng::misv{ nullptr };
CYyScheduler** CMoResourceMng::curv{ nullptr };

CYyOs2VtxBuffer* CMoResourceMng::os2vtxbuffers[16] = { nullptr };
const CYyClass* FFXI::CYy::CMoResourceMng::GetRuntimeClass()
{
	return &CMoResourceMngClass;
}

FFXI::CYy::CMoResourceMng::~CMoResourceMng()
{
	if (CMoResourceMng::file27 != nullptr) {
		(*CMoResourceMng::file27)->SomeClean3();
		CMoResource::UnlinkFromManager((CMoResource***) &CMoResourceMng::file27);
		CMoResourceMng::file27 = nullptr;
	}

	if (this->Unknown3) {
		(*this->Unknown3)->SomeClean3();
		CMoResource::UnlinkFromManager((CMoResource***)&this->Unknown3);
		this->Unknown3 = nullptr;
	}
	if (!CMoResourceMng::LastResourceCreated) return;

	CMoResource* last = *CMoResourceMng::LastResourceCreated;
	if (last) {
		CMoResource* v7{ nullptr };
		do {
			CMoResource** prev = last->Header.LastResourceCreated;
			if (prev)
				v7 = *prev;
			else
				v7 = nullptr;
			last->PreHeader.field_16 |= 4u;
			if (!FFXI::Config::RegistryConfig::g_pOptions->Other.field_28)
				last->PreHeader.field_14 &= 0x8000u;
			CMoResource::UnlinkFromManager(&last->PreHeader.PointerToThisInRPL);
			last = v7;
		} while (v7);
	}
}

FFXI::CYy::CMoResourceMng::CMoResourceMng()
{
	int v1 = sizeof(this->Resources) / sizeof(this->Resources[0]) - 1;
	void** Resources = this->Resources;
	
	for (int i = 0; i < v1; ++i) {
		Resources[i] = Resources + i + 1;
	}

	this->Resources[v1] = nullptr;
	this->NextFreeSlot = this->Resources;
	this->Unknown3 = nullptr;
	this->MaybeResourceCount = 0;

}

int FFXI::CYy::CMoResourceMng::SomeFileMappingFunc(int a1, int a2)
{
	switch (a1) {
	case 1:
		return a2 + 0x996B;
	case 2:
		if (a2 < 0x100)
			return a2 + 0x809F;

		if (a2 < 0x200)
			return a2 + 0xC7EF;

		if (a2 < 0xE00)
			return 0;

		if (a2 < 0xF00 || a2 < 0x1000)
			return a2 + 0xE75B;

		return 0x809F;
	case 3:
		return a2 + 0x1889B;
	case 4:
		return a2 + 0x19131;
	default:
		if (a2 >= 0xDAC)
			return a2 - 0xDAC + 0x18D6B;

		if (a2 >= 0xBB8)
			return a2 - 0xBB8 + 0x18643;

		if (a2 >= 0x5DC)
			return a2 - 0x5DC + 0xCA53;

		return a2 + 0x514;
	}
}

bool __fastcall FFXI::CYy::CMoResourceMng::CheckResourceThing(CMoResource*** a1)
{
	bool result = CYyDb::g_pCYyDb->pCMoResourceMng->InList(*a1);
	if (result == false)
		return false;

	return *a1 && **a1 && (**a1)->GetSomething() == 0;
}

CYyResourceFile** FFXI::CYy::CMoResourceMng::GetOrLoadDatByIndex(int a2)
{
	CMoResource** prmp = CMoResourceMng::LastResourceCreatedByType[FFXI::Constants::Enums::ResourceType::Rmp];
	CYyResourceFile* rmp{ nullptr };
	if (prmp)
		rmp = (CYyResourceFile*)*prmp;

	CMoResource* head{ nullptr };
	while (rmp) {
		head = rmp->GetHead();
		if (head == rmp && rmp->FileIndex == a2) {
			rmp->PreHeader.field_14 |= 0x8000;
			return (CYyResourceFile**)rmp->PreHeader.PointerToThisInRPL;
		}
		if (rmp->PreHeader.LastResourceCreatedMine)
			rmp = (CYyResourceFile*)(*rmp->PreHeader.LastResourceCreatedMine);
		else
			rmp = nullptr;
	}

	CMoTask* loadtask = CYyDb::g_pCYyDb->pCMoTaskMng->FindInQueue(a2, 'Mrt\0');
	CYyFileLoadTask* fileloadtask{ nullptr };
	if (loadtask) {
		fileloadtask = (CYyFileLoadTask*)loadtask;
		return fileloadtask->ResourceFile;
	}
	else {
		char* mem = StorageLists::instance->Get(sizeof(CYyFileLoadTask), FFXI::Constants::Enums::MEM_MODE::Ex);
		CMoTaskMng::DeleteThisTask = true;
		if (mem) {
			fileloadtask = new (mem) CYyFileLoadTask();
			fileloadtask->FileDataBuffer = nullptr;
			fileloadtask->FileStat = nullptr;
		}

		//fileloadtask could be null, as per the client
		CYyDb::g_pCYyDb->pCMoTaskMng->DoSomething(fileloadtask);
		fileloadtask->Init(a2);
		return fileloadtask->ResourceFile;
	}

	return nullptr;
}

CMoResource*** FFXI::CYy::CMoResourceMng::FindNextUnder(CMoResource*** a1, CMoResource* a2, int a3, int a4)
{
	int v5 = ((a2->Header.Type_Size & 0x7F) != 1) - 1;
	CMoResource* v4 = a2;
	while (v4) {
		int type = v4->Header.Type_Size & 0x7F;
		if (type == FFXI::Constants::Enums::ResourceType::Rmp)
			++v5;
		if (type == FFXI::Constants::Enums::ResourceType::Terminate && --v5 < 0) {
			*a1 = nullptr;
			return a1;
		}
		if (a3) {
			if (type == a3) {
				if (!a4 || a4 == v4->Header.FourCC) {
					*a1 = v4->PreHeader.PointerToThisInRPL;
					return a1;
				}
			}
		}
		else if (v4->Header.FourCC == a4) {
			*a1 = v4->PreHeader.PointerToThisInRPL;
			return a1;
		}

		CMoResource** Next = v4->PreHeader.NextRPL;
		if (Next)
			v4 = *Next;
		else
			v4 = nullptr;
	}
	*a1 = nullptr;
	return a1;
}

void FFXI::CYy::CMoResourceMng::InitSomeStatics()
{
	CMoResourceMng::ActiveFileLoadTaskCount = 0;
	memset(CMoResourceMng::LastResourceCreatedByType, 0, sizeof(CMoResourceMng::LastResourceCreatedByType));
	CMoResourceMng::LastResourceCreated = nullptr;
}

void FFXI::CYy::CMoResourceMng::RemoveRes(CMoResource** a2)
{
	if (a2 == nullptr) {
		return;
	}

	void* v2 = *a2;
	int lastIndex = sizeof(this->Resources) / sizeof(this->Resources[0]) - 1;

	if (v2 < this->Resources || v2 > (this->Resources + lastIndex)) {
		*a2 = (CMoResource*)this->NextFreeSlot;
		this->NextFreeSlot = a2;
		this->MaybeResourceCount -= 1;
		if (this->MaybeResourceCount < 0)
			this->MaybeResourceCount = 0;
	}
}

void FFXI::CYy::CMoResourceMng::Unlink(CMoResource** a2)
{
	CMoResource* v2 = *a2;
	if ((v2->PreHeader.field_14 & 0x7FFF) == 0) {
		if ((v2->PreHeader.field_16 & 4) == 0)
		{
			CMoResource* v3 = v2->DoSomething3();
			if ((v3->PreHeader.field_14 & 0x7FFF) != 0)
				return;

			CMoResource* v7{ nullptr };
			if (v3->PreHeader.NextRPL)
				v7 = *v3->PreHeader.NextRPL;

			if (v7) {
				CMoResource* v9{ nullptr };
				do {
					if (v7->PreHeader.NextRPL)
						v9 = *v7->PreHeader.NextRPL;
					else
						v9 = nullptr;
					v7->CMoResource::Close();
					v7 = v9;
				} while (v9);
			}

			v2 = v3;
		}
		v2->CMoResource::Close();
	}
}

CYyResourceFile*** FFXI::CYy::CMoResourceMng::LoadNumFile(CYyResourceFile*** a2, int a3)
{
	CYyResourceFile** piVar6{ nullptr };
	this->SetNumFileIndex(a3);
	CYyResourceFile** Last = (CYyResourceFile**)CMoResourceMng::LastResourceCreatedByType[Constants::Enums::ResourceType::Rmp];
	if (Last) {
		piVar6 = (CYyResourceFile**)(*Last)->PreHeader.PointerToThisInRPL;
		while (piVar6 != nullptr) {
			CYyResourceFile* piVar4 = (CYyResourceFile*)*piVar6;
			if (piVar4->GetHead() == piVar4 && piVar4->FileIndex == a3) {
				(*piVar6)->PreHeader.field_14 |= 0x80;
				*a2 = piVar6;
				return a2;
			}
			piVar6 = (CYyResourceFile**)piVar4->PreHeader.LastResourceCreatedMine;
		}
	}
	unsigned int filesize = FFXI::File::XIFileManager::g_pXIFileManager->GetFileSizeByNumfile(a3);
	if (filesize > 0) {
		char* buffer = new char[filesize];
		if (buffer) {
			int iVar3 = FFXI::File::XIFileManager::g_pXIFileManager->ReadNumfileNow(a3, buffer, filesize, 0);
			if (iVar3 != 0) {
				CYyResourceFile** v13{ nullptr };
				this->Parse(buffer, filesize, &v13, nullptr);
				if (v13 && *v13) {
					(*v13)->FileIndex = a3;
					delete[] buffer;
					*a2 = v13;
					return a2;
				}
			}
			delete[] buffer;
		}
	}
	*a2 = nullptr;
	return a2;
}

CYyResourceFile*** FFXI::CYy::CMoResourceMng::LoadNumFile2(CYyResourceFile*** a2, int a3)
{
	CYyResourceFile** piVar6{ nullptr };
	this->SetNumFileIndex(a3);
	CYyResourceFile** Last = (CYyResourceFile**)CMoResourceMng::LastResourceCreatedByType[Constants::Enums::ResourceType::Rmp];
	if (Last) {
		piVar6 = (CYyResourceFile**)(*Last)->PreHeader.PointerToThisInRPL;
		while (piVar6 != nullptr) {
			CYyResourceFile* piVar4 = reinterpret_cast<CYyResourceFile*>(*piVar6);
			if (piVar4->GetHead() == piVar4 && piVar4->FileIndex == a3) {
				(*piVar6)->PreHeader.field_14 |= 0x80;
				*a2 = piVar6;
				return a2;
			}
			piVar6 = (CYyResourceFile**)piVar4->PreHeader.LastResourceCreatedMine;
		}
	}
	unsigned int filesize = FFXI::File::XIFileManager::g_pXIFileManager->GetFileSizeByNumfile(a3);
	if (filesize > 0) {
		char* buffer = new char[filesize];
		if (buffer) {
			int iVar3 = FFXI::File::XIFileManager::g_pXIFileManager->ReadNumfileNow(a3, buffer, filesize, 0);
			if (iVar3 != -1) {
				CYyResourceFile** v13{ nullptr };
				this->Parse(buffer, filesize, &v13, nullptr);
				if (v13 && *v13) {
					CYyResourceFile* v15{ nullptr };
					(*v13)->FileIndex = a3;
					delete[] buffer;
					*a2 = v13;
					return a2;
				}
			}
			delete[] buffer;
		}
	}
	*a2 = nullptr;
	return a2;
}

CYyResourceFile*** FFXI::CYy::CMoResourceMng::Parse(char* p_buf, int p_buflen, CYyResourceFile*** a2, CMoResource** a4)
{
	CYyResourceFile** headPtr{ nullptr };
	if ((p_buf[4] & 0x7F) == Constants::Enums::ResourceType::Rmp) {
		CYyResourceFile* head = (CYyResourceFile*)CMoResource::OpenResource(p_buf, nullptr);
		if (!head) {
			*a2 = nullptr;
			return a2;
		}
		if (a4) {
			*a4 = head;
			this->RemoveRes(head->PreHeader.PointerToThisInRPL);
			head->PreHeader.PointerToThisInRPL = a4;
		}

		head->DoSomething2();
		head->Header.RPLHead = head->PreHeader.PointerToThisInRPL;
		head->DoSomething3()->PreHeader.field_18 = 0;

		headPtr = (CYyResourceFile**)head->PreHeader.PointerToThisInRPL;
		CYyResourceFile** headList[0x100];

		int pos = 16 * ((*((int*)p_buf + 1) >> 7) & 0x7FFFF);

		int headIndex = 0;
		headList[headIndex] = headPtr;

		CMoResource* lastRes = head;
		p_buf += pos;
		while (p_buf) {
			CMoResource* newRes = CMoResource::OpenResource(p_buf, head);
			this->Link(newRes);

			int newType = newRes->Header.Type_Size & 0x7F;
			if (newType == Constants::Enums::ResourceType::Rmp) {
				CYyResourceFile* newHead = (CYyResourceFile*)newRes;
				head = newHead;
				headList[++headIndex] = (CYyResourceFile**)newHead->PreHeader.PointerToThisInRPL;
			}
			if (newType == Constants::Enums::ResourceType::Bmp2 ||
				newType == Constants::Enums::ResourceType::Msb) {
				char* mem = StorageLists::instance->GetOrUpper(sizeof(a32or27thing), Constants::Enums::MEM_MODE::Lower);
				CMoResourceMng::SomeByte = 1;
				a32or27thing* newGraphic = new (mem) a32or27thing();
				CYyBmp2* casted = (CYyBmp2*)newRes;

				newGraphic->Header.FourCC = casted->Header.FourCC;
				newGraphic->Header.Type_Size = 0x20A;
				newGraphic->Header.RPLHead = nullptr;
				newGraphic->Header.LastResourceCreated = nullptr;
				newGraphic->Init(casted->Data + 1);
				newGraphic->Header.RPLHead = head->PreHeader.PointerToThisInRPL;
				newGraphic->PreHeader.field_16 = 1;

				this->Link(newGraphic);
				lastRes->PreHeader.SetNextRPL(newGraphic);
				newGraphic->PreHeader.SetPrevRPL(lastRes);
				newGraphic->PreHeader.SetNextRPL(casted);
				casted->PreHeader.SetPrevRPL(newGraphic);
			}
			else {
				lastRes->PreHeader.SetNextRPL(newRes);
				newRes->PreHeader.SetPrevRPL(lastRes);
			}

			newRes->PreHeader.field_16 &= 0xFFFFFFFD;

			if ((newRes->Header.Type_Size & 0x7F) == Constants::Enums::ResourceType::Terminate) {
				
				if (head->Header.RPLHead)
					head = (CYyResourceFile*)*head->Header.RPLHead;
				else
					head = nullptr;

				CYyResourceFile** popHead = headList[headIndex];
				if (popHead && *popHead)
					(*popHead)->TerminateNode = newRes->PreHeader.PointerToThisInRPL;

				if (headIndex <= 0)
					break;
				--headIndex;
			}

			lastRes = newRes;
			int resSize = 16 * ((newRes->Header.Type_Size >> 7) & 0x7FFFF);
			p_buf += resSize;
		}

		CMoResource* res{ nullptr };

		if (head->PreHeader.NextRPL) {
			int value = 1;
			for (int i = 0; i < 3; ++i) {
				res = *head->PreHeader.NextRPL;

				while (res) {
					res->DoSomething4(value);
					if (res->PreHeader.NextRPL)
						res = *res->PreHeader.NextRPL;
					else
						res = nullptr;
				}
				value *= 2;
			}
		}
	}
	else {
		throw "does this ever happen";
	}

	if (this->Unknown3 != nullptr)
		(*headPtr)->CheckGenerators();

	*a2 = headPtr;
	return a2;
}

FFXI::CTk::_49SubList*** FFXI::CYy::CMoResourceMng::LoadNumFileGet49(int a2, CYyResourceFile*** a3)
{
	CYyResourceFile** a0{ nullptr };
	CYyResourceFile*** NumFile = this->LoadNumFile(&a0, a2);
	*a3 = *NumFile;
	CMoResource::DoTheThing(**NumFile);

	CYyMsb** result{ nullptr };
	
	(**NumFile)->FindResourceUnder((CMoResource***)&result, Constants::Enums::ResourceType::Msb, 0);

	if (result)
		return CTk::TkManager::g_CTkMenuDataList.GetSubLists((char*)&(*result)[1]);
	
	return nullptr;
}

CMoResource** FFXI::CYy::CMoResourceMng::Link(CMoResource* a3)
{
	CMoResource** res = a3->PreHeader.PointerToThisInRPL;
	if (!res) {
		res = (CMoResource**)(this->NextFreeSlot);
		if (!res) {
			return nullptr;
		}
		this->NextFreeSlot = *res;
		*res = a3;
		a3->PreHeader.PointerToThisInRPL = res;
		this->MaybeResourceCount += 1;
	}
	
	return res;
}

bool FFXI::CYy::CMoResourceMng::InList(CMoResource** a2)
{
	if (a2 == nullptr)
		return false;

	const int lastIndex = sizeof(this->Resources) / sizeof(this->Resources[0]) - 1;

	if ((int)a2 < (int)this->Resources) 
		return false;
	if ((int)a2 > (int)(this->Resources + lastIndex))
		return false;

	if ((int)(*a2) < (int)this->Resources)
		return true;
	if ((int)(*a2) > (int)(this->Resources + lastIndex))
		return true;

	return false;
}

void FFXI::CYy::CMoResourceMng::SetNumFileIndex(int p_index)
{
	CMoResourceMng::NumFileIndex = p_index;
}

void FFXI::CYy::CMoResourceMng::LoadStartScheduler(int a2, int a3, CXiActor* a4, CXiActor* a5, unsigned int a6)
{
	CYyResourceFile** unused{ nullptr };
	CMoResource** unused2{ nullptr };

	CYyResourceFile** file = *this->LoadNumFile(&unused, a2);
	(*file)->PreHeader.field_16 |= 0x10;
	CYyScheduler** scheduler = *(CYyScheduler***)(*file)->FindResourceUnder(&unused2, Constants::Enums::ResourceType::Scheduler, a3);
	if (scheduler)
		(*scheduler)->CreateStartTask(a4, a5, a6, nullptr);
}

int FFXI::CYy::CMoResourceMng::GetNumFileIndex()
{
	return CMoResourceMng::NumFileIndex;
}

void FFXI::CYy::CMoResourceMng::InitUnk3(int a1)
{
	this->LoadNumFile2(&this->Unknown3, a1);
	if (this->Unknown3 != nullptr) {
		CMoResource::DoTheThing(*this->Unknown3);
		(*this->Unknown3)->CheckGenerators();
	}

	this->LoadNumFile(&CMoResourceMng::file27, 27);
	if (file27 != nullptr) {
		CMoResource** unused{ nullptr };
		CMoResource::DoTheThing(*file27);
		CMoResourceMng::damv = *(CYyScheduler***)(*file27)->FindResourceUnder(&unused, FFXI::Constants::Enums::ResourceType::Scheduler, 'vmad');
		CMoResourceMng::misv = *(CYyScheduler***)(*file27)->FindResourceUnder(&unused, FFXI::Constants::Enums::ResourceType::Scheduler, 'vsim');
		CMoResourceMng::curv = *(CYyScheduler***)(*file27)->FindResourceUnder(&unused, FFXI::Constants::Enums::ResourceType::Scheduler, 'vruc');
	}
	//sub //TODO
	//doesn't seem like this is used anywhere
}

void** FFXI::CYy::CMoResourceMng::ReserveSlot()
{
	void** nextfree = (void**)this->NextFreeSlot;
	if (nextfree) {
		this->NextFreeSlot = *nextfree;
		*nextfree = nullptr;
		return nextfree;
	}

	return nullptr;
}
