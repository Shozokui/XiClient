#include "CYyMenuMemory.h"

using namespace FFXI::CYy;

const CYyClass CYyMenuMemory::CYyMenuMemoryClass{
	"CYyMenuMemory", sizeof(CYyMenuMemory), &CYyObject::CYyObjectClass
};

const CYyClass* CYyMenuMemory::GetRuntimeClass() {
	return &CYyMenuMemoryClass;
}

