#pragma once
#include "CMoTask.h"
#include "WMatrix.h"
#include "CMoAttachments.h"

namespace FFXI {
	namespace CYy {
		class CMoCamera;
		class CMoPathObject;
		class CXiActor;
		class CMoCameraTask : public CMoTask, public CMoAttachments {
		public:
			static const CYyClass CMoCameraTaskClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			virtual char OnMove() override final;
			CMoCameraTask(CMoCamera*, float, CXiActor*, CXiActor*);
			virtual ~CMoCameraTask();
			float GetT(float);
			CMoCamera* Camera;
			float field_78;
			float field_7C;
			FFXI::Math::WMatrix field_80;
			CMoPathObject* Path;
			D3DXVECTOR4* field_C4;
		};
	}
}