#pragma once
#include "CMoTask.h"
#include "CMoAttachmentsSubStruct.h"
#include "d3dx9math.h"
#include "d3d8to9/d3d8types.hpp"
#include "KzCibCollect.h"
namespace FFXI {
	namespace CYy {
		class XiAtelBuff;
		class CMoResource;
		class CYyResourceFile;
		class CMoAttachments;
		class CXiActor : public CMoTask {
		public:
			static const CYyClass CXiActorClass;
			virtual const CYyClass* GetRuntimeClass() override;
			static CXiActor* top;
			static CMoAttachmentsSubStruct tempAttachStruct;
			static CMoAttachmentsSubStruct TWOtempAttachStruct;
			static int skeletonActorIndex;
			static int config60;
			static bool somebool;
			static int maybeActorDrawCount;
			static int emulate_game_status;
			static int emulate_type;
			static int emulate_race;
			static short emulate_item[16];
			static KzCibCollect emulate_cib;
			static float emulate_skeleton[18];
			static int GetActorsNum();
			static CXiActor* GetHead();
			static void InitActorStatics();
			virtual char OnMove() override;
			~CXiActor();
			CXiActor();
			void Init();
			void Link();
			int GetGameStatus();
			bool CheckGameStatus5(int);
			bool CheckGameStatus85(int);
			bool SomeEventCheck();
			int GetType();
			int GetRace();
			char GetMoveMode();
			bool GetMonsterFlag();
			CXiActor* GetNext();
			bool InOwnActorPointers();
			bool GetMotStop();
			unsigned short GetEquipNum(char);
			bool CheckSomeFlag1();
			bool CheckSomeFlag2();
			bool CheckSomeFlag3();
			bool CheckAtel120Bit5();
			bool CheckAtel13CBit20();
			void SetAtel13CBit20(bool);
			bool TestSomethingIfAtel(int);
			CXiActor* Get154Actor();
			bool DefSchedularCall();
			bool StartScheduler(int, CXiActor*, void*);
			D3DXVECTOR4 field_34;
			D3DXVECTOR4 field_44;
			CXiActor* PreviousActor;
			CXiActor* NextActor;
			CMoAttachmentsSubStruct field_5C;
			CMoAttachments* CasterAttachments;
			CMoAttachments* TargetAttachments;
			XiAtelBuff* AtelBuffer;
			int ActorsNum;
			int field_78;
			int field_7C;
			int field_80;
			int field_84;
			unsigned int field_88;
			float field_8C;
			int field_90;
			float field_94;
			float field_98;
			float field_9C;
			int field_A0;
			int SubActorStatus;
			int field_A8;
			int field_AC;
			char field_B0;
			char field_B1;
			short field_B2;
			CYyResourceFile** field_B4;
			int field_B8;
			float field_BC;
			char field_C0;
			char field_C1;
			char field_C2;
			char field_C3;
			virtual int VirtActor6();
			virtual void VirtActor11();
			virtual float GetWidthScale();
			virtual float GetHeightScale();
			virtual float GetDepthScale();
			virtual void VirtActor32(char);
			virtual void VirtActor34(char);
			virtual void VirtActor36(char);
			virtual void VirtActor38(char);
			virtual void VirtActor40(char);
			virtual float VirtActor58();
			virtual unsigned char VirtActor67() final;
			virtual int VirtActor69() final;
			virtual bool VirtActor73() final;
			virtual bool VirtActor75();
			virtual void VirtActor80() final;
			virtual void VirtActor81() final;
			virtual int VirtActor88();
			virtual void VirtActor89();
			virtual void VirtActor90();
			virtual char VirtActor92();
			virtual char VirtActor93();
			virtual float VirtActor94() final;
			virtual void VirtActor95(float) final;
			virtual float VirtActor96() final;
			virtual void VirtActor97(float) final;
			virtual void VirtActor99(float) final;
			virtual D3DXVECTOR4* VirtActor101();
			virtual D3DXVECTOR4* VirtActor102();
			virtual void VirtActor103(int, D3DXVECTOR4*);
			virtual void GetElemLocal(unsigned int, D3DXVECTOR4*); //VirtActor104
			virtual void VirtActor109(CYyResourceFile***) final;
			virtual void GetModelFile(CYyResourceFile***) final; //(VirtActor110)
			virtual void ActorFindResource(CMoResource***, int, int); //(VirtActor113)
			virtual void SetMotionLock(bool);
			virtual bool IsMotionLock();
			virtual D3DXVECTOR4* VirtActor122();
			virtual int VirtActor124();
			virtual void VirtActor127(D3DLIGHT8*);
			virtual int VirtActor130() final;
			virtual void VirtActor144(CXiActor*);
			virtual void OnDraw();
			virtual void VirtActor154(CYyResourceFile**);
			virtual bool SetAction(int, CXiActor*, void*); //(VirtActor156)
			virtual bool AmIControlActor(); //VirtActor183
			virtual int VirtActor186();
			virtual bool IsDirectionLock(); //188
			virtual void SetConstrain(char, int); //189
			virtual short IsConstrain(); //190
			virtual void VirtActor191(char, int);
			virtual char VirtActor192(int);
			virtual char VirtActor194();
			virtual char VirtActor198();
			virtual bool VirtActor201() final;
			virtual void SetSubActorStatus(int) final;
			virtual bool VirtActor203() final;
			virtual bool VirtActor204() final;
			virtual bool IsFishingRod() final;
			virtual bool VirtActor209() final;
			virtual bool VirtActor212() final;
			virtual bool VirtActor214(int) final;
			virtual bool VirtActor215() final;
			virtual int VirtActor217() final;
			virtual bool VirtActor218(int) final;
			virtual bool VirtActor219() final;
			virtual int VirtActor221() final;
			virtual bool VirtActor222(int) final;
			virtual bool VirtActor223() final;
			virtual int VirtActor225() final;
			virtual bool VirtActor226(int) final;
			virtual bool VirtActor227();
			virtual int VirtActor229();
			virtual unsigned short VirtActor231(char) final;
			virtual float* VirtActor232(bool);
			virtual KzCibCollect* VirtActor236();
			virtual short VirtActor238();
			virtual void VirtActor240(CMoAttachments*) final;
			virtual void VirtActor241(CMoAttachments*) final;
			virtual char VirtActor242();
			virtual void VirtActor243(char);
		};
	}
}