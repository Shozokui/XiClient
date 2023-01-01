#pragma once
#define WIN32_LEAN_AND_MEAN
#include "RuntimeClass.h"
#include "CMoAttachmentsSubStruct.h"
#include "WMatrix.h"
namespace FFXI {
	namespace CYy {
		class CYyObject;
		class CXiActor;
		class CMoTask;
		class CMoAttachments {
		public:
			const static CYyClass CMoAttachmentsClass;
			static FFXI::Math::WMatrix AttachMatrix;
			virtual const CYyClass* AttachedTo();
			virtual ~CMoAttachments();
			virtual void VirtAttach1(CMoAttachments*);
			virtual void VirtAttach2(CMoAttachments*);
			virtual bool VirtAttach3();
			virtual bool VirtAttach4();
			virtual CYyObject* GetCYyObject();
			CMoAttachments();
			void Init();
			float MakeAttachMatrix(FFXI::Math::WMatrix*, D3DXVECTOR4*, D3DXVECTOR4*);
			void ClearCasterTarget();
			bool CheckFlags();
			CXiActor* GetMasterTarget();
			CXiActor* GetMasterCaster();
			CXiActor* GetCasterCheckDoll();
			CXiActor* GetTargetCheckDoll();
			void SetCaster(CXiActor*);
			void SetTarget(CXiActor*);
			void ClearCaster(bool);
			void ClearTarget(bool);
			void MixFlags(CMoAttachments*);
			unsigned int field_4;
			int field_8;
			CXiActor* Caster;
			CXiActor* Target;
			CMoAttachmentsSubStruct CasterSubStruct;
			CMoAttachmentsSubStruct TargetSubStruct;
			CMoAttachments* CastAttach;
			CMoAttachments* TargAttach;
			void* field_34;
			char field_38;
			char field_39;
			char field_3A;
			char field_3B;
			char field_3C;
			char field_3D;
			char field_3E;
			char field_3F;
		};
	}
}