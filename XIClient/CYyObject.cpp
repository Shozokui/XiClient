#include "CYyObject.h"
#include "Globals.h"
#include "StorageLists.h"
using namespace FFXI::CYy;

const CYyClass CYyObject::CYyObjectClass{
	"CYyObject", sizeof(CYyObject), nullptr
};

const CYyClass* CYyObject::GetRuntimeClass() {
	return &CYyObject::CYyObjectClass;
}

void FFXI::CYy::CYyObject::VObj2(int*)
{
}

unsigned int FFXI::CYy::CYyObject::VObj3(char)
{
	return 1;
}

int FFXI::CYy::CYyObject::VObj4()
{
	return 1;
}

void FFXI::CYy::CYyObject::VObj5(void*)
{
}

void FFXI::CYy::CYyObject::VObj6()
{
}

bool FFXI::CYy::CYyObject::IsKindOf(const CYyClass* a2)
{
	const CYyClass* v2 = this->GetRuntimeClass();
	while (v2) {
		if (v2 == a2)
			return true;
		v2 = v2->parent;
	}

	return false;
}

void* FFXI::CYy::CYyObject::Wrap(int size, Constants::Enums::MEM_MODE a2)
{
	char* mem = StorageLists::instance->Get(size + sizeof(CYyObject), a2);
	if (!mem)
		return nullptr;

	CYyObject* obj = new (mem) CYyObject();
	return mem + sizeof(CYyObject);
}

void FFXI::CYy::CYyObject::Unwrap(void* a2)
{
	char* obj = (char*)a2 - sizeof(CYyObject);
	StorageLists::instance->Delete((CYyObject*)obj);
}

void* FFXI::CYy::CYyObject::WrapPlus12(int a1, Constants::Enums::MEM_MODE a2)
{
	return (char*)CYyObject::Wrap(a1 + 12, a2) + 12;
}

void FFXI::CYy::CYyObject::UnwrapPlus12(void* a1)
{
	CYyObject::Unwrap((char*)a1 - 12);
}
