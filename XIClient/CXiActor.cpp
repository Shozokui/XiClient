#include "CXiActor.h"
#include "XiAtelBuff.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CXiDollActor.h"
#include "CYyScheduler.h"
#include "CYyResourceFile.h"
#include "CMoResourceMng.h"

using namespace FFXI::CYy;

CXiActor* CXiActor::top{ nullptr };
CMoAttachmentsSubStruct CXiActor::tempAttachStruct{};
CMoAttachmentsSubStruct CXiActor::TWOtempAttachStruct{};
int CXiActor::skeletonActorIndex{ 0 };
int CXiActor::config60{ 0 };
bool CXiActor::somebool{ false };
int CXiActor::maybeActorDrawCount{ 0 };
int CXiActor::emulate_game_status{ 0 };
int CXiActor::emulate_type{ 0 };
int CXiActor::emulate_race{ 0 };
short CXiActor::emulate_item[16] = { 0 };
KzCibCollect CXiActor::emulate_cib{};
float CXiActor::emulate_skeleton[18] = { 0.0 };

const CYyClass CXiActor::CXiActorClass {
	"CXiActor", sizeof(CXiActor), &CMoTask::CMoTaskClass
};

const CYyClass* FFXI::CYy::CXiActor::GetRuntimeClass()
{
	return &CXiActor::CXiActorClass;
}
int FFXI::CYy::CXiActor::GetActorsNum()
{
	CXiActor* actor = CXiActor::GetHead();
	int count = 0;
	while (actor) {
		++count;
		actor = actor->GetNext();
	}
	return count;
}
CXiActor* FFXI::CYy::CXiActor::GetHead()
{
	CXiActor* retval = CXiActor::top;
	if (retval == nullptr) return nullptr;

	while (retval->IsKindOf(&CXiDollActor::CXiDollActorClass)) {
		retval = retval->PreviousActor;
		if (retval == nullptr)
			return nullptr;
	}
	return retval;
}
void FFXI::CYy::CXiActor::InitActorStatics()
{
	CXiActor::top = nullptr;
	CXiActor::tempAttachStruct.DoSomething(nullptr);
	CXiActor::skeletonActorIndex = 0;
	CXiActor::maybeActorDrawCount = 0;
	CXiActor::config60 = 50;
}
char FFXI::CYy::CXiActor::OnMove()
{
	return 0;
}

FFXI::CYy::CXiActor::~CXiActor()
{
	//sub //TODO
}
FFXI::CYy::CXiActor::CXiActor()
{
	this->Init();
}

void FFXI::CYy::CXiActor::Init()
{
	D3DXVECTOR4* v2 = this->VirtActor101();
	D3DXVECTOR4* v3 = this->VirtActor102();

	*v2 = { 0.0, 0.0, 0.0, 1.0 };
	*v3 = { 0.0, 0.0, 0.0, 1.0 };

	this->VirtActor144(nullptr);

	//sub //TODO
	if (CYyDb::g_pCYyDb->field_9) {
		
	}
	else {

	}

	this->field_88 &= 0xFFFFF800;
	this->field_78 = 0x80808080;
	this->field_7C = 0x80808080;
	this->field_B2 = 0;
	this->field_80 = 0;
	this->field_B4 = 0;
	this->field_A0 = 0;
	this->field_A8 = 0;
	this->field_84 = 0;
	this->AtelBuffer = 0;
	this->field_C0 = 0;
	this->TargetAttachments = 0;
	this->CasterAttachments = 0;

	this->VirtActor97(5.0);
	this->VirtActor95(5.0);
	this->VirtActor99(1.6666666f);
	this->VirtActor154(nullptr);
	this->Link();
	this->SetSubActorStatus(0);
	//why
	if (CYyDb::g_pCYyDb->field_9 == 0)
		this->ActorsNum = CXiActor::GetActorsNum();

	this->ActorsNum = CXiActor::GetActorsNum();
}

void FFXI::CYy::CXiActor::Link()
{
	if ((this->field_88 & 2) == 0) {
		this->field_88 |= 2;

		if (CXiActor::top != nullptr) 
			CXiActor::top->NextActor = this;

		this->PreviousActor = CXiActor::top;
		this->NextActor = nullptr;
		CXiActor::top = this;

		this->ActorsNum = CXiActor::GetActorsNum();
	}
}

int FFXI::CYy::CXiActor::GetGameStatus()
{
	if (this->AtelBuffer)
		return this->AtelBuffer->GameStatus;

	return CXiActor::emulate_game_status;
}

bool FFXI::CYy::CXiActor::CheckGameStatus5(int a2)
{
	if (this->AtelBuffer == nullptr)
		return false;

	return this->AtelBuffer->CheckGameStatus5(a2);
}

bool FFXI::CYy::CXiActor::CheckGameStatus85(int a2)
{
	if (this->AtelBuffer == nullptr)
		return false;

	return this->AtelBuffer->CheckGameStatus85(a2);
}

bool FFXI::CYy::CXiActor::SomeEventCheck() {
	if (this->AtelBuffer == nullptr)
		return false;

	if (this->AtelBuffer->field_D4 == 0)
		return false;
	//sub //TODO
	exit(0x10082CD1);
}
int FFXI::CYy::CXiActor::GetType()
{
	if (this->AtelBuffer)
		return this->AtelBuffer->ActorType;

	return CXiActor::emulate_type;
}

int FFXI::CYy::CXiActor::GetRace()
{
	if (this->VirtActor227())
		return this->VirtActor229();

	if (this->AtelBuffer)
		return this->AtelBuffer->RaceGender;

	return CXiActor::emulate_race;
}

char FFXI::CYy::CXiActor::GetMoveMode()
{
	if (this->AtelBuffer == nullptr)
		return 0;

	return (this->AtelBuffer->field_124 >> 17) & 1;
}

bool FFXI::CYy::CXiActor::GetMonsterFlag()
{
	if (this->AtelBuffer)
		return (this->AtelBuffer->field_120 >> 13) & 1;

	return 1;
}

CXiActor* FFXI::CYy::CXiActor::GetNext()
{
	CXiActor* retval = this->PreviousActor;
	if (retval == nullptr) return nullptr;

	while (retval->IsKindOf(&CXiDollActor::CXiDollActorClass)) {
		retval = retval->PreviousActor;
		if (retval == nullptr)
			return nullptr;
	}
	return retval;

}

bool FFXI::CYy::CXiActor::InOwnActorPointers()
{
	if (this->AtelBuffer == nullptr) return false;

	int actorCount = sizeof(this->AtelBuffer->ActorPointers) / sizeof(this->AtelBuffer->ActorPointers[0]);

	for (int i = 0; i < actorCount; ++i) {
		if (this->AtelBuffer->ActorPointers[i] == this)
			return true;
	}
	return false;
}

bool FFXI::CYy::CXiActor::GetMotStop()
{
	if (this->AtelBuffer)
		return (this->AtelBuffer->field_12C >> 25) & 1;

	return false;
}

unsigned short FFXI::CYy::CXiActor::GetEquipNum(char a2)
{
	if (this->VirtActor227())
		return this->VirtActor231(a2);

	if (this->AtelBuffer)
		return this->AtelBuffer->Equipment[a2];

	return (a2 << 12) | CXiActor::emulate_item[a2];
}

bool FFXI::CYy::CXiActor::CheckSomeFlag1()
{
	if (this->AtelBuffer == nullptr)
		return CYyDb::g_pCYyDb->field_9 != 0;

	return this->AtelBuffer->field_128 & 0x08;
}

bool FFXI::CYy::CXiActor::CheckSomeFlag2()
{
	if (this->AtelBuffer == nullptr)
		return true;

	if ((this->AtelBuffer->field_188 & 0x08) != 0)
		return true;

	return this->AtelBuffer->field_128 & 0x10;
}

bool FFXI::CYy::CXiActor::CheckSomeFlag3()
{
	if (this->AtelBuffer == nullptr)
		return false;

	return (this->AtelBuffer->field_120 & 0x100000) != 0
		|| (this->AtelBuffer->field_120 & 0x200000) != 0
		|| (this->AtelBuffer->field_120 & 0x004000) != 0;
}

bool FFXI::CYy::CXiActor::CheckAtel120Bit5()
{
	if (this->AtelBuffer == nullptr)
		return false;

	return (this->AtelBuffer->field_120 >> 5) & 1;
}

bool FFXI::CYy::CXiActor::CheckAtel13CBit20()
{
	if (this->AtelBuffer == nullptr)
		return false;

	return (this->AtelBuffer->field_13C >> 20) & 1;
}

void FFXI::CYy::CXiActor::SetAtel13CBit20(bool a2)
{
	if (this->AtelBuffer == nullptr)
		return;

	unsigned int result = this->AtelBuffer->field_13C & 0xFFEFFFFF;
	this->AtelBuffer->field_13C = result ^ ((a2 & true) << 20);
}

bool FFXI::CYy::CXiActor::TestSomethingIfAtel(int a2)
{
	if (this->AtelBuffer == nullptr)
		return false;

	return this->AtelBuffer->TestSomething(a2);
}

CXiActor* FFXI::CYy::CXiActor::Get154Actor()
{
	if (this->AtelBuffer == nullptr)
		return nullptr;

	if (this->AtelBuffer->field_1F4 == 0)
		return nullptr;

	XiAtelBuff* atel = XiAtelBuff::AtelBuffers[this->AtelBuffer->field_1F4];
	if (atel == nullptr)
		return nullptr;

	if ((atel->field_120 & 0x200) == 0)
		return nullptr;

	return atel->Actor;
}

bool FFXI::CYy::CXiActor::DefSchedularCall()
{
	if (this->AtelBuffer == nullptr)
		return false;

	if ((this->AtelBuffer->field_120 & 0x200) == 0)
		return false;

	if (this->AtelBuffer->GameStatus == 34)
		return true;

	if (this->AtelBuffer->GetSomeFlag() != 3)
		return false;

	if ((this->AtelBuffer->field_128 & 0x20000000) != 0)
		this->AtelBuffer->Actor->SetAction('dcs@', this->AtelBuffer->Actor, nullptr);

	return true;
}

bool FFXI::CYy::CXiActor::StartScheduler(int a2, CXiActor* a3, void* a4)
{
	CYyScheduler** sched{ nullptr };
	if (a2 == '?ni!') {
		this->ActorFindResource((CMoResource***) &sched, FFXI::Constants::Enums::ResourceType::Scheduler, '1ni!');
		if (sched == nullptr) {
			this->ActorFindResource((CMoResource***)&sched, FFXI::Constants::Enums::ResourceType::Scheduler, '2ni!');
			if (sched == nullptr) {
				this->ActorFindResource((CMoResource***)&sched, FFXI::Constants::Enums::ResourceType::Scheduler, '3ni!');
			}
		}
	}
	else {
		this->ActorFindResource((CMoResource***)&sched, FFXI::Constants::Enums::ResourceType::Scheduler, a2);
	}

	if (sched != nullptr) {
		(*sched)->Execute(this, a3, 0, a4);
		return true;
	}

	if (a4 != nullptr) {
		//sub //TODO a4 is void*
		exit(0x100CCFD1);
		delete a4;
	}

	return false;
}

int FFXI::CYy::CXiActor::VirtActor6()
{
	return 128;
}

void FFXI::CYy::CXiActor::VirtActor11()
{
	//nullsub
}

float FFXI::CYy::CXiActor::GetWidthScale()
{
	return 1.0f;
}

float FFXI::CYy::CXiActor::GetHeightScale()
{
	return 1.0f;
}

float FFXI::CYy::CXiActor::GetDepthScale()
{
	return 1.0f;
}

void FFXI::CYy::CXiActor::VirtActor32(char)
{
	//nullsub
}

void FFXI::CYy::CXiActor::VirtActor34(char)
{
	//nullsub
}

void FFXI::CYy::CXiActor::VirtActor36(char)
{
	//nullsub
}

void FFXI::CYy::CXiActor::VirtActor38(char)
{
	//nullsub
}

void FFXI::CYy::CXiActor::VirtActor40(char)
{
	//nullsub
}

float FFXI::CYy::CXiActor::VirtActor58()
{
	if (this->AtelBuffer == nullptr)
		return -1.0;

	return this->AtelBuffer->field_200;
}

unsigned char FFXI::CYy::CXiActor::VirtActor67()
{
	if (this->AtelBuffer == nullptr)
		return 0;

	return this->AtelBuffer->field_1B9;
}

int FFXI::CYy::CXiActor::VirtActor69()
{
	if (this->AtelBuffer == nullptr)
		return 100;

	return this->AtelBuffer->field_DC;
}

bool FFXI::CYy::CXiActor::VirtActor73()
{
	if (this->AtelBuffer == nullptr)
		return false;

	return this->AtelBuffer->field_20C != 0;
}

bool FFXI::CYy::CXiActor::VirtActor75()
{
	return false;
}

void FFXI::CYy::CXiActor::VirtActor80()
{
	this->field_84 += 1;
}

void FFXI::CYy::CXiActor::VirtActor81()
{
	this->field_84 -= 1;
}

int FFXI::CYy::CXiActor::VirtActor88()
{
	return 0;
}

void FFXI::CYy::CXiActor::VirtActor89()
{
	//nullsub
}

void FFXI::CYy::CXiActor::VirtActor90()
{
	//nullsub
}

char FFXI::CYy::CXiActor::VirtActor92()
{
	return 0;
}

char FFXI::CYy::CXiActor::VirtActor93()
{
	return 0;
}

float FFXI::CYy::CXiActor::VirtActor94()
{
	return this->field_98;
}

void FFXI::CYy::CXiActor::VirtActor95(float a2)
{
	this->field_98 = a2;
}

float FFXI::CYy::CXiActor::VirtActor96()
{
	return this->field_94;
}

void FFXI::CYy::CXiActor::VirtActor97(float a2)
{
	this->field_94 = a2;
}

void FFXI::CYy::CXiActor::VirtActor99(float a2)
{
	this->field_9C = a2;
}


D3DXVECTOR4* FFXI::CYy::CXiActor::VirtActor101()
{
	return &this->field_34;
}

D3DXVECTOR4* FFXI::CYy::CXiActor::VirtActor102()
{
	return &this->field_44;
}

void FFXI::CYy::CXiActor::VirtActor103(int a2, D3DXVECTOR4* a3)
{
	*a3 = this->field_34;
}

void FFXI::CYy::CXiActor::GetElemLocal(unsigned int, D3DXVECTOR4* a2)
{
	*a2 = { 0.0, 0.0, 0.0, 1.0 };
}

void FFXI::CYy::CXiActor::VirtActor109(CYyResourceFile*** a2)
{
	*a2 = this->field_B4;
}

void FFXI::CYy::CXiActor::GetModelFile(CYyResourceFile*** a2)
{
	*a2 = this->field_B4;
}

void FFXI::CYy::CXiActor::ActorFindResource(CMoResource*** a2, int a3, int a4)
{
	CYyResourceFile** file{ nullptr };
	this->GetModelFile(&file);

	(*file)->AnotherResourceSearcher(a2, a3, a4);
}

void FFXI::CYy::CXiActor::SetMotionLock(bool)
{
	//nullsub
}

bool FFXI::CYy::CXiActor::IsMotionLock()
{
	return false;
}

D3DXVECTOR4* FFXI::CYy::CXiActor::VirtActor122()
{
	return &this->field_34;
}

int FFXI::CYy::CXiActor::VirtActor124()
{
	return 0;
}

void FFXI::CYy::CXiActor::VirtActor127(D3DLIGHT8*)
{
	//nullsub
}

int FFXI::CYy::CXiActor::VirtActor130()
{
	return this->field_A8;
}

void FFXI::CYy::CXiActor::VirtActor144(CXiActor* a2)
{
	this->field_5C.DoSomething(a2);
}

void FFXI::CYy::CXiActor::OnDraw()
{
	//nullsub
}

void FFXI::CYy::CXiActor::VirtActor154(CYyResourceFile** a2)
{
	this->field_B4 = a2;
}

bool FFXI::CYy::CXiActor::SetAction(int a2, CXiActor* a3, void* a4)
{
	CYyResourceFile** pfile{ nullptr };
	this->VirtActor109(&pfile);
	if (pfile == nullptr)
		return false;

	if (CMoResourceMng::CheckResourceThing((CMoResource***) &pfile) == false)
		return false;

	CYyResourceFile* file = *pfile;
	CYyScheduler** scheduler{ nullptr };
	CMoResourceMng::FindNextUnder((CMoResource***)&scheduler, file, FFXI::Constants::Enums::ResourceType::Scheduler, a2);
	if (scheduler == nullptr)
		return false;

	(*scheduler)->Execute(this, a3, 0, a4);
	return true;
}

bool FFXI::CYy::CXiActor::AmIControlActor()
{
	return false;
}

bool FFXI::CYy::CXiActor::IsDirectionLock()
{
	return false;
}

void FFXI::CYy::CXiActor::SetConstrain(char, int)
{
	//nullsub
}

short FFXI::CYy::CXiActor::IsConstrain()
{
	return 0;
}

int FFXI::CYy::CXiActor::VirtActor186()
{
	return 0;
}

void FFXI::CYy::CXiActor::VirtActor191(char, int)
{
	//nullsub
}

char FFXI::CYy::CXiActor::VirtActor192(int)
{
	return 0;
}

char FFXI::CYy::CXiActor::VirtActor194()
{
	return 1;
}

char FFXI::CYy::CXiActor::VirtActor198()
{
	return 0;
}

bool FFXI::CYy::CXiActor::VirtActor201()
{
	return this->SubActorStatus >= 1 && this->SubActorStatus <= 4;
}

void FFXI::CYy::CXiActor::SetSubActorStatus(int a2)
{
	this->SubActorStatus = a2;
}

bool FFXI::CYy::CXiActor::VirtActor203()
{
	return this->SubActorStatus == 3 || this->SubActorStatus == 4;
}

bool FFXI::CYy::CXiActor::VirtActor204()
{
	return this->SubActorStatus >= 84 && this->SubActorStatus <= 84;
}

bool FFXI::CYy::CXiActor::IsFishingRod()
{
	return this->SubActorStatus >= 5 && this->SubActorStatus <= 36;
}

bool FFXI::CYy::CXiActor::VirtActor209()
{
	return this->SubActorStatus >= 61 && this->SubActorStatus <= 81;
}

bool FFXI::CYy::CXiActor::VirtActor212()
{
	return this->SubActorStatus >= 37 && this->SubActorStatus <= 52;
}

bool FFXI::CYy::CXiActor::VirtActor214(int a2)
{
	if (a2 < 37 || a2 > 52)
		return false;

	return true;
}

bool FFXI::CYy::CXiActor::VirtActor215()
{
	return this->SubActorStatus >= 53 && this->SubActorStatus <= 60;
}

int FFXI::CYy::CXiActor::VirtActor217()
{
	return this->SubActorStatus - 53;
}

bool FFXI::CYy::CXiActor::VirtActor218(int a2)
{
	if (a2 < 53 || a2 > 60)
		return false;

	return true;
}

bool FFXI::CYy::CXiActor::VirtActor219()
{
	return this->SubActorStatus >= 82 && this->SubActorStatus <= 82;
}

int FFXI::CYy::CXiActor::VirtActor221()
{
	return this->SubActorStatus - 82;
}

bool FFXI::CYy::CXiActor::VirtActor222(int a2)
{
	if (a2 < 82 || a2 > 82)
		return false;

	return true;
}

bool FFXI::CYy::CXiActor::VirtActor223()
{
	return this->SubActorStatus >= 83 && this->SubActorStatus <= 83;
}

int FFXI::CYy::CXiActor::VirtActor225()
{
	return this->SubActorStatus - 83;
}

bool FFXI::CYy::CXiActor::VirtActor226(int a2)
{
	if (a2 < 83 || a2 > 83)
		return false;

	return true;
}

bool FFXI::CYy::CXiActor::VirtActor227()
{
	return false;
}

int FFXI::CYy::CXiActor::VirtActor229()
{
	return 0;
}

unsigned short FFXI::CYy::CXiActor::VirtActor231(char)
{
	return 0;
}

float* FFXI::CYy::CXiActor::VirtActor232(bool a2) {
	return CXiActor::emulate_skeleton;
}

KzCibCollect* FFXI::CYy::CXiActor::VirtActor236()
{
	return &CXiActor::emulate_cib;
}

short FFXI::CYy::CXiActor::VirtActor238()
{
	return 0;
}

void FFXI::CYy::CXiActor::VirtActor240(CMoAttachments* a2)
{
	if (a2 == nullptr)
		return;

	CMoAttachments* attach = this->CasterAttachments;
	CMoAttachments* prev{ nullptr };
	while (attach != nullptr) {
		if (attach == a2) {
			if (prev != nullptr) {
				prev->CastAttach = attach->CastAttach;
			}
			else {
				this->CasterAttachments = attach->CastAttach;
			}
			a2->CastAttach = nullptr;
			a2->ClearCaster(false);
			return;
		}
		prev = attach;
		attach = attach->CastAttach;
	}
}

void FFXI::CYy::CXiActor::VirtActor241(CMoAttachments* a2)
{
	if (a2 == nullptr)
		return;

	CMoAttachments* attach = this->TargetAttachments;
	CMoAttachments* prev{ nullptr };
	while (attach != nullptr) {
		if (attach == a2) {
			if (prev != nullptr) {
				prev->TargAttach = attach->TargAttach;
			}
			else {
				this->TargetAttachments = attach->TargAttach;
			}
			a2->TargAttach = nullptr;
			a2->ClearCaster(false);
			return;
		}
		prev = attach;
		attach = attach->TargAttach;
	}
}

char FFXI::CYy::CXiActor::VirtActor242()
{
	return 0;
}

void FFXI::CYy::CXiActor::VirtActor243(char)
{
	//nullsub
}
