#include "CYyUpperMemory.h"

using namespace FFXI::CYy;

const CYyClass CYyUpperMemory::CYyUpperMemoryClass{
	"CYyUpperMemory", sizeof(CYyUpperMemory), &CYyObject::CYyObjectClass
};

const CYyClass* CYyUpperMemory::GetRuntimeClass() {
	return &CYyUpperMemoryClass;
}

