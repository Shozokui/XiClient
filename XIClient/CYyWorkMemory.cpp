#include "CYyWorkMemory.h"

using namespace FFXI::CYy;

const CYyClass CYyWorkMemory::CYyWorkMemoryClass{
	"CYyWorkMemory", sizeof(CYyWorkMemory), &CYyObject::CYyObjectClass
};

const CYyClass* CYyWorkMemory::GetRuntimeClass() {
	return &CYyWorkMemoryClass;
}

