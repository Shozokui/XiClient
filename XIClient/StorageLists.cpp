#include "StorageLists.h"
#include "CYyLowerMemory.h"
#include "CYyUpperMemory.h"
#include "CYyElemMemory.h"
#include "CYyWorkMemory.h"
#include "CYyMenuMemory.h"
#include "CYyExMemory.h"
#include "CYyLoadMemory.h"
#include <iostream>
#include <Windows.h>
using namespace FFXI;

StorageLists* StorageLists::instance{ nullptr };
CYy::MemoryHeader* StorageLists::g_pCYyDataHead{ nullptr };
CYy::MemoryHeader* StorageLists::g_pCYyDataTail{ nullptr };
CYy::MemoryHeader* StorageLists::g_pCYyElemHead{ nullptr };
CYy::MemoryHeader* StorageLists::g_pCYyElemTail{ nullptr };
CYy::MemoryHeader* StorageLists::g_pCYyLoadHead{ nullptr };
CYy::MemoryHeader* StorageLists::g_pCYyLoadTail{ nullptr };
CYy::MemoryHeader* StorageLists::g_pCYyMenuHead{ nullptr };
CYy::MemoryHeader* StorageLists::g_pCYyMenuTail{ nullptr };
CYy::MemoryHeader* StorageLists::g_pCYyExHead{ nullptr };
CYy::MemoryHeader* StorageLists::g_pCYyExTail{ nullptr };

//LOCAL FUNCS
int FindLargestSlot(CYy::MemoryHeader* p_head, CYy::MemoryHeader* p_tail) {
	int size = 0;
	do {
		if (!p_head->occupied && size < p_head->GetSize())
			size = p_head->GetSize();
		p_head = p_head->field_10;
	} while (p_head != p_tail);
	return size;
}
bool FFXI::StorageLists::Initialize()
{
	StorageLists::instance = new StorageLists();
	if (StorageLists::instance == nullptr) {
		return false;
	}
	return StorageLists::instance->Init();
}
void FFXI::StorageLists::Uninitialize()
{
	if (StorageLists::instance != nullptr) {
		delete StorageLists::instance;
		StorageLists::instance = nullptr;
	}
}
//~LOCAL FUNCS
StorageLists::~StorageLists() {
	if (this->field_C) {
		delete[] this->field_C;
		this->field_C = nullptr;
	}
}

CYy::MemoryHeader* AllocateLinkedList(char* Buffer, int Size, CYy::MemoryHeader** Head, CYy::MemoryHeader** Tail) {
	CYy::MemoryHeader* head; // eax
	int* v5; // ebx
	CYy::MemoryHeader* tail; // edi
	unsigned int v9;

	*Head = (CYy::MemoryHeader*)Buffer;
	*Tail = (CYy::MemoryHeader*)(Buffer + (Size & 0xFFFFFFF0) - 16);
	head = *Head;
	head->PreviousEntry = nullptr;
	tail = *Tail;
	head->field_14 = nullptr;
	head->NextEntry = tail;
	head->field_10 = tail;
	head->occupied = false;
	head->field_18 &= 0xFFFFFFFC;
	head->field_1C = 0;
	tail->PreviousEntry = head;
	tail->NextEntry = nullptr;
	tail->field_10 = nullptr;
	tail->field_14 = head;
	tail->field_1C = 0;
	tail->occupied = true;
	tail->field_18 &= 0xFFFFFFFC;
	return tail;
}
bool StorageLists::Init() {
	unsigned int v2; // ecx
	signed __int32 v3; // eax
	bool v4; // cc
	int v5; // edi
	int v6; // eax
	char* v7; // eax
	char* v8; // eax
	char* v10; // eax
	char* v11;
	MEMORYSTATUS Buffer{};
	memset(&Buffer, 0, sizeof(Buffer));
	Buffer.dwLength = 32;
	GlobalMemoryStatus(&Buffer);
	v2 = 0x5A02000;
	v3 = Buffer.dwTotalPhys >> 20;
	v4 = Buffer.dwTotalPhys >> 20 <= 0x80;
	this->field_4 = Buffer.dwTotalPhys >> 20;
	v5 = 0x4000000;
	if (!v4)
	{
		if (v3 > 256)
			this->field_4 = 256;
		v6 = (this->field_4 - 128) << 20;
		v5 = v6 + 0x4000000;
		v2 = v6 + 0x5A02000;
	}
	v7 = new char[v2];
	this->field_C = v7;
	if (!v7)
	{
		v2 = 0x5A02000u;
		v5 = 0x4000000;
		this->field_C = new char[0x5A02000u];
	}
	v8 = this->field_C;
	if (!v8)
		return false;
	memset(v8, 0, v2);
	v10 = v8 + 15;
	int ok = (int)v10;
	ok &= 0xFFFFFFF0;
	v10 = reinterpret_cast<char*>(ok);
	this->field_8 = v10;
	v11 = v10 + v5;

	AllocateLinkedList(v10, v5 - 128, &g_pCYyDataHead, &g_pCYyDataTail);
	AllocateLinkedList(v11, 0x9FFF80, &g_pCYyElemHead, &g_pCYyElemTail);
	AllocateLinkedList(v11 + 0xA00000, 0xF80, &g_pCYyLoadHead, &g_pCYyLoadTail);
	AllocateLinkedList(v11 + 0xA01000, 0xFFFF80, &g_pCYyExHead, &g_pCYyExTail);
	AllocateLinkedList(v11 + 0x1A01000, 0xF80, &g_pCYyMenuHead, &g_pCYyMenuTail);

	return true;
}

CYy::MemoryHeader* GetFromHead(int p_size, CYy::MemoryHeader* p_head, CYy::MemoryHeader* p_tail) {
	CYy::MemoryHeader* head = p_head;
	CYy::MemoryHeader* tail = p_tail;
	int size = p_size + 15;
	size &= 0xFFFFFFF0;
	if (size < 32) size = 32;
	while (head->occupied || head->GetSize() < (size + 32)) {
		head = head->field_10;
		if (head == tail) return 0;
	}
	CYy::MemoryHeader* NextEntry = head->NextEntry;
	CYy::MemoryHeader* NextFreeByteLocation = reinterpret_cast<CYy::MemoryHeader*>(reinterpret_cast<char*>(head) + size + 32);
	//head->field_1c = dword???
	head->occupied = true;
	head->field_18 &= 0xFFFFFFFC;
	if (NextEntry == NextFreeByteLocation) {
		throw "NEXT ENTRY EQUALS NEXT FREEBYTE";
	} 
	else {
		NextFreeByteLocation->occupied = 0;
		NextFreeByteLocation->NextEntry = head->NextEntry;
		NextFreeByteLocation->PreviousEntry = head;
		NextEntry->PreviousEntry = NextFreeByteLocation;
		CYy::MemoryHeader* v10 = head->field_10;
		head->NextEntry = NextFreeByteLocation;
		NextFreeByteLocation->field_10 = v10;
		v10->field_14 = NextFreeByteLocation;
		CYy::MemoryHeader* v11 = head->field_14;
		if (v11) {
			NextFreeByteLocation->field_14 = v11;
			v11->field_10 = NextFreeByteLocation;
		}
		else {
			NextFreeByteLocation->field_14 = head;
			head->field_10 = NextFreeByteLocation;
		}
		return head;
	}
}

CYy::MemoryHeader* GetFromTail(int p_size, CYy::MemoryHeader* p_head, CYy::MemoryHeader* p_tail) {
	CYy::MemoryHeader* tail = p_tail;

	int size = p_size + 15;
	size &= 0xFFFFFFF0;
	if (size < 32) size = 32;
	do {
		if (!tail->PreviousEntry) 
			return nullptr;
		tail = tail->field_14;
	} while (tail->occupied || tail->GetSize() < (size + 32));
	
	CYy::MemoryHeader* NextEntry = tail->NextEntry;
	CYy::MemoryHeader* FreeSpot = (CYy::MemoryHeader*)((char*)NextEntry - size - 32);
	//FreeSpot->field_1C = dword_oeifjeafe?
	//sub //TODO
	if (FreeSpot == tail) {
		if (tail->field_14) {
			tail->field_14->field_10 = tail->field_10;
			tail->field_10->field_14 = tail->field_14;
		}

		tail->occupied = true;
		tail->field_18 &= 0xFFFFFFFC;
		return tail;
	}
	else {
		FreeSpot->NextEntry = NextEntry;
		FreeSpot->PreviousEntry = tail;
		NextEntry->PreviousEntry = FreeSpot;
		tail->NextEntry = FreeSpot;
		FreeSpot->occupied = true;
		FreeSpot->field_18 &= 0xFFFFFFFC;
		return FreeSpot;
	}
}

char* StorageLists::Get(int p_size, Constants::Enums::MEM_MODE p_mode) {
	CYy::MemoryHeader* header{ nullptr };
	//sub //TODO
	//don't copy vtable.. create a new base class with only vtable
	switch (p_mode) {
	case Constants::Enums::MEM_MODE::Upper:
		header = GetFromHead(p_size + sizeof(CYy::MemoryHeader), g_pCYyDataHead, g_pCYyDataTail);
		if (header) {
			CYy::CYyUpperMemory v1{};
			memcpy(header, &v1, 4);
			return (char*)(header + 1);
		}
		break;
	case Constants::Enums::MEM_MODE::Lower:
		header = GetFromTail(p_size + sizeof(CYy::MemoryHeader), g_pCYyDataHead, g_pCYyDataTail);
		if (header) {
			CYy::CYyLowerMemory v1{};
			memcpy(header, &v1, 4);
			return (char*)(header + 1);
		}
		break;
	case Constants::Enums::MEM_MODE::Elem:
		header = GetFromTail(p_size + sizeof(CYy::MemoryHeader), g_pCYyElemHead, g_pCYyElemTail);
		if (header) {
			CYy::CYyElemMemory v1{};
			memcpy(header, &v1, 4);
			return (char*)(header + 1);
		}
		break;
	case Constants::Enums::MEM_MODE::Work:
		header = GetFromTail(p_size + sizeof(CYy::MemoryHeader), g_pCYyElemHead, g_pCYyElemTail);
		if (header) {
			CYy::CYyWorkMemory v1{};
			memcpy(header, &v1, 4);
			return (char*)(header + 1);
		}
		break;
	case Constants::Enums::MEM_MODE::Load:
		header = GetFromTail(p_size + sizeof(CYy::MemoryHeader), g_pCYyLoadHead, g_pCYyLoadTail);
		if (header) {
			CYy::CYyLoadMemory v1{};
			memcpy(header, &v1, 4);
			return (char*)(header + 1);
		}
		break;
	case Constants::Enums::MEM_MODE::Menu:
		header = GetFromTail(p_size + sizeof(CYy::MemoryHeader), g_pCYyMenuHead, g_pCYyMenuTail);
		if (header) {
			CYy::CYyMenuMemory v1{};
			memcpy(header, &v1, 4);
			return (char*)(header + 1);
		}
		break;
	default:
		//std::cout << std::hex << "EX ALLOCATE: " << p_size + 32 << " / " << ((FFXI::Object::MemoryHeader*)Globals::g_pCYyExTail)->GetFreeSize() << std::endl;
		header = GetFromTail(p_size + sizeof(CYy::MemoryHeader), g_pCYyExHead, g_pCYyExTail);
		if (!header) break;
		CYy::CYyExMemory v1{};
		memcpy(header, &v1, 4);
		return (char*)(header + 1);
	}
	exit(2002);
}

char* FFXI::StorageLists::GetOrUpper(int p_size, Constants::Enums::MEM_MODE p_mode)
{
	char* result = this->Get(p_size, p_mode);
	if (result || p_mode != Constants::Enums::MEM_MODE::Elem && p_mode != Constants::Enums::MEM_MODE::Work)
		return result;
	result = this->Get(p_size, Constants::Enums::MEM_MODE::Upper);
	if (!result)
		exit(2002);
	return result;
}

int FFXI::StorageLists::FindLargestOpenSlot(Constants::Enums::MEM_MODE p_mode)
{
	CYy::MemoryHeader* head{ nullptr }, * tail{ nullptr };
	switch (p_mode) {
	case Constants::Enums::MEM_MODE::Upper:
	case Constants::Enums::MEM_MODE::Lower:
		head = reinterpret_cast<CYy::MemoryHeader*>(g_pCYyDataHead);
		tail = reinterpret_cast<CYy::MemoryHeader*>(g_pCYyDataTail);
		break;
	case Constants::Enums::MEM_MODE::Elem:
	case Constants::Enums::MEM_MODE::Work:
		head = reinterpret_cast<CYy::MemoryHeader*>(g_pCYyElemHead);
		tail = reinterpret_cast<CYy::MemoryHeader*>(g_pCYyElemTail);
		break;
	case Constants::Enums::MEM_MODE::Load:
		head = reinterpret_cast<CYy::MemoryHeader*>(g_pCYyLoadHead);
		tail = reinterpret_cast<CYy::MemoryHeader*>(g_pCYyLoadTail);
		break;
	case Constants::Enums::MEM_MODE::Menu:
		head = reinterpret_cast<CYy::MemoryHeader*>(g_pCYyMenuHead);
		tail = reinterpret_cast<CYy::MemoryHeader*>(g_pCYyMenuTail);
		break;
	default:
		head = reinterpret_cast<CYy::MemoryHeader*>(g_pCYyExHead);
		tail = reinterpret_cast<CYy::MemoryHeader*>(g_pCYyExTail);
	}
	return FindLargestSlot(head, tail);
}

void FFXI::StorageLists::Delete(CYy::CYyObject* p_obj)
{
	CYy::MemoryHeader* header = (CYy::MemoryHeader*)((char*)p_obj - sizeof(CYy::MemoryHeader));
	if (header->occupied == false) return;

	header->occupied = false;

	CYy::MemoryHeader* v1 = header;
	CYy::MemoryHeader* v2 = header->PreviousEntry;
	if (v2 && !v2->occupied) {
		v2->NextEntry = header->NextEntry;
		header->NextEntry->PreviousEntry = v2;
		v1 = v2;
	}
	CYy::MemoryHeader* v3 = v1->NextEntry;
	if (!v3->occupied) {
		v1->NextEntry = v3->NextEntry;
		v3->NextEntry->PreviousEntry = v1;
	}
	CYy::MemoryHeader* v4 = v1->PreviousEntry;
	CYy::MemoryHeader* v5 = v1;
	if (v4) {
		while (v4->occupied) {
			v5 = v4;
			v4 = v4->PreviousEntry;
			if (!v4)
				goto LABEL_12;
		}
		v5 = v5->PreviousEntry;
	}
LABEL_12:
	if (v5->PreviousEntry || v1->PreviousEntry) {
		v5->field_10 = v1;
		v1->field_14 = v5;
	}
	CYy::MemoryHeader* v6 = v1->NextEntry;
	CYy::MemoryHeader* v7 = v1;
	if (v6) {
		while (v6->occupied) {
			v7 = v6;
			v6 = v6->NextEntry;
			if (!v6)
				goto LABEL_20;
		}
		v7 = v7->NextEntry;
	}
LABEL_20:
	if (v7->NextEntry || v1->NextEntry) {
		v7->field_14 = v1;
		v1->field_10 = v7;
	}
}
void CleanList(int* a1, FFXI::CYy::MemoryHeader* a2, FFXI::CYy::MemoryHeader* a3) {
	while (a2 != a3) {
		if (a2->occupied) {
			if ((a2->field_18 & 2) == 0) {
				CYy::CYyObject* obj = (CYy::CYyObject*)(a2 + 1);
				obj->VObj2(a1);
			}
		}
		a2 = a2->NextEntry;
	}
}
void FFXI::StorageLists::RunClean(int* a2)
{
	CleanList(a2, g_pCYyExHead, g_pCYyExTail);
	CleanList(a2, g_pCYyLoadHead, g_pCYyLoadTail);
	CleanList(a2, g_pCYyElemHead, g_pCYyElemTail);
	CleanList(a2, g_pCYyDataHead, g_pCYyDataTail);
}

void FFXI::StorageLists::Clean123()
{
	if (StorageLists::instance == nullptr) {
		return;
	}

	for (int i = 1; i < 4; ++i) {
		StorageLists::instance->RunClean(&i);
	}
}

void FFXI::StorageLists::Clean456()
{
	if (StorageLists::instance == nullptr) {
		return;
	}

	for (int i = 4; i < 7; ++i) {
		StorageLists::instance->RunClean(&i);
	}
}
