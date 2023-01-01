#include "CXiSkeletonActorRes.h"
using namespace FFXI::CYy;

FFXI::CYy::CXiSkeletonActorRes::CXiSkeletonActorRes(CXiSkeletonActor* a2, void(__cdecl* a3)(void))
{
	this->Actor = a2;
	this->Callback = a3;
}
