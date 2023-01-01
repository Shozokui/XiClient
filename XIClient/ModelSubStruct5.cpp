#include "ModelSubStruct5.h"
#include "FsConfig.h"
#include "FVF144Vertex.h"
#include "Globals.h"
#include "CDx.h"
#include "FsConfig.h"
#include "CEnv.h"

using namespace FFXI::CYy;

D3DCOLOR ModelSubStruct5::const_color{};
ModelSubStruct5::ModelSubStruct5() {
	this->field_4 = 0;
	this->field_8 = 0;
}

ModelSubStruct5::~ModelSubStruct5() {
	//nullsub
}

void FFXI::CYy::ModelSubStruct5::SetConstColor()
{
	const_color = 0x80808080;
}

void FFXI::CYy::ModelSubStruct5::DrawSomething()
{
	if (FFXI::Config::FsConfig::GetConfig(FFXI::Constants::Enums::FsConfigSubjects::Subject200) == 0)
		return;

	if (this->field_4 == 0)
		return;

	if (Globals::g_pSomeTexture8 == nullptr)
		return;

	if (Globals::g_pSomeTexture1 == nullptr)
		return;

	FVF144Vertex vertData[4]{};

	vertData[0].DiffuseColor = ModelSubStruct5::const_color;
	vertData[1].DiffuseColor = ModelSubStruct5::const_color;
	vertData[2].DiffuseColor = ModelSubStruct5::const_color;
	vertData[3].DiffuseColor = ModelSubStruct5::const_color;

	vertData[0].RHW = 1.0;
	vertData[1].RHW = 1.0;
	vertData[2].RHW = 1.0;
	vertData[3].RHW = 1.0;

	vertData[0].TexVertX = 0.0;
	vertData[0].TexVertY = 0.0;
	vertData[1].TexVertX = 1.0;
	vertData[1].TexVertY = 0.0;
	vertData[2].TexVertX = 0.0;
	vertData[2].TexVertY = 1.0;
	vertData[3].TexVertX = 1.0;
	vertData[3].TexVertY = 1.0;

	vertData[0].X = 0.0;
	vertData[0].Y = 0.0;
	vertData[0].Z = 0.0;
	vertData[1].X = 256.0;
	vertData[1].Y = 0.0;
	vertData[1].Z = 0.0;
	vertData[2].X = 0.0;
	vertData[2].Y = 256.0;
	vertData[2].Z = 0.0;
	vertData[3].X = 256.0;
	vertData[3].Y = 256.0;;
	vertData[3].Z = 0.0;

	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	if ((ModelSubStruct5::const_color & 0xFF000000) != 0x80000000) {
		FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
		FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	}

	FFXI::CYy::CDx::instance->DXDevice->SetVertexShader(vertData->FVF);
	int backup = FFXI::CYy::CDx::instance->field_828;
	FFXI::CYy::CDx::instance->field_828 = 0;
	if (Globals::g_pCenv->CheckField18() == true) {
		if (Globals::g_pSomeTexture10 != nullptr) {
			if (Globals::g_pSomeTexture3 != nullptr) {
				FFXI::CYy::CDx::instance->DXDevice->SetTexture(0, Globals::g_pSomeTexture3);
				FFXI::CYy::CDx::instance->AddViewportAtOrigin(Globals::g_pSomeTexture10, Globals::g_pSomeTexture9, 0);
				FFXI::CYy::CDx::instance->DrawVerts(D3DPT_TRIANGLESTRIP, 2, &vertData[0].Y, sizeof(vertData[0]));
				FFXI::CYy::CDx::instance->RevertStage();
			}
		}
	}

	FFXI::CYy::CDx::instance->DXDevice->SetTexture(0, (IDirect3DBaseTexture8*)Globals::g_pSomeTexture1);
	FFXI::CYy::CDx::instance->AddViewportAtOrigin(Globals::g_pSomeTexture8, Globals::g_pSomeTexture7, 0);
	FFXI::CYy::CDx::instance->DrawVerts(D3DPT_TRIANGLESTRIP, 2, &vertData[0].X, sizeof(vertData[0]));
	FFXI::CYy::CDx::instance->RevertStage();
	FFXI::CYy::CDx::instance->field_828 = backup;
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

void FFXI::CYy::ModelSubStruct5::DrawAnotherSomething()
{
	if (FFXI::Config::FsConfig::GetConfig(FFXI::Constants::Enums::FsConfigSubjects::Subject200) == 0)
		return;

	if (this->field_4 == 0)
		return;

	exit(0x10037FFA);
}

CMoResource** ModelSubStruct5::GetResource() {
	int result = Config::FsConfig::GetConfig(FFXI::Constants::Enums::FsConfigSubjects::Subject200);
	if (result == 0)
		return this->field_4;
	
	return 0;
}