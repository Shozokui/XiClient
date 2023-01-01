#include "CXiOpening.h"
#include "Globals.h"
#include "Subtitles.h"
#include "CXiMovie.h"
#include "CDx.h"
#include "InputMng.h"
#include "streams.h"
#include <iostream>
#include "VideoRenderer.h"
#include "FVF144Vertex.h"
#include "StorageLists.h"
#include "CApp.h"
#include "CYyDb.h"
#include "SoundMng.h"

using namespace FFXI::CYy;

const CYyClass CXiOpening::CXiOpeningClass{
	"CXiOpening", sizeof(CXiOpening), &CYyObject::CYyObjectClass
};
int CXiOpening::MovieFileIndex{ 0 };
CXiMovie* CXiOpening::XiMovie{ nullptr };
const char* MovieFiles[] = {
	"mov999.pmv\0", "\0", "\0"
};

//LOCAL FUNCS
bool TryRun() {
	if (!CXiOpening::XiMovie) return false;

	return !CXiOpening::XiMovie->MediaControl
		|| CXiOpening::XiMovie->MediaControl->Run() >= 0;
}
bool CreateXiMovie(char* a1) {
	FILE* v1{ nullptr };
	fopen_s(&v1, a1, "rb");
	if (!v1) return false;
	fclose(v1);
	
	char* mem = FFXI::StorageLists::instance->Get(sizeof(CXiMovie), FFXI::Constants::Enums::MEM_MODE::Ex);
	if (mem)
		CXiOpening::XiMovie = new (mem) CXiMovie();
	if (CXiOpening::XiMovie == nullptr)
		return false;
	
	if (CXiOpening::XiMovie->InitFilterGraph(a1))
		return true;

	CXiOpening::CleanXiMovie();
	return false;
}
//~LOCAL FUNCS
const CYyClass* CXiOpening::GetRuntimeClass()
{
	return &CXiOpeningClass;
}

CXiOpening::CXiOpening(char* path) {
	this->Subtitle = nullptr;
	this->field_124 = 0;
	CXiOpening::MovieFileIndex = 0;
	this->State = 0;
	this->field_110 = 0;
	if (FFXI::CYy::CDx::instance->field_915) {
		FFXI::CYy::CDx::instance->Gamma.GetTriplet(&this->field_114, &this->field_118, &this->field_11C);
		FFXI::CYy::CDx::instance->Gamma.SetTriplet(1.0, 1.0, 1.0);
	}

	strncpy_s(this->MoviePath, sizeof(this->MoviePath), path, sizeof(this->MoviePath) - 1);
	
	int RegionCode = FFXI::GlobalStruct::g_GlobalStruct.GetRegionCode();
	this->Subtitle = new Subtitles(RegionCode);
	
	if (this->Subtitle != nullptr) {
		this->Subtitle->Init();
	}
}

CXiOpening::~CXiOpening() 
{
	if (this->Subtitle != nullptr) {
		delete this->Subtitle;
		this->Subtitle = nullptr;
	}

	if (FFXI::CYy::CDx::instance->field_915 != 0) {
		FFXI::CYy::CDx::instance->Gamma.SetTriplet(this->field_114, this->field_118, this->field_11C);
	}
}

void FFXI::CYy::CXiOpening::DrawMovie(int left, int top, int right, int bottom, float* Mods)
{
	CXiMovie* movie = CXiOpening::XiMovie;
	if (movie) {
		{ //inline sub
			movie->MediaPosition->get_CurrentPosition(&movie->MediaPosBackup);
			long v4{ 0 }, v5{ 0 }, v6{ 0 };
	
			bool v2 = movie->MediaEvent->GetEvent(&v4, &v5, &v6, 0) >= S_OK;
			if (v2) {
				if (v4 == EC_COMPLETE) {
					movie->MediaPosition->put_CurrentPosition(0);
					movie->field_4 = 2;
				}
				movie->MediaEvent->FreeEventParams(v4, v5, v6);
			}
		}//~inline sub
		IDirect3DTexture8* tex = movie->GetTexture();
		if (tex) {
			CDx* CDx = FFXI::CYy::CDx::instance;
			CDx->DXDevice->SetTexture(0, tex);
			D3DSURFACE_DESC8 v11{};
			if (tex->GetLevelDesc(0, &v11) >= D3D_OK) {
				
				FVF144Vertex Verts[4]{};
				float modleft = (left + Mods[0]) * Mods[1];
				float modright = (right + Mods[0]) * Mods[1];
				float modbottom = (bottom + Mods[2]) * Mods[3];
				float modtop = (top + Mods[2]) * Mods[3];
				Verts[0].X = modleft;
				Verts[0].Y = modtop;
				Verts[0].Z = 0.0;
				Verts[0].RHW = 1.0;
				Verts[0].DiffuseColor = 0x80808080;
				Verts[0].TexVertX = 0.0;
				Verts[0].TexVertY = 1.0;
				
				Verts[1].X = modright;
				Verts[1].Y = modtop;
				Verts[1].Z = 0.0;
				Verts[1].RHW = 1.0;
				Verts[1].DiffuseColor = 0x80808080;
				Verts[1].TexVertX = 1.0;
				Verts[1].TexVertY = 1.0;

				Verts[2].X = modleft;
				Verts[2].Y = modbottom;
				Verts[2].Z = 0.0;
				Verts[2].RHW = 1.0;
				Verts[2].DiffuseColor = 0x80808080;
				Verts[2].TexVertX = 0.0;
				Verts[2].TexVertY = 0.0;

				Verts[3].X = modright;
				Verts[3].Y = modbottom;
				Verts[3].Z = 0.0;
				Verts[3].RHW = 1.0;
				Verts[3].DiffuseColor = 0x80808080;
				Verts[3].TexVertX = 1.0;
				Verts[3].TexVertY = 0.0;

				CDx->SetRenderStates();
				CDx->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
				CDx->DXDevice->SetRenderState(D3DRS_FOGENABLE, false);
				CDx->DXDevice->SetRenderState(D3DRS_LIGHTING, false);
				CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
				CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
				CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_LINEAR);
				CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
				CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				CDx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
				CDx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
				CDx->DXDevice->SetVertexShader(FVF144Vertex::FVF);
				CDx->DrawVerts(D3DPT_TRIANGLESTRIP, 2, Verts, sizeof(FVF144Vertex));
				CDx->SetRenderStates();
				CDx->DXDevice->SetTexture(0, nullptr);
			}
		}

	}
}

void FFXI::CYy::CXiOpening::CleanXiMovie()
{
	if (CXiOpening::XiMovie) {
		delete CXiOpening::XiMovie;
		CXiOpening::XiMovie = nullptr;
	}
}

bool FFXI::CYy::CXiOpening::IsFinished()
{
	return this->State == 4;
}

void FFXI::CYy::CXiOpening::Update()
{
	
	switch (this->State) {
	case 0:
		if (!MovieFiles[CXiOpening::MovieFileIndex]) {
			this->State = 4;
			return;
		}
		char v8[260];
		_snprintf_s(v8, sizeof(v8), "%s\\%s", this->MoviePath, MovieFiles[CXiOpening::MovieFileIndex]);
		if (CreateXiMovie(v8)) {
			if (FFXI::SoundMng::g_pSoundMng) {
				_snprintf_s(v8, sizeof(v8), "%s\\%s", this->MoviePath, "music999.bgw");
				FILE* v2{ nullptr };
				fopen_s(&v2, v8, "rb");
				if (v2)
					fclose(v2);
				SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::PlaySoundFromDisk, (int)v8, 0x7F, 0, 0, 0, 0, 0);
			}
			if (TryRun()) {
				this->State = 1;
			}
			else {
				this->State = 4;
				if (FFXI::SoundMng::g_pSoundMng)
					SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::MusicFadeOut, -1, 0, 0, 0, 0, 0, 0);
			}
		}
		break;
	case 1:
	{
		REFTIME v3 = 0.0;
		v3 = CXiOpening::XiMovie->MediaPosBackup;

		REFTIME v5 = v3 * 30.0;
		if (this->Subtitle) {
			this->Subtitle->field_10 = v5;
			this->Subtitle->Update();
		}
		if (CXiOpening::XiMovie->field_4 == 2) {
			CXiOpening::MovieFileIndex += 1;
			CleanXiMovie();
			if (this->Subtitle)
				this->Subtitle->Init();
			this->State = 2;
		}
		else if (FFXI::CYy::CApp::g_pCApp->g_pInputMng->CheckThing(0)) {
			CleanXiMovie();
			this->State = 3;
			this->field_110 = 240;
			if (FFXI::SoundMng::g_pSoundMng)
				FFXI::SoundMng::g_pSoundMng->QueueNewSoundCommand(Constants::Enums::SoundCommand::MusicFadeOut, -1, 60, 0, 0, 0, 0, 0);
		}
	}
		break;
	case 2:
		this->State = 3;
		break;
	case 3:
		this->field_110 -= CYyDb::g_pCYyDb->CheckTick();
		if (this->field_110 < 0)
			this->State = 4;
		break;
	default:
		return;
	}
}
