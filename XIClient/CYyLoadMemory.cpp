#include "CYyLoadMemory.h"

using namespace FFXI::CYy;

const CYyClass CYyLoadMemory::CYyLoadMemoryClass{
	"CYyLoadMemory", sizeof(CYyLoadMemory), &CYyObject::CYyObjectClass
};

const CYyClass* CYyLoadMemory::GetRuntimeClass() {
	return &CYyLoadMemoryClass;
}

