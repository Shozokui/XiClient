#include "EquipReferenceRead.h"
#include "CYyResourceFile.h"
#include "CMoResourceMng.h"
#include "CXiSkeletonActor.h"
using namespace FFXI::File;

void FFXI::File::EquipReferenceRead::ReadCallback(CYy::CYyResourceFile** a2, ReferenceReadBase* a3)
{
	EquipReferenceRead* context = (EquipReferenceRead*)a3;
	if (context->field_10 != 0) {
		if (CYy::CMoResourceMng::CheckResourceThing((CYy::CMoResource***)&a2) == true) {
			CYy::CXiActor* baseactor = context->field_4.GetActor();
			if (baseactor) {
				CYy::CXiSkeletonActor* skeletonactor = (CYy::CXiSkeletonActor*)baseactor;
				int v7 = 0;
				if (baseactor->VirtActor226(context->field_14) == true) {
					v7 = skeletonactor->VirtActor225() + 11;
				} 
				else if (skeletonactor->VirtActor222(context->field_14) == true) {
					v7 = skeletonactor->VirtActor221() + 10;
				}
				else if (skeletonactor->VirtActor218(context->field_14) == true) {
					v7 = skeletonactor->VirtActor217() + 2;
				}
				else if (skeletonactor->VirtActor214(context->field_14) == true) {
					v7 = 1;
				}

				if (context->field_10 == skeletonactor->field_768[v7]) {
					skeletonactor->SetEquipModel(a2, context->EquipModelID, context->ActorIndex, true);
				}
			}
		}
	}
	else {
		CYy::CXiSkeletonActor* skeletonactor = (CYy::CXiSkeletonActor*)context->field_4.GetActor();
		if (skeletonactor) {
			skeletonactor->SetEquipModel(a2, context->EquipModelID, context->ActorIndex, CYy::CMoResourceMng::CheckResourceThing((CYy::CMoResource***)&a2));
		}
	}
	
	delete context;
}