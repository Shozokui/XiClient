#include "StorageListObject.h"
#include "Globals.h"
using namespace FFXI::CYy;

void StorageListObject::operator delete(void* ptr) {
	StorageLists::instance->Delete((CYyObject*)ptr);
}

void FFXI::CYy::StorageListObject::operator delete[](void* ptr)
{
	StorageLists::instance->Delete((CYyObject*)ptr);
}
