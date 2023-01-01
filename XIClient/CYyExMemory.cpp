#include "CYyExMemory.h"

using namespace FFXI::CYy;

const CYyClass CYyExMemory::CYyExMemoryClass{
	"CYyExMemory", sizeof(CYyExMemory), &CYyObject::CYyObjectClass
};
const CYyClass* CYyExMemory::GetRuntimeClass() {
	return &CYyExMemoryClass;
}

