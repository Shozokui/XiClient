#include "MotionReferenceRead.h"
#include "CXiSkeletonActor.h"
#include "CMoResourceMng.h"
using namespace FFXI;

void FFXI::File::MotionReferenceRead::ReadCallback(CYy::CYyResourceFile** a1, ReferenceReadBase* a2)
{
	if (CYy::CMoResourceMng::CheckResourceThing((CYy::CMoResource***) &a1) == true) {
		CYy::CXiSkeletonActor* actor = (CYy::CXiSkeletonActor*)a2->field_4.GetActor();
		if (actor != nullptr) {
			actor->AddMotRes((CYy::CMoResource**)a1, a2->ActorIndex);
		}
	}
	if (a2 != nullptr) {
		delete a2;
	}
}
