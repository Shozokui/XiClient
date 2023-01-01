#pragma once
#include "CMoResource.h"
#include "CMoAttachments.h"
namespace FFXI {
	namespace CYy {
		class CMoCameraTask;
		class CXiActor;
		class CMoCamera : public CMoResource , public CMoAttachments {
		public:
			static const CYyClass CMoCameraClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			virtual void ConstructFromData(char*) override final;
			virtual void Open() override final;
			virtual void Close() override final;
			CMoCamera() = default;
			virtual ~CMoCamera() = default;
			virtual const CYyClass* AttachedTo() override final;
			CMoCameraTask* Execute(float, CXiActor*, CXiActor*);
			void SomeFunc(CMoCameraTask*);
			char Data[72];
		};
	}
}