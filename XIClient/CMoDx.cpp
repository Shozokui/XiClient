#include "CMoDx.h"
#include "CYyDb.h"
#include "CMoResourceMng.h"
#include "Strings.h"
#include "CDx.h"
#include "CMoOt.h"
#include "Globals.h"
#include "CApp.h"
#include "InputMng.h"
#include "BaseProcessor.h"
#include "CYyResourceFile.h"
#include "CYyScheduler.h"
#include "CMoDxStatusMng.h"
#include "CMoOcclusionMng.h"
#include "SomeCMoObj.h"
#include "TexHelper.h"
#include "SomeVertexThing.h"
#include "SVTSStorage.h"
#include "CMoTaskMng.h"
#include "XiZone.h"
#include "OTStruct.h"
#include <timeapi.h>

using namespace FFXI::CYy;

TexHelper CMoDx::texhelper{};
CAcc* CMoDx::elem_cacc{ nullptr };
CAcc* CMoDx::th_cacc{ nullptr };

int CMoDx::ImageHeight{ 0 };
int CMoDx::ImageWidth{ 0 };

SomeVertexThing* CMoDx::svt1{ nullptr };
SomeVertexThing* CMoDx::svt2{ nullptr };
SomeVertexThing* CMoDx::svt3{ nullptr };
SomeVertexThing* CMoDx::svt4{ nullptr };
SomeVertexThing* CMoDx::svt5{ nullptr };
SVTSStorage* CMoDx::svtsstorage{ nullptr };
CMoOT* CMoDx::OT{ nullptr };
const CYyClass CMoDx::CMoDxClass{
	"CMoDx", sizeof(CMoDx), &CYyObject::CYyObjectClass
};

CMoDxStatusMng* CMoDx::g_pCMoDxStatusMng{ nullptr };
CMoOcclusionMng* CMoDx::g_pCMoOcclusionMng{ nullptr };
SomeCMoObj* CMoDx::g_pSomeCMoObj{ nullptr };
const CYyClass* FFXI::CYy::CMoDx::GetRuntimeClass()
{
	return &CMoDxClass;
}

void Init1() {
	CMoResourceMng::rng.Seed(0x1105);
	memcpy(CMoDx::texhelper.data, "cubemap spec    ", sizeof(CMoDx::texhelper.data));

	CMoDx::texhelper.Find(*FFXI::CYyDb::g_pCYyDb->pCMoResourceMng->Unknown3);

	char* mem = FFXI::StorageLists::instance->Get(sizeof(CAcc), FFXI::Constants::Enums::MEM_MODE::Ex);
	CMoDx::elem_cacc = new (mem) CAcc();
	
	CMoDx::elem_cacc->SomeTexture = FFXI::CYyDb::g_pCYyDb->g_pCMoDx->field_DC8->CreateTexture(256, 256, &CMoDx::elem_cacc->Texture2, D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT);
	CMoDx::elem_cacc->Surface = FFXI::CYyDb::g_pCYyDb->g_pCMoDx->field_DC8->CreateDepthStencilSurface(256, 256);

	CMoDx::svt1 = new SomeVertexThing();
	CMoDx::svt2 = new SomeVertexThing();
	CMoDx::svt3 = new SomeVertexThing();
	CMoDx::svt4 = new SomeVertexThing();
	CMoDx::svt5 = new SomeVertexThing();

	CMoDx::svt1->Init(4096, D3DFVF_DIFFUSE | D3DFVF_XYZ);
	CMoDx::svt2->Init(4096, D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
	CMoDx::svt3->Init(4096, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
	CMoDx::svt4->Init(4096, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ);
	CMoDx::svt5->Init(8192, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ);

	mem = FFXI::StorageLists::instance->Get(sizeof(SomeCMoObj), FFXI::Constants::Enums::MEM_MODE::Ex);
	if (mem)
		CMoDx::g_pSomeCMoObj = new (mem) SomeCMoObj();
}

void __cdecl OtCallback(CMoTask* a2) {
	CMoOT* ot = CMoDx::OT;

	XiZone::zone->GetFogByFourCC(&ot->FogColor, &ot->FogEnd, &ot->FogStart, 0, 0);
	ot->Material.Diffuse = { 1.0, 1.0, 1.0, 1.0 };
	ot->Material.Ambient = { 0.7f, 0.7f, 0.7f, 0.7f };
	ot->Material.Specular = { 0.0, 0.0, 0.0, 0.0 };
	ot->Material.Emissive = { 0.0, 0.0, 0.0, 0.0 };
	ot->Material.Power = 50.0;
	XiZone::zone->GetAmbientByFourCC(&ot->Ambient, 0, 0);
	D3DCOLOR color{ 0 };
	for (int shifter = 24; shifter >= 0; shifter -= 8) {
		unsigned char part = (ot->Ambient >> shifter) & 0xFF;
		if (part < 0x80u) {
			part *= 2;
		}
		else {
			part = 0xFF;
		}
		color |= part << shifter;
	}
	ot->Ambient = color;
	
	if (ot->field_4 != nullptr) {
		FFXI::OTStruct* v2 = ot->field_4 + ot->field_8;
		while (v2 != nullptr) {
			FFXI::OTStruct* prev = v2->previous;
			v2->Callback(v2->field_C);
			v2->field_14 = 0;
			v2 = prev;
		}
	}
	ot->InitOtStructs();
}

void Init3() {
	CMoDx* modx = FFXI::CYyDb::g_pCYyDb->g_pCMoDx;
	char* mem = FFXI::StorageLists::instance->Get(sizeof(CMoOT), FFXI::Constants::Enums::MEM_MODE::Ex);
	CMoDx::OT = new (mem) CMoOT();
	CMoDx::OT->Init(512, 0.0, 512.0);
	FFXI::CYyDb::g_pCYyDb->pCMoTaskMng->InitStackTask(OtCallback, 12);
}

void Init2() {
	CMoDx* modx = FFXI::CYyDb::g_pCYyDb->g_pCMoDx;

	CMoDx::th_cacc = nullptr;
	modx->field_EBC->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	modx->field_EBC->SetRenderState(D3DRS_ZWRITEENABLE, false);
	modx->field_EBC->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	modx->field_EBC->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	modx->field_EBC->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//sub //TODO global

	D3DSURFACE_DESC8 v13{};
	modx->field_DC8->BackbufferSurface->GetDesc(&v13);

	for (int i = 0; i < 3; ++i) {
		//sub //TODO
	}
}
FFXI::CYy::CMoDx::CMoDx(CDx* a2)
{
	UINT wPeriodMin{}, v4{};
	struct timecaps_tag ptc {};
	this->field_DC8 = a2;
	if (!timeGetDevCaps(&ptc, sizeof(ptc))) {
		wPeriodMin = ptc.wPeriodMin;
		v4 = ptc.wPeriodMin;
		if (ptc.wPeriodMin <= 1)
			v4 = 1;
		if (v4 >= ptc.wPeriodMax)
			wPeriodMin = ptc.wPeriodMax;
		else if (ptc.wPeriodMin <= 1)
			wPeriodMin = 1;
		timeBeginPeriod(wPeriodMin);
	}

	this->field_EA8 = timeGetTime();
	this->field_EB4 = 0;
	this->MatrixStackIndex = 0;
	this->field_EA0 = 0;
	this->field_EBC = this->field_DC8->DXDevice;
	this->field_EBC->GetDeviceCaps(&this->field_DCC);
	this->field_EC0[0] = 0;
	this->field_EC0[1] = 0;

	memset(this->field_10, 0, sizeof(this->field_10));
	memset(this->field_110, 0, sizeof(this->field_110));
	memset(this->field_210, 0, sizeof(this->field_210));
	memset(this->field_310, 0, sizeof(this->field_310));
}

FFXI::CYy::CMoDx::~CMoDx()
{
	for (int i = 0; i < sizeof(this->field_EC0) / sizeof(this->field_EC0[0]); ++i) {
		if (this->field_EC0[i])
			this->field_EBC->SetTexture(i, NULL);
	}
}

void FFXI::CYy::CMoDx::Init()
{
	char* mem = StorageLists::instance->Get(sizeof(CMoDx), Constants::Enums::MEM_MODE::Ex);
	if (mem)
		CYyDb::g_pCMoDx = new (mem) CMoDx(FFXI::CYy::CDx::instance);

	mem = StorageLists::instance->Get(sizeof(SVTSStorage), Constants::Enums::MEM_MODE::Ex);
	if (mem)
		CMoDx::svtsstorage = new (mem) SVTSStorage();

	CYyDb::pCMoResourceMng->InitUnk3(0);
	CYyScheduler** v3{ nullptr };
	(*CYyDb::pCMoResourceMng->Unknown3)->FindResourceUnder((CMoResource***) &v3, FFXI::Constants::Enums::ResourceType::Scheduler, 'corp');
	if (v3 && *v3)
		(*v3)->field_74 = 0x80;

	Init1();
	mem = StorageLists::instance->Get(sizeof(CMoDxStatusMng), Constants::Enums::MEM_MODE::Ex);
	if (mem)
	CYyDb::g_pCMoDx->g_pCMoDxStatusMng = new (mem) CMoDxStatusMng();
	Init2();
	Init3();
	mem = StorageLists::instance->Get(sizeof(CMoOcclusionMng), Constants::Enums::MEM_MODE::Ex);
	if (mem) {
		CYyDb::g_pCMoDx->g_pCMoOcclusionMng = new (mem) CMoOcclusionMng();
		CYyDb::g_pCMoDx->g_pCMoOcclusionMng->InitField8();
	}
	else {
		CYyDb::g_pCMoDx->g_pCMoOcclusionMng = nullptr;
	}

}

void FFXI::CYy::CMoDx::Update()
{
	int Time = timeGetTime();
	if (Time < this->field_EA8) {
		Time = 16;
		this->field_EA8 = 0;
	}
	int v4 = Time - this->field_EA8;
	this->field_EA8 = Time;
	double v3 = (double)v4 * 0.001;
	this->field_EAC = v3;
	this->field_EB0 = v3 * 60.0;
	this->field_EB4 += this->field_EB0;
	this->field_EB0 = CYyDb::g_pCYyDb->CheckTick();
}

void FFXI::CYy::CMoDx::CheckInput()
{
	for (int i = 0; i < 256; ++i) {
		char v4 = FFXI::CYy::CApp::g_pCApp->g_pInputMng->CheckThing(i);
		char v5 = this->field_10[i];
		this->field_10[i] = v4;
		this->field_110[i] = v4 & (v5 == 0);
		if (v4) {
			this->field_310[i] += this->field_EB0;
			if (this->field_310[i] < 20.0) {
				this->field_210[i] = this->field_110[i];
			}
			else {
				this->field_310[i] = 20.0;
				this->field_210[i] = 1;
			}
		} 
		else {
			this->field_310[i] = 0.0;
			this->field_210[i] = this->field_110[i];
		}
	}
}

void FFXI::CYy::CMoDx::DoSomething(int a2, int a3, int a4, int a5)
{
	this->field_DC8->GetTransform(D3DTS_VIEW, &this->field_B50);
	this->field_B90 = this->field_B50;
	this->field_B90._41 = 0.0;
	this->field_B90._42 = 0.0;
	this->field_B90._43 = 0.0;
	this->field_B90._44 = 1.0;

	this->field_DB4 = -this->field_B50._13;
	this->field_DB8 = -this->field_B50._23;
	this->field_DBC = -this->field_B50._33;

	this->field_D50 = this->field_B50;
	this->field_D50._11 = 1.0;
	this->field_D50._12 = 0.0;
	this->field_D50._13 = 0.0;
	this->field_D50._31 = 0.0;
	this->field_D50._32 = 0.0;
	this->field_D50._33 = 1.0;

	this->field_D10 = this->field_B50;
	this->field_D10._21 = 0.0;
	this->field_D10._22 = 1.0;
	this->field_D10._23 = 0.0;
	this->field_D10._12 = 0.0;
	this->field_D10._32 = 0.0;

	CYyDb::g_pCYyDb->pMoProcessor->MatrixMultiply(&this->field_C10, &this->field_D10, &this->field_D50);
	
	CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor40(&this->field_C50, &this->field_C10);
	CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor40(&this->field_BD0, &this->field_B50);
	CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor40(&this->field_C90, &this->field_D10);
	CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor40(&this->field_CD0, &this->field_D50);

	D3DVIEWPORT8* vp = this->field_DC8->GetTopViewport();
	this->field_DC0 = (double)vp->Width * 0.5;
	this->field_DC4 = (double)vp->Height * 0.5;

	this->field_DC8->GetTransform(D3DTS_PROJECTION, &this->field_B10);
}

void FFXI::CYy::CMoDx::PushViewTransform()
{
	if (this->MatrixStackIndex >= 8) return;

	this->GetViewTransform(this->MatrixStack + this->MatrixStackIndex);
	this->MatrixStackIndex += 1;
}

void FFXI::CYy::CMoDx::PopViewTransform()
{
	if (this->MatrixStackIndex <= 0) return;

	this->MatrixStackIndex -= 1;
	this->SetViewTransform(this->MatrixStack + this->MatrixStackIndex);
}

void FFXI::CYy::CMoDx::SetViewTransform(FFXI::Math::WMatrix* a2)
{
	this->field_DC8->SetTransform(D3DTS_VIEW, a2);
}

void FFXI::CYy::CMoDx::GetViewTransform(FFXI::Math::WMatrix* a2)
{
	this->field_DC8->GetTransform(D3DTS_VIEW, a2);
}

void FFXI::CYy::CMoDx::SetWorldTransform(FFXI::Math::WMatrix* a2)
{
	this->field_DC8->SetTransform(D3DTS_WORLD, a2);
}

void FFXI::CYy::CMoDx::ClearRewind(int a2, int a3)
{
	this->field_EBC->SetTexture(0, nullptr);
	this->field_EBC->SetTexture(1, nullptr);
	this->field_EBC->SetTexture(2, nullptr);
	this->field_EBC->SetTexture(3, nullptr);
	this->field_DC8->RewindStage();
}
