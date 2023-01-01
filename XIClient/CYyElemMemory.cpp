#include "CYyElemMemory.h"

using namespace FFXI::CYy;

const CYyClass CYyElemMemory::CYyElemMemoryClass{
	"CYyElemMemory", sizeof(CYyElemMemory), &CYyObject::CYyObjectClass
};

const CYyClass* CYyElemMemory::GetRuntimeClass() {
	return &CYyElemMemoryClass;
}

