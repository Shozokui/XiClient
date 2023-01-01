#include "CXiAtelActor.h"
#include "Globals.h"
using namespace FFXI::CYy;

const CYyClass CXiAtelActor::CXiAtelActorClass{
	"CXiAtelActor", sizeof(CXiAtelActor), &CXiActor::CXiActorClass
};

const CYyClass* FFXI::CYy::CXiAtelActor::GetRuntimeClass()
{
	return &CXiAtelActor::CXiAtelActorClass;
}

char FFXI::CYy::CXiAtelActor::OnMove()
{
	this->field_C4 = this->field_34;
	Globals::PiClamp(&this->field_44);
	return 0;
}
