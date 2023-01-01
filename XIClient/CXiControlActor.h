#pragma once
#include "CXiAtelActor.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CXiControlActor : public CXiAtelActor {
		public:
			static const CYyClass CXiControlActorClass;
			static bool is_first_person_view;
			static bool is_auto_running;
			virtual const CYyClass* GetRuntimeClass() override;
			virtual char OnMove() override;
			CXiControlActor();
			virtual ~CXiControlActor() = default;
			void SetPos(D3DXVECTOR4*);
			void ApproachControl(D3DXVECTOR4*);
			void BackJumpControl(D3DXVECTOR4*);
			void ActorMoveControl(D3DXVECTOR4*);
			void BlowBackControl(D3DXVECTOR4*);
			void TryOnKeyDown(D3DXVECTOR4*);
			void OnKeyDown(D3DXVECTOR4*);
			void UserControl(D3DXVECTOR4*);
			void GetApproachPoint(D3DXVECTOR3*);
			D3DXVECTOR4 field_D4;
			D3DXVECTOR4 field_E4;
			int field_F4;
			char field_F8;
			char field_F9;
			char field_FA;
			char field_FB;
			int field_FC;
			char field_100;
			char field_101;
			char field_102;
			char field_103;
			char field_104;
			char field_105;
			char field_106;
			char field_107;
			float field_108;
			int field_10C;
			int field_110;
			int field_114;
			int field_118;
			int field_11C;
			D3DXVECTOR4 field_120;
			D3DXVECTOR4 field_130;
			D3DXVECTOR4 field_140;
			D3DXVECTOR4 field_150;
			D3DXVECTOR4 field_160;
			int field_170;
			int field_174;
			float field_178;
			char field_17C;
			char field_17D;
			char field_17E;
			char field_17F;
			D3DLIGHT8 field_180[5];
			D3DLIGHT8 field_388[5];
			int field_590;
			float field_594;
			int field_598;
			float field_59C;
			CMoAttachmentsSubStruct field_5A0;
			short field_5AC;
			short field_5AE;
			char field_5B0;
			char field_5B1;
			char field_5B2;
			char field_5B3;
			D3DXVECTOR4 field_5B4;
			virtual D3DXVECTOR4* VirtActor101() override;
			virtual D3DXVECTOR4* VirtActor102() override;
			virtual D3DXVECTOR4* VirtActor122() override final;
			virtual int VirtActor124() override final;
			virtual void VirtActor127(D3DLIGHT8*) override final;
			virtual void VirtActor144(CXiActor*) override final;
			virtual bool AmIControlActor() override final;
			virtual int VirtActor186() override final;
			virtual char VirtActor194() override final;
			virtual char VirtActor198() override final;
		};
	}
}