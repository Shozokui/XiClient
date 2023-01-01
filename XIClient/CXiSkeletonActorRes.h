#pragma once
#include "CMoTask.h"
namespace FFXI {
	namespace CYy {
		class CXiSkeletonActor;
		class CXiSkeletonActorRes : public CMoTask {
		public:
			CXiSkeletonActorRes(CXiSkeletonActor*, void(__cdecl*)(void));
			CXiSkeletonActor* Actor;
			void(__cdecl* Callback)(void);
			int field_3C;
			int field_40;
			int field_44;
		};
	}
}