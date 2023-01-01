#include "CXiDollActor.h"
using namespace FFXI::CYy;

const CYyClass CXiDollActor::CXiDollActorClass{
	"CXiDollActor", sizeof(CXiDollActor), &CXiActor::CXiActorClass
};
const CYyClass* FFXI::CYy::CXiDollActor::GetRuntimeClass()
{
	return &CXiDollActor::CXiDollActorClass;
}
