#include "CYyLowerMemory.h"

using namespace FFXI::CYy;

const CYyClass CYyLowerMemory::CYyLowerMemoryClass{
	"CYyLowerMemory", sizeof(CYyLowerMemory), &CYyObject::CYyObjectClass
};

const CYyClass* CYyLowerMemory::GetRuntimeClass() {
	return &CYyLowerMemoryClass;
}

