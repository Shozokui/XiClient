#include "CYyDb.h"
#include "CommandLineConfig.h"
#include "FFXIDX.h"
#include "CXiActor.h"
#include "IwManager.h"
#include "TaskBuff.h"
#include "CDx.h"
#include "CMoDx.h"
#include "CEnv.h"
#include "CMoTaskMng.h"
#include "FsConfig.h"
#include "SomeVertexThing.h"
#include "BaseProcessor.h"
#include "XiZone.h"
#include "DMsg.h"
#include "XIString.h"
#include "StringTables.h"
#include "CApp.h"
#include "CMoResourceMng.h"
#include "StorageListObject.h"
#include "TkManager.h"
#include "CMoResource.h"
#include "CYySoundElem.h"
#include "CTkObject.h"
#include "CXiOpening.h"
#include "CXITimerLow.h"
#include "CYyResourceFile.h"
#include "SomeCMoObj.h"
#include "CMoDx.h"
#include "CYyVbMng.h"
#include "FsMenuDraw.h"
#include "CTkMouse.h"
#include "CYyTexMng.h"
#include "SomeTaskType.h"
#include "CTkMsgWinData.h"
#include "NT_SYS.h"
#include "UnknownClass.h"
#include "DatIndices.h"
#include "RegistryConfig.h"
#include "Globals.h"
#include "d3dx9math.h"
#include <stdlib.h>
#include "CYyCamMng2.h"
#include <iostream>
#include "TextRenderer.h"
#include "MojiDraw.h"
#include "InputMng.h"
#include "CTsZoneMap.h"
#include "XiDateTime.h"
#include "YkManager.h"
#include "CXiActorDraw.h"
#include "CMoTextureDisplayTask.h"
#include "RuntimeConfig.h"

using namespace FFXI;
using namespace FFXI::CTk;

CYyDb* CYyDb::g_pCYyDb{ nullptr };
IDirect3DTexture8* CYyDb::SomeTexture1 { nullptr };
IDirect3DTexture8* CYyDb::SomeTexture2 { nullptr };
IDirect3DTexture8* CYyDb::SomeTexturesKindaLikeCApp2[2] = { nullptr };
IDirect3DTexture8* CYyDb::TextureDisplayTaskArr[4] = { nullptr };
int CYyDb::TextureDisplayState{ 0 };
int CYyDb::TextureDisplaySubState[2] = { 0 };
Config::FsConfig* CYyDb::g_pFsConfig{ nullptr };
CYy::CMoTaskMng* CYyDb::pCMoTaskMng{ nullptr };
CYy::CMoResourceMng* CYyDb::pCMoResourceMng{ nullptr };
CYy::SomeTaskType* CYyDb::pSomeTaskType{ nullptr };
CYy::CYyTexMng* CYyDb::pCYyTexMng{ nullptr };
CYy::CYyVbMng* CYyDb::pCYyVbMng{ nullptr };
CYy::CYyVb* CYyDb::pCYyVb{ nullptr };
CYy::BaseProcessor* CYyDb::pMoProcessor{ nullptr };

CYy::CYyResourceFile** CYyDb::ResFile_MENUMissionQuest{ nullptr };
CYy::CYyResourceFile** CYyDb::ResFile_MENUUnk1{ nullptr };
CYy::CYyResourceFile** CYyDb::ResFile_TEXGeneral{ nullptr };
CYy::CYyResourceFile** CYyDb::ResFile_TEXIcons1{ nullptr };
CYy::CYyResourceFile** CYyDb::ResFile_TEXIcons2{ nullptr };
CYy::CMoResource** CYyDb::Res_Unk1{ nullptr };
void* CYyDb::SomeStatic{ nullptr };
CTkMsgWinData* CYyDb::MsgWinData1{ nullptr };
CTkMsgWinData* CYyDb::MsgWinData2{ nullptr };
UnknownClass* CYyDb::UnknownClass{ nullptr };
CYy::CTsZoneMap* CYyDb::g_pTsZoneMap{ nullptr };
CYy::CXiActorDraw* CYyDb::pCXiActorDraw{ nullptr };
int CYyDb::SomeState{ 0 };
int CYyDb::SomeState2{ 0 };
int CYyDb::SomeState3{ 0 };
int CYyDb::SomeState4{ 0 };
unsigned char CYyDb::SomeByte{ 0x80 };
int CYyDb::SomeFlag{ 0 };
float CYyDb::StaticFloat{ 0 };
float CYyDb::FloatArray[4] = { 0,0,0,0 };
int CYyDb::FloatArrayIndex{ 0 };
WORD CYyDb::TimerSeconds{ 0xFFFF };
CYy::CXiOpening* CYyDb::XiOpening{ nullptr };
bool CYyDb::PatchingComplete{ false };
CYy::CYyResourceFile** CYyDb::RES_1{ nullptr };
FsMenuDraw* CYyDb::g_pFsMenuDraw{ nullptr };
int CYyDb::PhaseInitCountdown{ 0 };
CYy::CMoDx* CYyDb::g_pCMoDx{ nullptr };

FFXI::CYy::FVF44Vertex some_verts[4] = {
	{0.0, 0.0, 0.0, 1.0, 0},
	{0.0, 0.0, 0.0, 1.0, 0},
	{0.0, 0.0, 0.0, 1.0, 0},
	{0.0, 0.0, 0.0, 1.0, 0}
};

//LOCAL FUNCS
void Present() {
	FFXIDX* v0 = FFXI::FFXIDX::g_pFFXIDX;
	int v1 = 100;
	do {
		--v1;
		HRESULT v2 = v0->DXDevice->Present(NULL, NULL, NULL, NULL);
		if (v2 == D3D_OK)
			break;
		if (v2 == D3DERR_DEVICELOST)
		{
			//!!!! DEVICE LOST !!!!
		}
	} while (v1);
}

void RenderTextureTask() {
	if (CYyDb::TextureDisplayTaskArr[0] == nullptr)
		return;

	if (CYy::CMoTextureDisplayTask::progress <= 0.0)
		return;

	if (CYyDb::TextureDisplayState == 1)
	{
		CYyDb::TextureDisplaySubState[0] = 1;
		if (Globals::g_pCenv->CheckField18() == true)
			CYyDb::TextureDisplaySubState[1] = 1;
	}
	else if (CYyDb::TextureDisplayState == 2)
	{
		CYyDb::TextureDisplaySubState[0] = 2;
		if (CYyDb::TextureDisplaySubState[1] == 1)
			CYyDb::TextureDisplaySubState[1] = 2;
	}
	
	CYy::CDx* cdx = FFXI::CYy::CDx::instance;
	for (int i = 0; i < sizeof(CYyDb::TextureDisplaySubState) / sizeof(CYyDb::TextureDisplaySubState[0]); ++i) {
		FFXI::CYy::CDx::instance->SetRenderStates();
		cdx->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		cdx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		cdx->DXDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		cdx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
		cdx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
		cdx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		cdx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		cdx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

		if (CYyDb::TextureDisplaySubState[i] == 1
			|| CYyDb::TextureDisplaySubState[i] == 2) {
			if (CYyDb::TextureDisplaySubState[i] == 1) {
				if (CYyDb::TextureDisplayTaskArr[i] != nullptr
					&& cdx->CAcc2.Texture[i] != nullptr) {
					cdx->AddViewportAtOrigin(CYyDb::TextureDisplayTaskArr[0], CYyDb::TextureDisplayTaskArr[2 + i], nullptr);
					cdx->DoViewportRender(cdx->CAcc2.Texture[i], 0x80808080, -1);
					cdx->RevertStage();
				}
				CYyDb::TextureDisplayState = 2;
			}

			if (CYyDb::TextureDisplayTaskArr[i] != nullptr
				&& cdx->CAcc2.Texture[i] != nullptr) {
				cdx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
				cdx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				cdx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				double dalpha = 255.0 * (double)CYy::CMoTextureDisplayTask::progress;
				long long llalpha = dalpha;
				int alpha = llalpha;
				D3DCOLOR color = alpha << 24;
				color |= 0x808080;
				if (i == 0) {
					cdx->DoViewportRender(CYyDb::TextureDisplayTaskArr[0], color, -1);
				}
				else {
					cdx->AddViewportAtOrigin(cdx->CAcc2.Texture[i], cdx->CAcc2.Texture[2 + i], nullptr);
					cdx->DoViewportRender(CYyDb::TextureDisplayTaskArr[i], color, -1);
					cdx->RevertStage();
				}
				cdx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			}
		}
	}

	cdx->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	cdx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	cdx->DXDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
}

void CreateSomeTexture() {
	CYy::CDx* CDx = FFXI::CYy::CDx::instance;
	unsigned short v3 = CYyDb::GetBackgroundYRes();
	unsigned short v0 = CYyDb::GetBackgroundXRes();
	Globals::g_pSomeTexture1 = CDx->CreateTexture(v0, v3, &Globals::g_pSomeTexture2, 1, D3DPOOL_DEFAULT);
	if (Globals::g_pCenv->field_18) {
		Globals::g_pSomeTexture3 = CDx->CreateTexture(v0, v3, &Globals::g_pSomeTexture4, 1, D3DPOOL_DEFAULT);
	}
	else {
		Globals::g_pSomeTexture3 = nullptr;
		Globals::g_pSomeTexture4 = nullptr;
	}
}
void CreateVertexBuffers() {
	HRESULT result{};
	CYy::CDx* CDx = FFXI::CYy::CDx::instance;
	if (Globals::g_pCenv->field_A)
		Globals::g_VertexBufferD3DPool = D3DPOOL_SYSTEMMEM;
	D3DXMatrixIdentity(&Globals::g_D3DTransformMatrix);
	Globals::g_pVertexBuffers[0] = nullptr;
	Globals::g_pVertexBuffers[1] = nullptr;
	Globals::g_pVertexBuffers[2] = nullptr;
	Globals::g_pVertexBuffers[3] = nullptr;
	if (!Globals::g_pVertexBuffersInitialized) {
		if (CDx->DXDevice->CreateVertexBuffer(
			0x7000, 
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ,
			Globals::g_VertexBufferD3DPool,
			&Globals::g_pVertexBuffers[0]) < D3D_OK)
			return;

		if (CDx->DXDevice->CreateVertexBuffer(
			0x9000,
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ,
			Globals::g_VertexBufferD3DPool,
			&Globals::g_pVertexBuffers[1]) < D3D_OK)
			return;

		if (CDx->DXDevice->CreateVertexBuffer(
			0x8000,
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFVF_TEX1 | D3DFVF_NORMAL | D3DFVF_XYZ,
			Globals::g_VertexBufferD3DPool,
			&Globals::g_pVertexBuffers[2]) < D3D_OK)
			return;
		
		if (CDx->DXDevice->CreateVertexBuffer(
			0x4000,
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFVF_TEX1 | D3DFVF_NORMAL | D3DFVF_XYZ,
			Globals::g_VertexBufferD3DPool,
			&Globals::g_pVertexBuffers[3]) < D3D_OK)
			return;

		Globals::g_pVertexBuffersInitialized = 1;
	}
	CreateSomeTexture();
	//-----
	Globals::g_pSomeTexture5 = nullptr;
	Globals::g_pSomeTexture6 = Globals::g_pSomeTexture2;
	if (Globals::g_pSomeTexture2)
		Globals::g_pSomeTexture2->AddRef();
	//-----
	Globals::g_pSomeTexture7 = nullptr;
	Globals::g_pSomeTexture8 = CDx->CreateTexture(0x100, 0x100, &Globals::g_pSomeTexture7, 1, D3DPOOL_DEFAULT);
	if (Globals::g_pCenv->field_18) {
		Globals::g_pSomeTexture9 = Globals::g_pSomeTexture4;
		if (Globals::g_pSomeTexture4)
			Globals::g_pSomeTexture4->AddRef();
		Globals::g_pSomeTexture10 = CDx->CreateTexture(0x100, 0x100, &Globals::g_pSomeTexture9, 1, D3DPOOL_DEFAULT);
	}
	else {
		Globals::g_pSomeTexture10 = nullptr;
		Globals::g_pSomeTexture9 = nullptr;
	}
	//-----
	Globals::g_pSomeTexture11 = nullptr;
	Globals::g_pSomeTexture12 = Globals::g_pSomeTexture2;
	if (Globals::g_pSomeTexture2)
		Globals::g_pSomeTexture2->AddRef();
}

void CleanDXMembers() {
	for (int i = 0; i < sizeof(Globals::g_pVertexBuffers) / sizeof(Globals::g_pVertexBuffers[0]); ++i)
	{
		IDirect3DVertexBuffer8* buffer = Globals::g_pVertexBuffers[i];
		if (buffer) {
			buffer->Release();
			buffer = nullptr;
		}
	}
	Globals::g_pVertexBuffersInitialized = 0;
	//-----------------
	if (Globals::g_pSomeTexture8) {
		Globals::g_pSomeTexture8->Release();
		Globals::g_pSomeTexture8 = nullptr;
	}
	if (Globals::g_pSomeTexture10) {
		Globals::g_pSomeTexture10->Release();
		Globals::g_pSomeTexture10 = nullptr;
	}
	if (Globals::g_pSomeTexture7) {
		Globals::g_pSomeTexture7->Release();
		Globals::g_pSomeTexture7 = nullptr;
	}
	if (Globals::g_pSomeTexture9) {
		Globals::g_pSomeTexture9->Release();
		Globals::g_pSomeTexture9 = nullptr;
	}
	//-----------------
	if (Globals::g_pSomeTexture11) {
		Globals::g_pSomeTexture11->Release();
		Globals::g_pSomeTexture11 = nullptr;
	}
	if (Globals::g_pSomeTexture12) {
		Globals::g_pSomeTexture12->Release();
		Globals::g_pSomeTexture12 = nullptr;
	}
	//-----------------
	if (Globals::g_pSomeTexture5) {
		Globals::g_pSomeTexture5->Release();
		Globals::g_pSomeTexture5 = nullptr;
	}
	if (Globals::g_pSomeTexture6) {
		Globals::g_pSomeTexture6->Release();
		Globals::g_pSomeTexture6 = nullptr;
	}
	//------------------
	if (Globals::g_pSomeTexture1) {
		Globals::g_pSomeTexture1->Release();
		Globals::g_pSomeTexture1 = nullptr;
	}
	if (Globals::g_pSomeTexture2) {
		Globals::g_pSomeTexture2->Release();
		Globals::g_pSomeTexture2 = nullptr;
	}
	if (Globals::g_pSomeTexture3) {
		Globals::g_pSomeTexture3->Release();
		Globals::g_pSomeTexture3 = nullptr;
	}
	if (Globals::g_pSomeTexture4) {
		Globals::g_pSomeTexture4->Release();
		Globals::g_pSomeTexture4 = nullptr;
	}
}

void DoSomething() {
	CYy::CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;
	if (cammng) {
		cammng->Update();
		if (cammng->Check() || cammng->field_F4 != 0) {
			cammng->field_F4 = 0;
			FFXI::Math::WMatrix* v3 = cammng->GetView();
			FFXI::CYy::CDx::instance->SetTransform(D3DTS_VIEW, v3);
			FFXI::Math::WMatrix v6 = *v3;
			CYyDb::g_pCYyDb->SetView(&v6);
		}
	}
	if (CYyDb::g_pCYyDb)
		CYyDb::g_pCYyDb->field_2F4 = CYyDb::g_pCYyDb->field_2F8;
	CYyDb::g_pCYyDb->SomeCalc1();
	FFXI::CYy::CDx::instance->SetTransform(D3DTS_PROJECTION, &CYyDb::g_pCYyDb->field_154);
	
	if (!CYyDb::g_pCYyDb->g_pCMoDx) return;
	if (!CYy::CMoDx::g_pSomeCMoObj) return;

	CYyDb::g_pCYyDb->g_pCMoDx->Update();
	if (CYy::CMoDx::svt1 != nullptr) {
		CYy::CMoDx::svt1->Toggle45();
	}
	if (CYy::CMoDx::svt2 != nullptr) {
		CYy::CMoDx::svt2->Toggle45();
	}
	if (CYy::CMoDx::svt3 != nullptr) {
		CYy::CMoDx::svt3->Toggle45();
	}
	if (CYy::CMoDx::svt4 != nullptr) {
		CYy::CMoDx::svt4->Toggle45();
	}
	if (CYy::CMoDx::svt5 != nullptr) {
		CYy::CMoDx::svt5->Toggle45();
	}
	CYy::CMoDx::g_pSomeCMoObj->Update();
	CYyDb::g_pCYyDb->g_pCMoDx->DoSomething(0, 0, 0, 0);
}
//~LOCAL FUNCS

void FFXI::CYyDb::FsGameLobbyStart()
{
	//sub //TODO //msgwin
	this->g_pFsConfig->initConfig();
	//sub //TODO
	//pol config?
	this->g_pFsConfig->loadTIGConfig();
	this->g_pFsConfig->field_A = 1;
}

void FFXI::CYyDb::HandleState()
{
	if (this->MainState != this->field_40) {
		this->field_40 = this->MainState;
		this->InitSubState(0);
		this->field_48 = 0;
	}
	int v13;
	switch (this->MainState) {
	case 0:
		if (FFXI::Config::RegistryConfig::g_pOptions->Other.field_5C) {
			int index = Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::TEX_1);
			CYy::CYyResourceFile** Unused{ nullptr };
			CYyDb::RES_1 = *this->pCMoResourceMng->LoadNumFile(&Unused, index);
			CYy::CMoResource::DoTheThing(*CYyDb::RES_1);
		}
		if (FFXI::Config::RegistryConfig::g_pOptions->Other.field_24) {
			CYyDb::SomeFlag = 0;
			v13 = CYy::CApp::GetAppState();
			if (v13 == 0) {
				this->MainState = 1;
			}
			else if (v13 == 1) {
				CYy::CApp::SetAppState(0);
				this->MainState = 8;
				CYyDb::g_pCYyDb->field_9 = 1;
				//sub //TODO
				//POL STUFF HERE
			}
			else if (v13 == 2) {
				CYyDb::PatchingComplete = 0;
				CYyDb::SomeFlag = 1;
				//sub //TODO
				CYy::CApp::SetAppState(0);
				CYyDb::g_pCYyDb->field_9 = 1;
				this->field_334 = 0x80000000;
				this->MainState = 2;
				//sub //TODO
				//POL STUFF HERE
			}
		}
		else {
			this->field_9 = 0;
			this->MainState = 10;
			CYy::CApp::g_pNT_SYS->ntTimeInit();
		}
		break;
	case 1:
		if (Config::MainRegistryConfig::PlayOpeningMovie == 0 || this->OpeningMovieIsFinished())
			this->MainState = 2;
		break;
	case 2:
	{
		char v113[0x200];
		sprintf_s(v113, sizeof(v113), "-net 3 -port %d\0", Constants::Values::PORT_LOBBY);
		FFXI::Config::CommandLineConfig::instance.AppendLaunchArgs(v113);
		FFXI::Config::RegistryConfig::g_pOptions->Other.field_24 = 3;
		this->MainState = 3;
		this->SubState = 0;
		if (FFXI::Config::RegistryConfig::g_pOptions->Other.field_24) {
			if (FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->Init()) {
				CYyDb::g_pCYyDb->field_9 = 1;
				//sub //TODO
				//POL stuff here
			}
			else {
				FFXI::PolVars::instance.g_polErrorCode = 5;
				this->field_A = 0;
			}
		}
	}
	break;
	case 3:
		if (FFXI::Config::RegistryConfig::g_pOptions->Other.field_20)
			FFXI::GlobalStruct::g_GlobalStruct.field_0 = 32;
		this->MainState = 4;
		break;
	case 4:
		//sub //TODO dancer flag, ffxi.dll checksum
		this->MainState = 5;
		break;
	case 5:
		FFXI::GlobalStruct::g_GlobalStruct.field_0 = 64;
		if (PhaseInit()) {
			if (this->field_48) {
				--this->field_48;
				if (this->field_48 == 0)
					this->MainState = 13;
			}
			else {
				FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->field_10 = 1;
				IwManager::SomeByte1 = 0;
				//sub //todo XiErr
				//FFXID stuff
				this->MainState = 6;
			}
		}
		break;
	case 6:
		FFXI::GlobalStruct::g_GlobalStruct.field_0 = 0x70;
		if (true) // Skip network check
			this->MainState = 10;
		break;
	case 10:
		//sub
		break;
	case 13:
		//sub //TODO //cleanup
		FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->field_10 = 0;
		FFXI::GlobalStruct::g_GlobalStruct.field_0 = 0xA0;
		this->CountdownTillClose();
		break;
	case 17:
		FFXI::CYy::CDx::instance->SomeFunc(0.0, 0.0, 1);
		FFXI::CYy::CDx::instance->SetSomeColor(0x80000000);
		this->field_338 = 0x80000000;
		this->field_334 = 0x80808080;
		if (true) {//sub //TODO
			if (CYyDb::SomeState != 12) {
				this->MainState = 13;
				this->HandleState();
			}
			else {
				this->MainState = 11; 
			}
		}
		break;
	default:
		break;
	}
}

void FFXI::CYyDb::OnMove()
{
	;
	//subs //TODO
	CYy::CDx::instance->field_828 = 1;
	CYy::XiZone::SysInit();
	XiDateTime::SysMove();
	//StDancer::Update();
	CYy::XiZone::zone->SysMove();
	CYyDb::pCMoTaskMng->DoTasks();
	//dancer
	CYy::CYySoundElem::SysMove();
}

void FFXI::CYyDb::CheckMoveHandle()
{
	if (this->MainState >= 5 && GlobalStruct::g_GlobalStruct.field_0 != 64 && IwManager::polircstts < 0)
		exit(2001);
	this->OnMove();
	this->HandleState();
}

CYyDb::~CYyDb() {

	if (CYyDb::XiOpening) {
		delete CYyDb::XiOpening;
		CYyDb::XiOpening = nullptr;
	}

	FFXI::CYy::CXiOpening::CleanXiMovie();

	if (IwManager::task_buff) {
		delete IwManager::task_buff;
		IwManager::task_buff = nullptr;
	}

	
	//sub TODO
	TkManager::g_CTkMenuMng.DeleteMenuAll();
	IwManager::IwDestroy();

	//sub //TODO
	
	Yk::YkManager::YkEnd();
	TkManager::TkEnd();
	//sub //TODO
	if (CYyDb::g_pFsMenuDraw) {
		delete CYyDb::g_pFsMenuDraw;
		CYyDb::g_pFsMenuDraw = nullptr;
	}

	if (FFXI::Text::TextRenderer::g_MojiDraw) {
		delete FFXI::Text::TextRenderer::g_MojiDraw;
		FFXI::Text::TextRenderer::g_MojiDraw = nullptr;
	}
	//sub TODO
	Config::FsConfig::FsConfigUninit();
	if (this->Timer1) {
		delete this->Timer1;
		this->Timer1 = nullptr;
	}

	if (this->Timer2) {
		delete this->Timer2;
		this->Timer2 = nullptr;
	}

	if (CYyDb::pCXiActorDraw != nullptr) {
		delete CYyDb::pCXiActorDraw;
		CYyDb::pCXiActorDraw = nullptr;
	}

	if (CYyDb::Res_Unk1) {
		CYy::CMoResource* res = *CYyDb::Res_Unk1;
		if (res) {
			res->SomeClean3();
			CYy::CMoResource::UnlinkFromManager(&res->PreHeader.PointerToThisInRPL);
		}
		delete[] CYyDb::Res_Unk1;
		CYyDb::Res_Unk1 = nullptr;
	}

	if (CYyDb::MsgWinData1) {
		delete[] CYyDb::MsgWinData1;
		CYyDb::MsgWinData1 = nullptr;
	}

	if (CYyDb::MsgWinData2) {
		delete[] CYyDb::MsgWinData2;
		CYyDb::MsgWinData2 = nullptr;
	}

	if (CYyDb::UnknownClass) {
		delete[] CYyDb::UnknownClass;
		CYyDb::UnknownClass = nullptr;
	}

	if (CYyDb::ResFile_MENUUnk1) {
		(*CYyDb::ResFile_MENUUnk1)->SomeClean3();
		CYy::CMoResource::UnlinkFromManager((CYy::CMoResource***)&CYyDb::ResFile_MENUUnk1);
		CYyDb::ResFile_MENUUnk1 = nullptr;
	}

	if (CYyDb::ResFile_MENUMissionQuest) {
		(*CYyDb::ResFile_MENUMissionQuest)->SomeClean3();
		CYy::CMoResource::UnlinkFromManager((CYy::CMoResource***)&CYyDb::ResFile_MENUMissionQuest);
		CYyDb::ResFile_MENUMissionQuest = nullptr;
	}

	if (CYyDb::ResFile_TEXGeneral) {
		(*CYyDb::ResFile_TEXGeneral)->SomeClean3();
		CYy::CMoResource::UnlinkFromManager((CYy::CMoResource***)&CYyDb::ResFile_TEXGeneral);
		CYyDb::ResFile_TEXGeneral = nullptr;
	}

	if (CYyDb::ResFile_TEXIcons1) {
		(*CYyDb::ResFile_TEXIcons1)->SomeClean3();
		CYy::CMoResource::UnlinkFromManager((CYy::CMoResource***)&CYyDb::ResFile_TEXIcons1);
		CYyDb::ResFile_TEXIcons1 = nullptr;
	}

	if (CYyDb::ResFile_TEXIcons2) {
		(*CYyDb::ResFile_TEXIcons2)->SomeClean3();
		CYy::CMoResource::UnlinkFromManager((CYy::CMoResource***)&CYyDb::ResFile_TEXIcons2);
		CYyDb::ResFile_TEXIcons2 = nullptr;
	}

	if (CYyDb::RES_1) {
		(*CYyDb::RES_1)->SomeClean3();
		CYy::CMoResource::UnlinkFromManager((CYy::CMoResource***)&CYyDb::RES_1);
		CYyDb::RES_1 = nullptr;
	}

	if (CYyDb::pCMoResourceMng) {
		delete CYyDb::pCMoResourceMng;
		CYyDb::pCMoResourceMng = nullptr;
	}

	if (CYyDb::pCMoTaskMng) {
		delete CYyDb::pCMoTaskMng;
		CYyDb::pCMoTaskMng = nullptr;
	}

	if (CYyDb::pCYyTexMng) {
		delete CYyDb::pCYyTexMng;
		CYyDb::pCYyTexMng = nullptr;
	}

	if (CYyDb::pCYyVbMng) {
		delete CYyDb::pCYyVbMng;
		CYyDb::pCYyVbMng = nullptr;
	}

	CYy::BaseProcessor::DestroyProcessor();

	if (SomeTexture1) {
		SomeTexture1->Release();
		SomeTexture1 = nullptr;
	}

	if (SomeTexture2) {
		SomeTexture2->Release();
		SomeTexture2 = nullptr;
	}

	if (SomeTexturesKindaLikeCApp2[0]) {
		SomeTexturesKindaLikeCApp2[0]->Release();
		SomeTexturesKindaLikeCApp2[0] = nullptr;
	}

	if (SomeTexturesKindaLikeCApp2[1]) {
		SomeTexturesKindaLikeCApp2[1]->Release();
		SomeTexturesKindaLikeCApp2[1] = nullptr;
	}
}

CYyDb::CYyDb() {
	this->UseMIPMapping = 1;
	this->SubState = 0;
	this->Timer1 = nullptr;
	this->field_2C = -1.0;
	this->field_28 = 1.0;
	this->FPSDivisor = 2;
	this->field_34 = 0;
	this->field_38 = 0;
	this->field_3C = 0;
	this->Timer2 = nullptr;
	this->field_8 = 1;
	this->field_9 = 0;
	this->field_A = 1;
	this->field_40 = -1;
	this->MainState = 0;
	this->field_48 = 0;
	this->field_4C = 0;
	CYyDb::pCYyTexMng = nullptr;
	CYyDb::pCYyVbMng = nullptr;
	CYyDb::SomeStatic = nullptr;
	CYyDb::pCMoTaskMng = nullptr;
	CYyDb::pSomeTaskType = nullptr;
	CYyDb::Res_Unk1 = nullptr;
	CYyDb::MsgWinData1 = nullptr;
	CYyDb::MsgWinData2 = nullptr;
	CYyDb::UnknownClass = nullptr;
	CYyDb::pCXiActorDraw = nullptr;
	this->UseMIPMapping = Config::MainRegistryConfig::MIPMapping;
	this->CameraManager = nullptr;
	CYyDb::TextureDisplayState = 0;
	memset(CYyDb::TextureDisplaySubState, 0, sizeof(CYyDb::TextureDisplaySubState));
	memset(CYyDb::TextureDisplayTaskArr, 0, sizeof(CYyDb::TextureDisplayTaskArr));
	CYyDb::SomeTexturesKindaLikeCApp2[0] = nullptr;
	CYyDb::SomeTexturesKindaLikeCApp2[1] = nullptr;
	CYyDb::SomeByte = 0x80;
	this->field_2E4 = 1.0;
	this->field_2E8 = 0.79100001;
	this->field_2EC = 1.0;
	this->field_2F0 = 1.0;
}

void CYyDb::InitSubState(short p_state) {
	this->field_6 = 0;
	this->SubState = p_state;
}

void FFXI::CYyDb::CountdownTillClose()
{
	switch (this->SubState) {
	case 0:
		this->CloseTimer = 900;
		++this->SubState;
		break;
	case 1:
		this->CloseTimer -= 1;
		if (this->CloseTimer == 0) {
			this->CloseTimer = 900;
			++this->SubState;
		}
		else {
			if (true) {
				//sub //TODO
				++this->SubState;
				this->CloseTimer = 900;
			}
		}
		break;
	case 2:
		this->CloseTimer -= 1;
		if (this->CloseTimer) {
			if (true)
				++this->SubState;
			//sub //TODO
		}
		else {
			++this->SubState;
		}
		break;
	case 3:
		this->MainState = 14;
		this->field_A = 0;
		break;
	}
}

void FFXI::CYyDb::IncrementSubState()
{
	++this->SubState;
	this->field_6 = 0;
}

char FFXI::CYyDb::Init(char* p_char)
{
	CYy::CDx* CDx = FFXI::CYy::CDx::instance;
	CDx->BeginScene();
	if (!this->SubState) {
		if (CDx->field_915)
			CDx->Gamma.SetGamma();
		FFXI::GlobalStruct::g_GlobalStruct.field_0 = 112;

		this->AppWidth = CDx->AppWidth;
		this->AppHeight = CDx->AppHeight;
		this->UIXRes = FFXI::Config::RuntimeConfig::instance.resolution_ui.X;
		this->UIYRes = FFXI::Config::RuntimeConfig::instance.resolution_ui.Y;
		this->BackgroundXRes = FFXI::Config::RuntimeConfig::instance.resolution_bg.X;
		this->BackgroundYRes = FFXI::Config::RuntimeConfig::instance.resolution_bg.Y;
		
		Globals::SetSomeManagerFloats(0);
		CDx->SetSomeValues(3, 0x80000000, 1.0, 0);
		CDx->SetViewport();
		p_char = nullptr;
		IDirect3DTexture8* texptr = nullptr;
		IDirect3DTexture8* Texture = FFXI::CYy::CDx::instance->CreateTexture(this->BackgroundXRes, this->BackgroundYRes, &texptr, 1, D3DPOOL_DEFAULT);
		if (Texture) {
			IDirect3DSurface8* Stencil = FFXI::CYy::CDx::instance->CreateDepthStencilSurface(this->BackgroundXRes, this->BackgroundYRes);
			if (Stencil) {
				CDx->CAcc2.Texture[0] = Texture;
				CDx->CAcc2.Texture2 = texptr;
				CDx->CAcc2.Surface = Stencil;
				CDx->AddViewportAtOrigin(CDx->CAcc2.Texture[0], CDx->CAcc2.Texture2, CDx->CAcc2.Surface);
				CDx->ClearFullViewport(D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, this->BackColor, 1.0, 0);
				CDx->RevertStage();
				CYyDb::SomeTexturesKindaLikeCApp2[0] = nullptr;
				CYyDb::SomeTexture1 = CDx->CreateTexture(this->BackgroundXRes >> 1, this->BackgroundYRes >> 1, CYyDb::SomeTexturesKindaLikeCApp2, 1, D3DPOOL_DEFAULT);
				//UI res checks
				if (!this->UIXRes || !this->UIYRes) {
					this->UIXRes = this->AppWidth;
					this->UIYRes = this->AppHeight;
				}
				else if (this->UIXRes != this->AppWidth || this->UIYRes != this->AppHeight) {
					CDx->CAcc_4.SomeTexture = CDx->CreateTexture(this->UIXRes, this->UIYRes, &texptr, 1, D3DPOOL_DEFAULT);
					CDx->CAcc_4.Texture2 = texptr;
					CDx->CAcc_4.Surface = CDx->CreateDepthStencilSurface(this->UIXRes, this->UIYRes);
					if (!CDx->CAcc_4.SomeTexture || !CDx->CAcc_4.Surface) {
						this->UIXRes = this->AppWidth;
						this->UIYRes = this->AppHeight;
						if (CDx->CAcc_4.SomeTexture) {
							CDx->CAcc_4.SomeTexture->Release();
							CDx->CAcc_4.SomeTexture = nullptr;
						}
						if (CDx->CAcc_4.Surface) {
							CDx->CAcc_4.Surface->Release();
							CDx->CAcc_4.Surface = nullptr;
						}
					}
				}
				
				//   sub_1000BB20(v48, (int)SomeTexture, this->BackgroundYRes, v56);
				if (Globals::g_pCenv->field_18) {
					IDirect3DTexture8* v50 = CDx->CreateTexture(this->BackgroundXRes, this->BackgroundYRes, nullptr, 1, D3DPOOL_DEFAULT);
					IDirect3DSurface8* v51 = CDx->CreateDepthStencilSurface(this->BackgroundXRes, this->BackgroundYRes);
					if (v50 && v51) {
						CDx->CAcc2.Texture[1] = v50;
						CDx->CAcc2.Surface2 = v51;
						CDx->AddViewportAtOrigin(CDx->CAcc2.Texture[1], CDx->CAcc2.Texture3, CDx->CAcc2.Surface2);
						CDx->ClearFullViewport(D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, this->BackColor, 1.0, 0);
						CDx->RevertStage();
						CYyDb::SomeTexturesKindaLikeCApp2[1] = nullptr;
						CYyDb::SomeTexture2 = CDx->CreateTexture(this->BackgroundXRes, this->BackgroundYRes, CYyDb::SomeTexturesKindaLikeCApp2 + 1, 1, D3DPOOL_DEFAULT);
					}
				}
				if (Config::MainRegistryConfig::dword10445900) {
					IDirect3DTexture8* v65{ nullptr };
					CDx->CAcc_2.SomeTexture = CDx->CreateTexture(0x100, 0x100, &v65, 1, D3DPOOL_DEFAULT);
					CDx->CAcc_2.Texture2 = v65;
					CDx->CAcc_2.Surface = CDx->CreateDepthStencilSurface(0x100, 0x100);
				}
				this->field_32E = 0x80;
				this->field_32D = 0x80;
				this->field_32C = 0x80;
				this->field_334 = 0x80808080;
				this->field_31C = 0x80808080;
				this->field_324 = 0x80000000;
				this->field_328 = 0x80000000;
				this->field_338 = 0x80000000;
				this->field_330 = 1.0;
				this->field_32F = 0;
				this->field_320 = 0x80000000;
				this->IncrementSubState();
				CDx->EndScene();
				return 1;
			}
		}
		CDx->EndScene();
		return 0;
	}
	if (this->SubState == 1) {
		this->Timer1 = CYy::CXITimerLow::GetNew();
		if (this->Timer1) {
			this->Timer2 = CYy::CXITimerLow::GetNew();
			if (this->Timer2) {
				FFXI::Text::InitStrPages();
				if (FFXI::Text::LoadStringTables()) {
					CreateVertexBuffers();
					CYy::BaseProcessor::InitProcessor();
					CYy::CMoTaskMng::DontInitThisTask = true;
					char* objmem = StorageLists::instance->Get(sizeof(CYy::CMoTaskMng), Constants::Enums::MEM_MODE::Ex);
					if (objmem)
						CYyDb::pCMoTaskMng = new (objmem) CYy::CMoTaskMng();
					else {
						CDx->EndScene();
						return 0;
					}
					CYy::CMoTaskMng::DontInitThisTask = false;

					CYy::CMoResourceMng::InitSomeStatics();
					objmem = StorageLists::instance->Get(sizeof(CYy::CMoResourceMng), Constants::Enums::MEM_MODE::Ex);
					if (objmem)
						CYyDb::pCMoResourceMng = new (objmem) CYy::CMoResourceMng();
					else {
						CDx->EndScene();
						return 0;
					}
					CYy::CMoTaskMng::DeleteThisTask = true;
					objmem = StorageLists::instance->Get(sizeof(CYy::SomeTaskType), Constants::Enums::MEM_MODE::Ex);
					if (objmem)
						CYyDb::pSomeTaskType = new (objmem) CYy::SomeTaskType();
					else {
						CDx->EndScene();
						return 0;
					}
					CYyDb::pSomeTaskType->TaskMngShift4(8);

					objmem = StorageLists::instance->Get(sizeof(CYy::CYyTexMng), Constants::Enums::MEM_MODE::Ex);
					if (objmem)
						CYyDb::pCYyTexMng = new (objmem) CYy::CYyTexMng();
					else {
						CDx->EndScene();
						return 0;
					}

					objmem = StorageLists::instance->Get(sizeof(CYy::CYyVbMng), Constants::Enums::MEM_MODE::Ex);
					if (objmem)
						CYyDb::pCYyVbMng = new (objmem) CYy::CYyVbMng();
					else {
						CDx->EndScene();
						return 0;
					}
					CYyDb::SomeStatic = nullptr;
					CYy::CYyResourceFile** Unused;
					int MENU_MissionQuest = Globals::GetLanguageDependentDatIndex(Constants::Enums::LanguageDependentDats::MENU_MissionQuest);
					CYyDb::ResFile_MENUMissionQuest = *CYyDb::pCMoResourceMng->LoadNumFile(&Unused, MENU_MissionQuest);
					int MENU_Unk1 = Globals::GetLanguageDependentDatIndex(Constants::Enums::LanguageDependentDats::MENU_Unk1);
					CYyDb::ResFile_MENUUnk1 = *CYyDb::pCMoResourceMng->LoadNumFile(&Unused, MENU_Unk1);
					int TEX_General = Globals::GetLanguageDependentDatIndex(Constants::Enums::LanguageDependentDats::TEX_General);
					CYyDb::ResFile_TEXGeneral = *CYyDb::pCMoResourceMng->LoadNumFile(&Unused, TEX_General);
					
					if (CYyDb::ResFile_TEXGeneral)
						CYy::CMoResource::DoTheThing(*CYyDb::ResFile_TEXGeneral);

					if (CYyDb::ResFile_MENUMissionQuest)
						CYy::CMoResource::DoTheThing(*CYyDb::ResFile_MENUMissionQuest);
					if (CYyDb::ResFile_MENUUnk1)
						CYy::CMoResource::DoTheThing(*CYyDb::ResFile_MENUUnk1);
					TkManager::g_CTkMenuMng.FrameLoad(&Unused, 0);
					CYyDb::ResFile_TEXIcons1 = *CYyDb::pCMoResourceMng->LoadNumFile2(&Unused, Constants::DatIndices::TEX_Icons1);
					if (CYyDb::ResFile_TEXIcons1)
						CYy::CMoResource::DoTheThing(*CYyDb::ResFile_TEXIcons1);

					CYyDb::ResFile_TEXIcons2 = *CYyDb::pCMoResourceMng->LoadNumFile2(&Unused, Constants::DatIndices::TEX_Icons2);
					if (CYyDb::ResFile_TEXIcons2)
						CYy::CMoResource::DoTheThing(*CYyDb::ResFile_TEXIcons2);
				
					char* mem = new char[4];
					if (!mem) {
						CDx->EndScene();
						return 0;
					}

					CYyDb::Res_Unk1 = reinterpret_cast<CYy::CMoResource**>(mem);
					if (CYyDb::Res_Unk1) {
						*CYyDb::Res_Unk1 = nullptr;

						mem = new char[sizeof(CTkMsgWinData)];

						if (!mem) {
							CDx->EndScene();
							return 0;
						}

						CYyDb::MsgWinData1 = new (mem) CTkMsgWinData();
						if (CYyDb::MsgWinData1) {
							mem = new char[sizeof(CTkMsgWinData)];

							if (!mem) {
								CDx->EndScene();
								return 0;
							}

							CYyDb::MsgWinData2 = new (mem) CTkMsgWinData();
							if (CYyDb::MsgWinData2) {
								mem = new char[sizeof(FFXI::UnknownClass)];
								if (!mem) {
									CDx->EndScene();
									return 0;
								}
								CYyDb::UnknownClass = new (mem) FFXI::UnknownClass();
								if (CYyDb::UnknownClass) {
									CYy::CMoTaskMng::DeleteThisTask = true;
									mem = StorageLists::instance->Get(sizeof(CYy::CTsZoneMap), Constants::Enums::MEM_MODE::Ex);
									((CYy::CMoTask*)mem)->Reset();
									CYyDb::g_pTsZoneMap = new (mem) CYy::CTsZoneMap();
									if (g_pTsZoneMap) {
										g_pTsZoneMap->TaskMngShift4(5);
										CYyDb::g_pFsMenuDraw = new CTk::FsMenuDraw();
										CYyDb::g_pFsMenuDraw->field_0 = false;
										FFXI::Text::TextRenderer::g_MojiDraw = new FFXI::Text::MojiDraw();
										//sub //TODO
										FFXI::CTk::TkManager::TkInit();
										FFXI::Config::FsConfig::FsConfigInit();
										FFXI::Yk::YkManager::YkInit();
										//FaqSystemInit 
										//sub //TODO

										mem = StorageLists::instance->Get(sizeof(CYy::CXiActorDraw), FFXI::Constants::Enums::MEM_MODE::Ex);
										CYy::CMoTaskMng::DeleteThisTask = true;
										if (mem == nullptr) {
											CDx->EndScene();
											return 0;
										}
										CYyDb::pCXiActorDraw = new (mem) CYy::CXiActorDraw();
										this->pCMoTaskMng->SomeShift4(CYyDb::pCXiActorDraw, 6);
										CYyDb::SomeState4 = 0;
										CYyDb::SomeState3 = 0;
										CYyDb::SomeState2 = 0;

										CYy::CMoDx::Init();
										this->InitSubState(0);
										*p_char = 1;
										goto LABEL_101;
									}
								}
							}
						}
					}
				}
			}
		}
	LABEL_85:
		CDx->EndScene();
		return 0;
	}
LABEL_101:
	CDx->EndScene();
	return 1;
}

void FFXI::CYyDb::Clean()
{
	//dancer //sub //TODO

	CleanDXMembers();
	StorageLists::Clean123();
	FFXI::Text::CleanStrTables();
}

void FFXI::CYyDb::SomeCalc1()
{
	float v5 = 1.0 / (this->field_2F0 * this->field_2E8);
	long double v2 = 2 * atan2(192.0, this->field_2F4);
	this->field_154.RHPerspective(v2, v5, this->field_2DC, this->field_2E0);
	this->field_1D4 = this->field_154;
}

void RenderSomething(CYyDb* a1)
{
	FFXI::CYy::CDx* cdx = FFXI::CYy::CDx::instance;
	if ((a1->field_324 & 0x00FFFFFF) != 0) {
		D3DVIEWPORT8* vp = cdx->GetTopViewport();
		some_verts[0].X = (float)vp->X;
		some_verts[0].Y = (float)vp->Y;
		some_verts[1].X = (float)vp->Width;
		some_verts[1].Y = (float)vp->Y;
		some_verts[2].X = (float)vp->X;
		some_verts[2].Y = (float)vp->Height;
		some_verts[3].X = (float)vp->Width;
		some_verts[3].Y = (float)vp->Height;
		int double_alpha = 2 * ((a1->field_324 >> 24) & 0xFF);
		if (double_alpha > 255) {
			double_alpha = 255;
		}
		a1->field_324 &= 0x00FFFFFF;
		a1->field_324 |= double_alpha << 24;
		some_verts[0].DiffuseColor = a1->field_324;
		some_verts[1].DiffuseColor = a1->field_324;
		some_verts[2].DiffuseColor = a1->field_324;
		some_verts[3].DiffuseColor = a1->field_324;

		cdx->DXDevice->SetRenderState(D3DRS_ZENABLE, false);
		cdx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		cdx->DXDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		cdx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		cdx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		cdx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		cdx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		cdx->DXDevice->SetVertexShader(some_verts[0].FVF);
		cdx->DrawVerts(D3DPT_TRIANGLESTRIP, 2, some_verts, sizeof(some_verts[0]));
		if (Globals::g_pCenv->CheckField18() == true) {
			cdx->AddViewportAtOrigin(cdx->CAcc2.Texture[1], cdx->CAcc2.Texture3, nullptr);
			cdx->DrawVerts(D3DPT_TRIANGLESTRIP, 2, some_verts, sizeof(some_verts[0]));
			cdx->RevertStage();
		}
		cdx->DXDevice->SetRenderState(D3DRS_ZENABLE, true);
		cdx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
		cdx->DXDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		cdx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
	if (a1->SomeTexture1 != nullptr) {
		if (a1->field_32F != 0) {
			exit(0x100128E5);
		}
	}
}

char FFXI::CYyDb::MainFlow()
{
	CYy::CDx* CDx = FFXI::CYy::CDx::instance;
	float v4{};
	int v10{};
	if (Globals::g_pCenv) {
		v4 = this->CheckTick();
		Globals::g_pCenv->DoSomething(v4);
	}

	if (!CDx->DeviceGood()) {
		FFXI::CYy::CApp::g_pCApp->field_1C = 0;
		if (Config::RuntimeConfig::instance.window_mode != Config::RuntimeConfig::WindowMode::Fullscreen)
			FFXI::PolVars::instance.g_polErrorCode = 10;
		else
			FFXI::PolVars::instance.g_polErrorCode = 3;
		return 0;
	}
	CDx->BeginScene();
	CDx->SetRenderStates();

	this->pCYyTexMng->CheckTex();
	//Some pol func done here
	if (Globals::g_pCenv->CheckField18()) { 
		CDx->AddViewportAtOrigin(CDx->CAcc2.Texture[1], CDx->CAcc2.Texture3, CDx->CAcc2.Surface2);
		CDx->ClearFullViewport(D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, this->BackColor, 1.0, 0);
		CDx->RevertStage();
	}
	
	CDx->AddViewportAtOrigin(CDx->CAcc2.Texture[0], CDx->CAcc2.Texture2, CDx->CAcc2.Surface);
	CDx->ClearFullViewport(D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, this->BackColor, 1.0, 0);
	
	if (FFXI::GlobalStruct::g_GlobalStruct.field_0 == 0xA0) {
		this->MainState = 13;
		this->HandleState();
	}
	else if (this->MainState == 17 || FFXI::GlobalStruct::g_GlobalStruct.field_0 == 0x90)
	{
		this->HandleState();
	} else if (this->MainState == 13) {
		this->OnMove();
		this->HandleState();
	} else 	if (!FFXI::CYy::CApp::g_pCApp->g_pNT_SYS) {
		this->CheckMoveHandle();
	}
	else {
		v10 = FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->modeGet();
		switch (v10) {
		case 12:
			this->MainState = 17;
			CYyDb::SomeState = 12;
			break;
		case 21:
			this->MainState = 17;
			CYyDb::SomeState = 21;
			break;
		case 202:
			exit(4003);
		case 203:
			if (FFXI::GlobalStruct::g_GlobalStruct.field_0 != 64)
				exit(4004);
			this->CheckMoveHandle();
			break;
		case 201:
			exit(4005);
		default:
			this->CheckMoveHandle();
			break;
		}
	}
	

	FFXI::CYy::CDx::instance->field_828 = 0;
	//if some dword some sub
	FFXI::CYy::CDx::instance->SetRenderStates();
	RenderTextureTask();
	RenderSomething(this);
	if (CYyDb::SomeByte < 0x80u) {
		int value{ 0 };
		int temp = CYyDb::SomeByte + 0x80u;
		this->field_338 = (unsigned char)(((this->field_320 >> 16) & 0xFF) + temp);
		this->field_338 <<= 8;
		this->field_338 |= ((this->field_320 >> 8) & 0xFF) + temp;
		this->field_338 <<= 8;
		this->field_338 |= ((this->field_320) & 0xFF) + temp;
	}
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)(D3DTSS_MAGFILTER), D3DTEXF_LINEAR);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)(D3DTSS_MINFILTER), D3DTEXF_LINEAR);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)(D3DTSS_ADDRESSU), D3DTADDRESS_CLAMP);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)(D3DTSS_ADDRESSV), D3DTADDRESS_CLAMP);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	CDx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CDx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CDx->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	short v7C = TkManager::g_CTkMenuMng.field_7C;
	short v7E = TkManager::g_CTkMenuMng.field_7E;
	short v80 = TkManager::g_CTkMenuMng.UIXRes;
	short v82 = TkManager::g_CTkMenuMng.UIYRes;
	TkManager::g_CTkMenuMng.UIXRes = this->BackgroundXRes;
	TkManager::g_CTkMenuMng.UIYRes = this->BackgroundYRes;
	//sub //TODO
	TkManager::g_CTkMenuMng.field_7C = v7C;
	TkManager::g_CTkMenuMng.field_7E = v7E;
	TkManager::g_CTkMenuMng.UIXRes = v80;
	TkManager::g_CTkMenuMng.UIYRes = v82;
	CDx->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	CDx->RevertStage();
	unsigned short AppWidth = this->AppWidth;
	unsigned short AppHeight = this->AppHeight;
	D3DVIEWPORT8 Viewport{};
	Viewport.X = 0;
	Viewport.Y = 0;
	Viewport.Width = AppWidth;
	Viewport.Height = AppHeight;
	Viewport.MinZ = 0.0;
	Viewport.MaxZ = 1.0;
	CDx->AddSetViewport(&Viewport);
	unsigned short v24 = this->BackgroundXRes;
	unsigned short v23 = this->AppWidth;
	unsigned short v25 = this->AppHeight;
	unsigned short v26 = this->BackgroundYRes;
	D3DCOLOR col = (this->field_320 & 0x00FFFFFF) | 0x80000000;
	DWORD arr[] = {
		0, 
		0,
		this->AppWidth,
		this->AppHeight
	};
	CDx->SomeCAcc2Render(&CDx->CAcc2, col, arr);
	CDx->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	if (FFXI::CYy::CApp::g_pCApp->g_pNT_SYS) {
		if (FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->field_B8 < 0) {
			//sub //todo? polircstts
		}
	}
	IDirect3DTexture8* SomeTexture = CDx->CAcc_4.SomeTexture;
	IDirect3DSurface8* Surface = CDx->CAcc_4.Surface;
	if (SomeTexture && Surface) {
		CDx->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		CDx->AddViewportAtOrigin(SomeTexture, CDx->CAcc_4.Texture2, Surface);
		CDx->DoViewportRender(CDx->CAcc2.Texture[0], this->field_320 & 0xFFFFFF , -1);
		CDx->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
		CDx->DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 15);
	}

	//input stuf
	FFXI::CYy::CApp::g_pCApp->g_pInputMng->Process();
	Input::InputMng::ResetVirtualKey();
	//sub //TODO 
	if (this->field_8) {
		if (CYyDb::g_pFsMenuDraw)
			CYyDb::g_pFsMenuDraw->onMenuKeyDispatch();
		//sub TODO
	}
	//sub //TODO
	if (this->field_8) {
		if (CYyDb::g_pFsMenuDraw) {
			TkManager::g_CTkMenuMng.InitDraw();
			CYyDb::g_pFsMenuDraw->field_0 = true;
			TkManager::g_CTkMenuMng.OnCalc();
			TkManager::g_CTkMenuMng.OnDraw();
			TkManager::g_CTkMenuMng.EndDraw();
		}
		//sub //TODO here
		if (CYyDb::XiOpening)
			CYyDb::XiOpening->Update();
	}
	TkManager::g_pCTkMouse->UpdateState();

	if (CDx->CAcc_4.SomeTexture && CDx->CAcc_4.Surface) {
		CDx->RevertStage();
		CDx->SetRenderStates();
		CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		CDx->DXDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, 3);
		CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, 3);
		CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, 2);
		CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, 2);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		CDx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		CDx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		CDx->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		CDx->DXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		CDx->DXDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		CDx->DrawViewportWithZedd(CDx->CAcc_4.SomeTexture, 1.0);
		CDx->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
		CDx->DXDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		CDx->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
	
	//Screen Fade Out
	CDx->DrawSomeVerts(0, this->field_31C);

	//sub //TODO lots of stuff here
	float v38{};
	int RegionCode = FFXI::GlobalStruct::g_GlobalStruct.GetRegionCode();
	//Makes room for subtitles
	if (RegionCode == Constants::Enums::LanguageCode::English)
		v38 = -110.0;
	else
		v38 = -160.0;

	float MovieMods[4] = {
		0,
		(float)this->AppWidth / 640.0f,
		v38,
		(float)this->AppHeight * 0.0021453374f
	};
	CYy::CXiOpening::DrawMovie(0, 0, 640, 512, MovieMods);
	//sub TODO
	if (Globals::g_pCenv && (Globals::g_pCenv->field_1C & 1) != 0) {
		//sub //TODO
	}

	FFXI::CYy::CDx::instance->EndScene();
	Present();

	this->field_28 = 0.0;
	char i{ 0 };
	float v45{ 0 };
	while (true){
		CYy::CXITimerLow* Timer = this->Timer1;
		float f1 = Timer->TimerVirt6();
		float f2 = Timer->TimerVirt3();
		CYyDb::StaticFloat = f1 * f2;
		v45 = 60.0 / CYyDb::StaticFloat;
		this->field_2C = v45;
		this->field_28 = v45;
		if (v45 >= this->FPSDivisor) 
			break;
		Sleep(1u);
		_mm_pause();
		i = 1;

	}

	if (i != 1 && v45 < this->FPSDivisor) {
		float v55{ 0 };
		do {
			CYy::CXITimerLow* Timer = this->Timer1;
			float f1 = Timer->TimerVirt6();
			float f2 = Timer->TimerVirt3();
			CYyDb::StaticFloat = f1 * f2;
			v55 = 60.0 / CYyDb::StaticFloat;
			this->field_2C = v55;
			this->field_28 = v55;
			Sleep(0);
		} while (this->FPSDivisor > this->field_28);
	}

	double v61 = 60.0 / this->Timer1->TimerVirt3() + this->field_3C;
	this->field_38 = v61;
	this->field_3C = v61 - (double)(int)(long long)(v61);
	this->Timer1->TimerVirt2();
	CYyDb::FloatArrayIndex += 1;
	CYyDb::FloatArrayIndex &= 3;
	CYyDb::FloatArray[CYyDb::FloatArrayIndex] = this->field_28;
	this->field_2C = 0.0;
	for (float f : CYyDb::FloatArray) 
		this->field_2C += f;
	this->field_2C *= 0.25;
	this->field_28 = this->field_2C;
	if (this->field_28 < this->FPSDivisor)
		this->field_28 = this->FPSDivisor;
	
	if (this->field_28 > 20.0)
		this->field_28 = 20.0;

	this->field_2C = this->field_28;
	CYyDb::StaticFloat = 60 / this->field_28;
	this->field_34 += 1;
	if (!this->field_34)
		this->field_34 = 1;

	if (this->Timer2) {
		float Floaty = this->Timer2->TimerVirt4();
		if (Floaty > 4000.0) {
			_SYSTEMTIME SystemTime{};
			GetLocalTime(&SystemTime);
			if (SystemTime.wSecond != CYyDb::TimerSeconds) {
				this->Timer2->TimerVirt2();
				float diff = (float)(SystemTime.wSecond - CYyDb::TimerSeconds);
				if (diff < 0.0)
					diff += 60.0;
				diff *= 1000;
				float v85 = Floaty - diff;
				if (v85 < 0.0)
					v85 = -v85;
				if (CYyDb::SomeState2 == 0) {
					CYyDb::SomeState2 = 1;
				}
				else if (CYyDb::SomeState2 == 1) {
					if (v85 >= 1500.0)
						CYyDb::SomeState2 = 2;
				}
				else if (CYyDb::SomeState2 == 2) {
					if (v85 < 1500.0) {
						if (CYyDb::SomeState3++ > 2) {
							this->Timer1->TimerVirt7(1.0);
							CYyDb::SomeState2 = 1;
							CYyDb::SomeState4 = 0;
							CYyDb::SomeState3 = 0;
						}
					}
					else {
						CYyDb::SomeState3 = 0;
						float v99 = Floaty / diff;
						this->Timer1->TimerVirt7(v99);
						if (++CYyDb::SomeState4 > 45) {
							CYyDb::SomeState4 = 0;
							//sub //TODO pGame
						}
					}
				}
				CYyDb::TimerSeconds = SystemTime.wSecond;
			}
		}
	}

	DoSomething();

	this->field_31C = this->field_334;
	this->field_320 = this->field_338;
	this->field_324 = this->field_328;

	if (CYyDb::SomeByte > 0x80u) {
		unsigned int uVar15 = CYyDb::SomeByte & 0xFF;
		unsigned char* data = reinterpret_cast<unsigned char*>(&this->field_324);
		unsigned int uVar11 = data[1] - 0x80 + uVar15;
		unsigned int uVar16 = data[2] - 0x80 + uVar15;
		uVar15 += data[0] - 0x80;
		if (0xFFu < uVar16)
			uVar16 = 0xFFu;
		unsigned int uVar17 = 0xFFu;
		if (uVar11 < 0x100u) {
			uVar17 = uVar11;
		}
		if (0xFFu < uVar15)
			uVar15 = 0xFFu;
		this->field_324 = uVar15;
		this->field_324 |= (uVar17 << 8);
		this->field_324 |= (uVar16 << 16);
	}
	if (FFXI::CYy::CDx::instance->field_915)
		FFXI::CYy::CDx::instance->Gamma.SetGamma();
	return this->field_A;
}

bool FFXI::CYyDb::PhaseInit()
{
	FFXI::CYy::CDx* CDx = FFXI::CYy::CDx::instance;
	int substate = this->SubState;
	int result{};
	switch (substate) {
	case 0:
		this->FPSDivisor = 2;
		this->FsGameLobbyStart();
		if (FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->ClientMode < 3) {
			this->field_334 = 0x80808080;
			return true;
		}
		CYy::CYyCamMng2::InitCameraManager();
		CYy::CXiActor::InitActorStatics();
		CDx->DXDevice->SetRenderState(D3DRS_FOGCOLOR, 0x80808080);
		if (CDx->field_92D) {
			CDx->DXDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
			CDx->DXDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
			CDx->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
		}
		else {
			if (CDx->field_92E) {
				CDx->DXDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
				CDx->DXDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
				if (CDx->field_92F)
					CDx->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, true);
				else
					CDx->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
			}
		}

		CDx->DXDevice->SetRenderState(D3DRS_FOGSTART, 0x42A00000); //80.0
		CDx->DXDevice->SetRenderState(D3DRS_FOGEND, 0x42C80000); //100.0
		CDx->DXDevice->SetRenderState(D3DRS_FOGDENSITY, 0);
		CDx->SetSomeColor(0x80000000);
		IwManager::IwInit();
		IwManager::IwLobbyInit(CYyDb::PatchingComplete);
		CYyDb::PatchingComplete = true;
		this->IncrementSubState();
		return false;
	case 1:
		if (this->CameraManager && CYyDb::g_pCYyDb->g_pTsZoneMap) {
			D3DXVECTOR3 a1 = this->CameraManager->Position;
			//sub //TODO
			CYyDb::g_pCYyDb->g_pTsZoneMap->SomeInit2();
			CYyDb::g_pCYyDb->g_pTsZoneMap->field_39450 = CYyDb::g_pCYyDb->g_pTsZoneMap->field_34350;
		}
		result = IwManager::TaskBuffUpdate();
		if (result) {
			//sub //TODO
			//config update
			if (FFXI::CYy::XiZone::zone != nullptr) {
				FFXI::CYy::XiZone::zone->Close();
				StorageLists::Clean456();
				FFXI::CYy::XiZone::zone->SysInit();
				FFXI::CYy::CYyCamMng2::InitCameraManager();
				FFXI::CYy::CXiActor::InitActorStatics();
			}
			if (result < 0) {
				CYyDb::SomeFlag = 0;
				if (result == -2) {
					//don't think this can happen
					//sub //TODO
					CYy::CApp::SetAppState(2);
				}
				this->MainState = 13;
			}
		}
		if (result <= 0)
			return false;
		CYyDb::SomeFlag = 0;
		this->IncrementSubState();
		return false;
	case 2:
		this->field_334 = 0x80808080;
		PhaseInitCountdown = 60;
		this->IncrementSubState();
		return false;
	case 3:
		if (PhaseInitCountdown) {
			--PhaseInitCountdown;
			return false;
		}
		if (this->field_31C == 0x80808080) {
			if (CameraManager)
				delete CameraManager;
			IwManager::IwDestroy();
			this->field_334 = 0x80808080;
			return true;
		}
		else
			this->SubState -= 1;
		return false;
	default:
		return false;
	}
}

bool FFXI::CYyDb::OpeningMovieIsFinished()
{
	if (this->SubState == 0) {
		int v3 = FFXI::Config::RegistryConfig::g_pOptions->Other.field_1C;
		char Buffer[260];
		if (v3 <= 0 || v3 > 2) {
			strncpy_s(Buffer, sizeof(Buffer), "c:\\image\\ffxi\\mov", sizeof(Buffer) - 1);
		}
		else {
			GetCurrentDirectoryA(sizeof(Buffer), Buffer);
			strncat_s(Buffer, sizeof(Buffer), "\\mov", sizeof(Buffer) - 1);
		}
		char* mem = StorageLists::instance->Get(sizeof(CYy::CXiOpening), Constants::Enums::MEM_MODE::Ex);
		if (mem)
			CYyDb::XiOpening = new (mem) CYy::CXiOpening(Buffer);
		else
			return true;

		this->IncrementSubState();
		return false;
	}
	if (this->SubState != 1 || !CYyDb::XiOpening->IsFinished())
		return false;
	if (!CYyDb::XiOpening)
		return true;
	delete CYyDb::XiOpening;
	CYyDb::XiOpening = nullptr;
	return true;
}

unsigned short FFXI::CYyDb::GetBackgroundYRes()
{
	return CYyDb::g_pCYyDb->BackgroundYRes;
}

unsigned short FFXI::CYyDb::GetBackgroundXRes()
{
	return CYyDb::g_pCYyDb->BackgroundXRes;
}

float FFXI::CYyDb::CheckTick()
{
	if (this->field_28 >= 1.0)
		return this->field_28;
	return 1.0f;
}

void FFXI::CYyDb::SetFarColor(unsigned char a2, unsigned char a3, unsigned char a4)
{
	this->FarColor = a2;
	this->FarColor <<= 8;
	this->FarColor |= a3;
	this->FarColor <<= 8;
	this->FarColor |= a4;
}

void FFXI::CYyDb::SetBackColor(unsigned char a2, unsigned char a3, unsigned char a4)
{
	this->BackColor = a4 | ((a3 | ((a2 | 0xFFFF8000) << 8)) << 8);
}

void FFXI::CYyDb::SetField2FC(float a1)
{
	CYyDb::g_pCYyDb->field_2FC = a1;
}

void FFXI::CYyDb::SetField300(float a1)
{
	CYyDb::g_pCYyDb->field_300 = a1;
}

void FFXI::CYyDb::SetField2F8(float a2)
{
	this->field_2F8 = a2;
}

void FFXI::CYyDb::SetField328(int a2)
{
	this->field_328 = a2;
}

void FFXI::CYyDb::SetField334(int a2)
{
	this->field_334 = a2;
}

void FFXI::CYyDb::SetField338(int a2)
{
	this->field_338 = a2;
}

void FFXI::CYyDb::SetField314(float a2)
{
	CYyDb::g_pCYyDb->field_314 = a2;
}

void FFXI::CYyDb::SetField318(float a2)
{
	CYyDb::g_pCYyDb->field_318 = a2;
}

void FFXI::CYyDb::Set324(int a2)
{
	int now_0 = (this->field_324 >> 0) & 0xFF;
	int now_1 = (this->field_324 >> 8) & 0xFF;
	int now_2 = (this->field_324 >> 16) & 0xFF;

	int new_0 = (a2 >> 0) & 0xFF;
	int new_1 = (a2 >> 8) & 0xFF;
	int new_2 = (a2 >> 16) & 0xFF;

	int sum_0 = now_0 + new_0;
	int sum_1 = now_1 + new_1;
	int sum_2 = now_2 + new_2;
	if (sum_0 > 255) {
		sum_0 = 255;
	}
	if (sum_1 > 255) {
		sum_1 = 255;
	}
	if (sum_2 > 255) {
		sum_2 = 255;
	}
	this->field_324 = a2 & 0xFF000000;
	this->field_324 |= sum_0 << 0;
	this->field_324 |= sum_1 << 8;
	this->field_324 |= sum_2 << 16;
}

void FFXI::CYyDb::SetView(FFXI::Math::WMatrix* a2)
{
	this->field_54 = *a2;
	this->field_94 = *a2;
	this->field_114.Identity();
	this->field_D4 = *a2;
	this->field_D4._41 = 0.0;
	this->field_D4._42 = 0.0;
	this->field_D4._43 = 0.0;
	this->field_294 = this->field_94;
	this->field_294.DoSomething();
}

void FFXI::CYyDb::LinkCameraManager(CYy::CYyCamMng2* a1)
{
	FFXI::Math::WMatrix v3 = *a1->MakeMatrix();
	this->field_54 = v3;
	this->field_94 = v3;
	this->field_D4 = v3;
	this->field_114.Identity();
	this->field_D4._41 = 0.0;
	this->field_D4._42 = 0.0;
	this->field_D4._43 = 0.0;
	this->field_294 = this->field_94;
	this->field_294.DoSomething();
	this->CameraManager = a1;
}