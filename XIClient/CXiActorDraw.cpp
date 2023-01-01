#include "CXiActorDraw.h"
#include "CXiActor.h"
#include "FsConfig.h"
using namespace FFXI::CYy;

const CYyClass FFXI::CYy::CXiActorDraw::CXiActorDrawClass{
	"CXiActorDraw", sizeof("CXiActorDraw"), &CMoTask::CMoTaskClass
};

const CYyClass* FFXI::CYy::CXiActorDraw::GetRuntimeClass()
{
	return &CXiActorDraw::CXiActorDrawClass;
}

char FFXI::CYy::CXiActorDraw::OnMove()
{
	//sub //todo
	CXiActor::config60 = Config::FsConfig::GetConfig(Constants::Enums::FsConfigSubjects::Subject60);
	//Statics

	CXiActor* actor = CXiActor::top;
	while (actor != nullptr) {
		CXiActor* prev = actor->PreviousActor;
		if (prev != nullptr && prev->field_8C < actor->field_8C) {
			if (prev->field_B2 == 0) {
				if (prev->field_8C < 0.0) {
					prev->field_B0 = 1;
					prev->field_B2 |= 1;
				}
				else {
					prev->OnDraw();
				}
			}

			if (actor->NextActor != nullptr) {
				actor->NextActor->PreviousActor = prev;
			}
			else {
				CXiActor::top = prev;
			}

			if (prev->PreviousActor != nullptr) {
				prev->PreviousActor->NextActor = actor;
			}

			actor->PreviousActor = prev->PreviousActor;
			prev->NextActor = actor->NextActor;
			actor->NextActor = prev;
			prev->PreviousActor = actor;

			prev = actor;
		}
		else if (actor->field_B2 == 0) {
			if (actor->field_8C < 0.0) {
				actor->field_B0 = 1;
				actor->field_B2 |= 1;
			}
			else {
				actor->OnDraw();
			}
		}

		actor = prev;
	}

	return 0;
}
