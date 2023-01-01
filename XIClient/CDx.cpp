#include "CDx.h"
#include "FVF144Vertex.h"
#include "CEnv.h"
#include "RuntimeConfig.h"
#include "Globals.h"
#include <iostream>
using namespace FFXI::CYy;

CDx* CDx::instance{ nullptr };
const CYyClass CDx::CDxClass{
	"CDx", sizeof(CDx), &CYyObject::CYyObjectClass
};

bool FFXI::CYy::CDx::Initialize(IDirect3D8* dx_interface, Direct3DDevice8* dx_device)
{
	IDirect3DSurface8* Surface{}, * Surface2{};
	//CDx init
	char* mem = StorageLists::instance->Get(sizeof(CYy::CDx), Constants::Enums::MEM_MODE::Ex);
	if (mem == nullptr) {
		return false;
	} 
		
	FFXI::CYy::CDx::instance = new (mem) CYy::CDx(dx_interface, dx_device);

	FFXI::CYy::CDx::instance->AppWidth = FFXI::Config::RuntimeConfig::instance.resolution_main.X;
	FFXI::CYy::CDx::instance->AppHeight = FFXI::Config::RuntimeConfig::instance.resolution_main.Y;

	FFXI::CYy::CDx::instance->DXDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &Surface);
	FFXI::CYy::CDx::instance->BackbufferSurface = Surface;
	Surface->Release();

	FFXI::CYy::CDx::instance->DXDevice->GetDepthStencilSurface(&Surface2);
	FFXI::CYy::CDx::instance->StencilDepthSurface = Surface2;
	Surface2->Release();

	return true;
}

void FFXI::CYy::CDx::Uninitialize()
{
	if (FFXI::CYy::CDx::instance) {
		delete FFXI::CYy::CDx::instance;
		FFXI::CYy::CDx::instance = nullptr;
	}
}

const CYyClass* CDx::GetRuntimeClass() {
	return &CDxClass;
}

FVF44Vertex CDx::SomeBasicVertices[4] = {
	{0.0, 0.0, 0.0, 1.0, 0x80808080},
	{0.0, 0.0, 0.0, 1.0, 0x80808080},
	{0.0, 0.0, 0.0, 1.0, 0x80808080},
	{0.0, 0.0, 0.0, 1.0, 0x80808080}
};
CDx::~CDx() {
	//TODO: IMPLEMENT
	if (this->StateBlock1 && this->StateBlock1 != -1)
		this->DXDevice->DeleteStateBlock(this->StateBlock1);

	if (this->StateBlock2 && this->StateBlock2 != -1)
		this->DXDevice->DeleteStateBlock(this->StateBlock2);
	
	if (this->field_814) {
		//Release
		this->field_814 = 0;
	}
	if (this->field_818) {
		//Release
		this->field_818 = 0;
	}
	if (this->field_81C) {
		//Release
		this->field_81C = 0;
	}
	if (this->field_820) {
		//Release
		this->field_820 = 0;
	}
	if (this->field_824) {
		//Release
		this->field_824 = 0;
	}
	if (this->field_82C) {
		//Release
		this->field_82C = 0;
	}
}

bool FFXI::CYy::CDx::SetSurface(IDirect3DSurface8* p_bbSurface, IDirect3DSurface8* p_sdSurface)
{
	this->BackbufferSurface = p_bbSurface;
	this->StencilDepthSurface = p_sdSurface;
	return true;
}

bool FFXI::CYy::CDx::CheckSurface(IDirect3DTexture8** a2, IDirect3DSurface8** a3)
{
	//TODO Verify this func
	if (this->field_828 == 0 || Globals::g_pCenv->CheckField18() == false)
		return false;
	bool v4 = false;
	
	IDirect3DSurface8* v13{ nullptr };
	FFXI::CYy::CDx::instance->DXDevice->GetRenderTarget(&v13);

	IDirect3DSurface8* v17{ nullptr }, * v18{ nullptr };
	this->CAcc2.Texture[0]->GetSurfaceLevel(0, &v17);
	this->CAcc2.Texture[1]->GetSurfaceLevel(0, &v18);
	if (v13 == v17 || v13 == v18) {
		*a3 = this->CAcc2.Surface2;
		*a2 = this->CAcc2.Texture[1];
		v4 = *a3 && *a2;
	}
	v17->Release();
	v18->Release();
	if (!v4) {
		IDirect3DSurface8* v12{};
		Globals::g_pSomeTexture1->GetSurfaceLevel(0, &v12);
		if (v13 == v12) {
			v4 = Globals::g_pSomeTexture3 && this->CAcc2.Surface2;
		}
		v12->Release();
	}
	if (v13)
		v13->Release();
	return v4;
}

bool FFXI::CYy::CDx::DeviceGood()
{
	return this->DXDevice->TestCooperativeLevel() == D3D_OK;
}

int FFXI::CYy::CDx::BeginScene()
{
	return this->DXDevice->BeginScene();
}

int FFXI::CYy::CDx::EndScene()
{
	return this->DXDevice->EndScene();
	return 0;
}

void FFXI::CYy::CDx::SetFromCursors(unsigned char a2)
{

	if (!FFXI::Config::MainRegistryConfig::HardwareMouse) return;

	int v4{};
	switch (a2) {
		case 0:
		case 3:
		case 6:
			v4 = 0;
			break;
		case 1:
		case 2:
			v4 = 1;
			break;
		case 4:
		case 5:
			v4 = 2;
			break;
		default:
			break;
	}
	HCURSOR CursorA = this->Cursors[v4];
	if (!CursorA) {
		CursorA = LoadCursorA(0, MAKEINTRESOURCEA(0x7F00));
		this->Cursors[v4] = CursorA;
	}
	SetCursor(CursorA);
}

void FFXI::CYy::CDx::SetSomeValues(int p1, D3DCOLOR p2, float p3, int p4)
{
	this->field_164 = p1;
	this->SomeColor = p2;
	this->field_16C = p3;
	this->field_170 = p4;
}

void FFXI::CYy::CDx::SetViewport()
{
	D3DVIEWPORT9* viewport = &this->Viewports[this->ViewportIndex];
	viewport->X = 0;
	viewport->Y = 0;
	viewport->Width = this->AppWidth;
	viewport->Height = this->AppHeight;
	viewport->MinZ = 0;
	viewport->MaxZ = 1.0;
	this->DXDevice->SetViewport(viewport);
}

int FFXI::CYy::CDx::AddViewportAtOrigin(IDirect3DTexture8* p1, IDirect3DTexture8* p2, IDirect3DSurface8* p3)
{
	short v5[2]{};
	v5[0] = 0;
	v5[1] = 0;
	return this->AddViewport(p1, p2, p3, v5, v5, 1, 1);
}

int FFXI::CYy::CDx::AddViewport(IDirect3DTexture8* p1, IDirect3DTexture8* p2, IDirect3DSurface8* p3, short* p4, short* p5, int p6, int p7)
{
	//83 EC 20 53 56 8B F1 57 8B 7C 24 30 8D 4C 24 30
	IDirect3DSurface8* a2{ nullptr };
	D3DSURFACE_DESC8 v15{};
	++this->ViewportIndex;
	p1->GetSurfaceLevel(0, &a2);
	HRESULT result = this->DXDevice->SetRenderTarget(a2, p3);
	a2->Release();
	a2->GetDesc(&v15);
	D3DVIEWPORT8* viewport = &this->Viewports[this->ViewportIndex];
	viewport->X = p4[0];
	viewport->Y = p4[1];
	viewport->Width = v15.Width + p5[0];
	viewport->Height = v15.Height + p5[1];
	viewport->MinZ = 0.0;
	viewport->MaxZ = 1.0;
	this->DXDevice->SetViewport(viewport);
	this->Textures2[this->ViewportIndex] = p1;
	this->RenderTargets[this->ViewportIndex] = 0;
	this->Textures[this->ViewportIndex] = p2;
	this->Stencils[this->ViewportIndex] = p3;
	return this->ViewportIndex;
}

D3DVIEWPORT8* FFXI::CYy::CDx::GetTopViewport()
{
	return this->Viewports + this->ViewportIndex;
}

void FFXI::CYy::CDx::AddSetViewport(D3DVIEWPORT8* a2)
{
	memcpy(&this->Viewports[this->ViewportIndex], a2, sizeof(this->Viewports[this->ViewportIndex]));
	this->DXDevice->SetViewport(a2);
}

int FFXI::CYy::CDx::ClearFullViewport(DWORD p1, D3DCOLOR p2, float p3, DWORD p4)
{
	return this->ClearViewport(0, NULL, p1, p2, p3, p4);
}

int FFXI::CYy::CDx::ClearViewport(DWORD p1, D3DRECT* p2, DWORD p3, D3DCOLOR p4, float p5, DWORD p6)
{
	this->EndScene();
	DWORD v8 = p3;
	if (FFXI::Config::RegistryConfig::g_pOptions->Other.field_58 && (v8 & 2) != 0)
		v8 |= 4;
	HRESULT result = this->DXDevice->Clear(p1, p2, v8, p4, p5, p6);
	return this->BeginScene();
}

void FFXI::CYy::CDx::RevertStage()
{
	this->DXDevice->SetTexture(0, NULL);
	this->RewindStage();
}

void FFXI::CYy::CDx::RewindStage()
{
	IDirect3DTexture8* tex{ nullptr };
	IDirect3DSurface8* surf{ nullptr };
	this->ViewportIndex -= 1;
	if (this->ViewportIndex) {
		tex = this->Textures2[this->ViewportIndex];
		if (tex) {
			tex->GetSurfaceLevel(0, &surf);
			this->DXDevice->SetRenderTarget(surf, this->Stencils[this->ViewportIndex]);
			surf->Release();
		}
		else {
			surf = this->RenderTargets[this->ViewportIndex];
			if (surf)
				this->DXDevice->SetRenderTarget(surf, this->Stencils[this->ViewportIndex]);
		}
	}
	else {
		this->DXDevice->SetRenderTarget(this->BackbufferSurface, this->StencilDepthSurface);
	}
	this->DXDevice->SetViewport(&this->Viewports[this->ViewportIndex]);
}

void FFXI::CYy::CDx::ShowCursor(bool a1)
{
	if (Config::MainRegistryConfig::HardwareMouse) {
		this->DXDevice->ShowCursor(a1);
		if (a1) return;
	}
	else if (FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::BorderlessFullscreen) {
		if (a1)	return;
	}
	else if (!a1)
		return;

	SetCursor(NULL);
}

IDirect3DTexture8* FFXI::CYy::CDx::CreateTexture(unsigned short p1, unsigned short p2, IDirect3DTexture8** p3, int p4, D3DPOOL p5)
{
	D3DSURFACE_DESC8 v9;
	if (p3)
		*p3 = nullptr;

	this->BackbufferSurface->GetDesc(&v9);
	if (this->DXDevice->CreateTexture(p1, p2, 1, p4, D3DFMT_A8R8G8B8, p5, p3) < D3D_OK
		&& this->DXDevice->CreateTexture(p1, p2, 1, p4, D3DFMT_A1R5G5B5, p5, p3) < D3D_OK
		&& this->DXDevice->CreateTexture(p1, p2, 1, p4, D3DFMT_X8R8G8B8, p5, p3) < D3D_OK
		&& this->DXDevice->CreateTexture(p1, p2, 1, p4, D3DFMT_X1R5G5B5, p5, p3) < D3D_OK
		&& this->DXDevice->CreateTexture(p1, p2, 1, p4, v9.Format, p5, p3) < D3D_OK)
	{
		return nullptr;
	}

	return *p3;
}

IDirect3DSurface8* FFXI::CYy::CDx::CreateDepthStencilSurface(unsigned short p1, unsigned short p2)
{
	D3DSURFACE_DESC8 v7{};
	IDirect3DSurface8* v6{ nullptr };
	this->StencilDepthSurface->GetDesc(&v7);
	if (this->DXDevice->CreateDepthStencilSurface(p1, p2, v7.Format, v7.MultiSampleType, &v6) < 0)
		return nullptr;

	return v6;
}

void FFXI::CYy::CDx::SetRenderStates()
{
	bool result1 = this->StateBlockFunc(D3DSBT_VERTEXSTATE, &this->StateBlock2);
	if (result1) {
		this->DXDevice->SetRenderState(D3DRS_FOGENABLE, false);
		this->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
		this->DXDevice->SetRenderState(D3DRS_CLIPPING, true);
		this->DXDevice->SetRenderState(D3DRS_LIGHTING, true);
		this->DXDevice->SetRenderState(D3DRS_AMBIENT, 0);
	
		if (this->field_92D) {
			this->DXDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
			this->DXDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
			this->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
		}
		else {
			if (this->field_92E) {
				this->DXDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
				this->DXDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
				if (this->field_92F)
					this->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, true);
				else
					this->DXDevice->SetRenderState(D3DRS_RANGEFOGENABLE, false);
			}
		}
		
		this->DXDevice->SetRenderState(D3DRS_COLORVERTEX, true);
		this->DXDevice->SetRenderState(D3DRS_LOCALVIEWER, false);
		this->DXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
		this->DXDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		this->DXDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);
		this->DXDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
		this->DXDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
		this->DXDevice->SetRenderState(D3DRS_VERTEXBLEND, false);
		this->DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, false);
		this->DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		this->DXDevice->SetRenderState(D3DRS_SPECULARENABLE, false);
	}
	this->StateBlockFunc2(3, &this->StateBlock2, result1);
	bool result2 = this->StateBlockFunc(D3DSBT_PIXELSTATE, &this->StateBlock1);
	if (result2) {
		DWORD zero = 0;
		this->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		this->DXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		this->DXDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		this->DXDevice->SetRenderState((D3DRENDERSTATETYPE)(D3DRS_LINEPATTERN), (DWORD)&zero);
		this->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
		this->DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		this->DXDevice->SetRenderState(D3DRS_LASTPIXEL, true);
		this->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		this->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		this->DXDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		this->DXDevice->SetRenderState(D3DRS_ALPHAREF, 0x60);
		this->DXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);

		if (this->field_830 == 32)
			this->DXDevice->SetRenderState(D3DRS_DITHERENABLE, false);
		else
			this->DXDevice->SetRenderState(D3DRS_DITHERENABLE, true);

		this->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		this->DXDevice->SetRenderState((D3DRENDERSTATETYPE)(D3DRS_EDGEANTIALIAS), false);
		this->DXDevice->SetRenderState((D3DRENDERSTATETYPE)(D3DRS_ZBIAS), 0);
		this->DXDevice->SetRenderState(D3DRS_STENCILENABLE, false);
		this->DXDevice->SetRenderState(D3DRS_WRAP0, 0);
		this->DXDevice->SetRenderState(D3DRS_WRAP1, 0);
		this->DXDevice->SetRenderState(D3DRS_WRAP2, 0);
		this->DXDevice->SetRenderState(D3DRS_WRAP3, 0);
		this->DXDevice->SetRenderState(D3DRS_WRAP4, 0);
		this->DXDevice->SetRenderState(D3DRS_WRAP5, 0);
		this->DXDevice->SetRenderState(D3DRS_WRAP6, 0);
		this->DXDevice->SetTextureStageState(0, D3DTEXTURESTAGESTATETYPE(D3DTSS_MIPFILTER), D3DTEXF_NONE);
	}
	this->StateBlockFunc2(2, &this->StateBlock1, result2);
	this->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

bool FFXI::CYy::CDx::StateBlockFunc(D3DSTATEBLOCKTYPE a2, DWORD* a3)
{
	if (!this->field_909 || !this->field_90A)
	{
		*a3 = -1;
		return true;
	}
	if (*a3)
	{
		return *a3 == -1;
	}
	else
	{
		this->DXDevice->CreateStateBlock(a2, a3);
		if (*a3 == -1)
			return true;
		this->DXDevice->BeginStateBlock();
		return true;
	}
}

void FFXI::CYy::CDx::StateBlockFunc2(int a2, DWORD* a3, char a4)
{
	if (a4 == 1)
		this->DXDevice->EndStateBlock(a3);
	if (*a3 && *a3 != -1)
	{
		this->DXDevice->ApplyStateBlock(*a3);
	}
}

void FFXI::CYy::CDx::SomeFunc(float a2, float a3, int a4)
{
	double v4; // st7
	double v5; // st7

	if (a4)
	{
		if (a4 == 1)
		{
			v4 = a2 * a3;
			if (v4 <= 0.0)
				v4 = 0.0099999998;
			v5 = this->field_174 - v4;
			this->field_174 = v5;
			if (v5 <= 0.0)
				this->field_174 = 0.0;
		}
	}
	else
	{
		this->field_174 = 128.0;
	}
}

void FFXI::CYy::CDx::SetSomeColor(D3DCOLOR a2)
{
	this->SomeColor = a2;
}

void FFXI::CYy::CDx::GetTransform(D3DTRANSFORMSTATETYPE a2, D3DMATRIX* a3)
{
	switch (a2) {
	case D3DTS_VIEW:
		*a3 = this->ViewTransform;
		break;
	case D3DTS_PROJECTION:
		*a3 = this->ProjectionTransform;
		break;
	case D3DTS_WORLD:
		*a3 = this->WorldTransform;
		break;
	}
}

HRESULT FFXI::CYy::CDx::SetTransform(D3DTRANSFORMSTATETYPE a2, D3DMATRIX* a3)
{
	D3DMATRIX* v3{ nullptr };
	switch (a2) {
	case D3DTS_VIEW:
		v3 = &this->ViewTransform;
		break;
	case D3DTS_PROJECTION:
		v3 = &this->ProjectionTransform;
		break;
	case D3DTS_WORLD:
		v3 = &this->WorldTransform;
		break;
	default:
		v3 = &this->ViewTransform;
	}
	memcpy(v3, a3, sizeof(D3DMATRIX));
	return this->DXDevice->SetTransform(a2, a3);
}

HRESULT FFXI::CYy::CDx::SetLight(int a2, D3DLIGHT8* a3)
{
	if (a3->Type == D3DLIGHT_POINT) {
		if (a3->Range < 0.0)
			return D3DERR_INVALIDCALL;
		if (a3->Attenuation2 < 0.0)
			return D3DERR_INVALIDCALL;
	}
	else if (a3->Type == D3DLIGHT_DIRECTIONAL) {
		if (a3->Direction.x == 0.0 && a3->Direction.y == 0.0 && a3->Direction.z == 0.0)
			return D3DERR_INVALIDCALL;
	}
	else
		return D3DERR_INVALIDCALL;
	
	D3DLIGHT8 v5 = *a3;
	if (this->GammaAffectLight(&v5))
		return this->DXDevice->SetLight(a2, &v5);
	
	return this->DXDevice->SetLight(a2, a3);
}

bool FFXI::CYy::CDx::GammaAffectLight(D3DLIGHT8* a2)
{
	if (FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::Fullscreen)
		return false;

	if (a2->Diffuse.r >= 0.8f || a2->Diffuse.g >= 0.8f || a2->Diffuse.b >= 0.8f)
		return false;

	float v17{}, v18{}, v19{};
	this->Gamma.GetLightMultipliers(&v17, &v18, &v19);

	a2->Diffuse.r *= v17;
	a2->Diffuse.g *= v18;
	a2->Diffuse.b *= v19;

	if (a2->Diffuse.r > 1.0)
		a2->Diffuse.r = 1.0;
	if (a2->Diffuse.g > 1.0)
		a2->Diffuse.g = 1.0;
	if (a2->Diffuse.b > 1.0)
		a2->Diffuse.b = 1.0;
	return true;
}

void FFXI::CYy::CDx::DrawVerts(D3DPRIMITIVETYPE primtype, int primcount, const void* VertData, int VertSize)
{
	IDirect3DSurface8* a4{ nullptr };
	IDirect3DTexture8* a2{ nullptr };
	if (this->CheckSurface(&a2, &a4)) {
		throw "NOT IMPLEMENTED";
		int backup = this->field_810;
		this->field_810 = 0;
		//sub
		this->DXDevice->DrawPrimitiveUP(primtype, primcount, VertData, VertSize);
		//sub
		this->field_810 = 1;
		//sub
		this->AddViewportAtOrigin(a2, nullptr, a4);
		this->DXDevice->DrawPrimitiveUP(primtype, primcount, VertData, VertSize);
		//sub
		//sub
		this->field_810 = backup;
	}
	else {
		this->DXDevice->DrawPrimitiveUP(primtype, primcount, VertData, VertSize);
	}
}

void FFXI::CYy::CDx::DoViewportRender(IDirect3DBaseTexture8* a2, D3DCOLOR a3, int a4)
{
	DWORD v5[4];
	D3DVIEWPORT8* port = &this->Viewports[this->ViewportIndex];
	v5[0] = port->X;
	v5[1] = port->Y;
	v5[2] = port->Width;
	v5[3] = port->Height;

	float texverts[4] = { 0.0, 1.0, 0.0, 1.0 };
	this->RenderTexOnViewport(a2, a3, v5, texverts, -1);
}

void FFXI::CYy::CDx::RenderTexOnViewport(IDirect3DBaseTexture8* a2, D3DCOLOR a3, DWORD* a4, float* a5, int a6)
{
	CYy::FVF144Vertex verts[4]{};

	verts[0].X = (double)a4[0] - 0.5;
	verts[1].X = (double)a4[2] - 0.5;
	verts[2].X = verts[0].X;
	verts[3].X = verts[1].X;

	verts[0].Y = (double)a4[1] - 0.5;
	verts[1].Y = verts[0].Y;
	verts[2].Y = (double)a4[3] - 0.5;
	verts[3].Y = verts[2].Y;

	verts[0].Z = 0.0;
	verts[1].Z = 0.0;
	verts[2].Z = 0.0;
	verts[3].Z = 0.0;
	
	verts[0].RHW = 1.0;
	verts[1].RHW = 1.0;
	verts[2].RHW = 1.0;
	verts[3].RHW = 1.0;

	verts[0].DiffuseColor = a3;
	verts[1].DiffuseColor = a3;
	verts[2].DiffuseColor = a3;
	verts[3].DiffuseColor = a3;

	verts[0].TexVertX = a5[0];
	verts[1].TexVertX = a5[1];
	verts[2].TexVertX = a5[0];
	verts[3].TexVertX = a5[1];

	verts[0].TexVertY = a5[2];
	verts[1].TexVertY = a5[2];
	verts[2].TexVertY = a5[3];
	verts[3].TexVertY = a5[3];

	this->DXDevice->SetTexture(0, a2);
	this->DXDevice->SetVertexShader(verts[0].FVF);
	this->DrawVerts(D3DPT_TRIANGLESTRIP, 2, verts, sizeof(verts[0]));
}

void FFXI::CYy::CDx::DrawViewportWithZedd(IDirect3DBaseTexture8* a2, float a3)
{
	CYy::FVF144Vertex a6[4]{};
	D3DVIEWPORT8* viewport = this->GetTopViewport();
	a6[1].X = (float)(int)viewport->Width;
	a6[0].DiffuseColor = 0x80808080;
	a6[1].Z = a3;
	a6[1].DiffuseColor = 0x80808080;
	a6[2].Y = (float)(int)viewport->Height;
	a6[3].X = a6[1].X;
	a6[2].DiffuseColor = 0x80808080;
	a6[3].Z = a3;
	a6[3].DiffuseColor = 0x80808080;
	a6[0].X = 0.0;
	a6[3].Y = a6[2].Y;
	a6[0].Y = 0.0;
	a6[0].Z = a3;
	a6[0].RHW = 1.0;
	a6[0].TexVertX = 0.0;
	a6[0].TexVertY = 0.0;
	a6[1].Y = 0.0;
	a6[1].RHW = 1.0;
	a6[1].TexVertX = 1.0;
	a6[1].TexVertY = 0.0;
	a6[2].X = 0.0;
	a6[2].Z = a3;
	a6[2].RHW = 1.0;
	a6[2].TexVertX = 0.0;
	a6[2].TexVertY = 1.0;
	a6[3].RHW = 1.0;
	a6[3].TexVertX = 1.0;
	a6[3].TexVertY = 1.0;
	this->DXDevice->SetTexture(0, a2);
	this->DXDevice->SetVertexShader(a6[0].FVF);
	this->DrawVerts(D3DPT_TRIANGLESTRIP, 2, a6, sizeof(a6[0]));

}

void FFXI::CYy::CDx::SomeCAcc2Render(CYy::CAcc2* a2, D3DCOLOR a3, DWORD* a4)
{
	if (Globals::g_pCenv->CheckField18()) {
		throw "NOT IMPLEMENTED";
	}
	else {
		this->DXDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		this->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		this->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		this->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
		this->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
		this->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
		this->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		this->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		this->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
		this->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		this->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		this->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		this->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		this->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		this->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		float v19[4];
		v19[0] = 0;
		v19[1] = 1.0;
		v19[2] = 0;
		v19[3] = 1.0;
		this->RenderTexOnViewport(a2->Texture[0], a3, a4, v19, -1);
		this->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		this->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	}
	this->field_810 += 1;
	if (this->field_810 > 1)
		this->field_810 = 0;
}

void FFXI::CYy::CDx::DrawSomeVerts(int a2, int a3)
{
	this->SetRenderStates();
	int Color = a3;

	if (Color != 0x80808080) {
		if ((Color & 0xFF000000) == 0x80000000)
			Color = ((unsigned char)a3 * (((unsigned int)a3 >> 7) & 0x1FE0000)) & 0xFF000000;

		if ((Color & 0xFF000000) <= 0x80000000)
		{
			this->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
			this->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
			Color &= 0xFF000000;
		}
		else {
			this->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			this->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
		}

		SomeBasicVertices[0].DiffuseColor = Color;
		SomeBasicVertices[1].DiffuseColor = Color;
		SomeBasicVertices[2].DiffuseColor = Color;
		SomeBasicVertices[3].DiffuseColor = Color;

		D3DVIEWPORT8* topV = this->GetTopViewport();
		SomeBasicVertices[0].X = (float)topV->X;
		SomeBasicVertices[2].X = (float)topV->X;
		SomeBasicVertices[1].X = (float)topV->Width;
		SomeBasicVertices[3].X = (float)topV->Width;
		SomeBasicVertices[0].Y = (float)topV->Y;
		SomeBasicVertices[1].Y = (float)topV->Y;
		SomeBasicVertices[2].Y = (float)topV->Height;
		SomeBasicVertices[3].Y = (float)topV->Height;
		this->DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		this->DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		this->DXDevice->SetRenderState(D3DRS_LIGHTING, false);
		this->DXDevice->SetRenderState(D3DRS_FOGENABLE, false);
		this->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		this->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		this->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		this->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		this->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		this->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		this->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		this->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		this->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		this->DXDevice->SetVertexShader(SomeBasicVertices[0].FVF);
		this->DrawVerts(D3DPT_TRIANGLESTRIP, 2, SomeBasicVertices, sizeof(SomeBasicVertices[0]));
	}

	this->SetRenderStates();
	

}

void FFXI::CYy::CDx::DrawStream(D3DPRIMITIVETYPE a2, int a3, int a4)
{
	IDirect3DTexture8* v8{ nullptr };
	IDirect3DSurface8* v7{ nullptr };
	if (this->CheckSurface(&v8, &v7) == false) {
		this->DXDevice->DrawPrimitive(a2, a3, a4);
		return;
	}
	throw "not implemented";
}

void FFXI::CYy::CDx::DrawStreamIndexed(D3DPRIMITIVETYPE a2, int a3, int a4, int a5, int a6)
{
	IDirect3DTexture8* v8{ nullptr };
	IDirect3DSurface8* v7{ nullptr };
	if (!this->CheckSurface(&v8, &v7)) {
		this->DXDevice->DrawIndexedPrimitive(a2, a3, a4, a5, a6);
		return;
	}
	throw "not implemented";
}
void FFXI::CYy::CDx::SetActiveVertexShader(DWORD a2) {
	if (a2 == 0) {
		this->field_9FC = 0;
	}
	else if (this->field_9FC != a2) {
		this->DXDevice->SetVertexShader(a2);
		this->field_9FC = a2;
	}
}

CDx::CDx(IDirect3D8* p_interface, Direct3DDevice8* p_device) {
	this->DXInterface = p_interface;
	this->DXDevice = p_device;
	this->ViewportIndex = 0;
	this->field_908 = 0;
	this->field_909 = 0;
	this->field_90A = 0;
	this->field_90C = 0;
	this->field_90E = 0;
	this->field_910 = 0;
	this->field_911 = 0;
	this->field_912 = 0;
	this->field_913 = 0;
	this->field_914 = 0;
	this->field_915 = 0;
	this->field_916 = 0;
	this->field_918 = 0;
	this->FullscreenMultisampleCount = 0;
	this->GPUVendor = 0;
	this->field_921 = 0;
	this->field_92C = 0;
	this->field_92D = 0;
	this->field_92E = 0;
	this->field_92F = 0;
	this->field_930 = 0;
	this->field_931 = 0;
	this->field_934 = 0;
	this->field_938 = 0;
	this->StateBlock1 = 0;
	this->StateBlock2 = 0;
	this->field_810 = 0;
	this->field_814 = 0;
	this->field_818 = 0;
	this->field_81C = 0;
	this->field_820 = 0;
	this->field_824 = 0;
	this->field_828 = 0;
	this->field_82C = 0;
	this->field_9FC = 0;
	this->field_174 = 128.0;
	this->field_178 = 128.0;
	this->field_17C = 128.0;
	this->field_180 = 128.0;
	this->field_184 = 128.0;
	this->field_188 = 128.0;
	this->field_18C = 0x80808080;
}

char FFXI::CYy::CDx::CheckDeviceCaps()
{
	this->DXDevice->GetDeviceCaps(&this->DeviceCaps);
	this->VertexBufferUsage = D3DUSAGE_WRITEONLY;
	this->VertexBufferPool = D3DPOOL_MANAGED;
	if ((this->DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0) {
		this->field_908 = 1;
		this->VertexBufferUsage = D3DUSAGE_WRITEONLY;
		this->VertexBufferPool = D3DPOOL_MANAGED;
	}
	else {
		this->VertexBufferUsage = D3DUSAGE_SOFTWAREPROCESSING;
		this->VertexBufferPool = D3DPOOL_SYSTEMMEM;
	}
	if (this->DeviceCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1)) {
		this->field_909 = 1;
		this->field_90C = 257;
	}
	if (this->DeviceCaps.PixelShaderVersion >= D3DPS_VERSION(1, 1)) {
		this->field_90A = 1;
		this->field_90E = 257;
		if (this->DeviceCaps.PixelShaderVersion >= D3DPS_VERSION(1, 4))
			this->field_90E = 260;
		if (this->DeviceCaps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
			this->field_90E = 512;
	}
	if ((this->DeviceCaps.PrimitiveMiscCaps & D3DPMISCCAPS_BLENDOP) != 0)
		this->field_913 = 1;
	if ((this->DeviceCaps.PrimitiveMiscCaps & D3DPMISCCAPS_TSSARGTEMP) != 0)
		this->field_914 = 1;
	if ((this->DeviceCaps.Caps2 & D3DDEVCAPS_CANBLTSYSTONONLOCAL) != 0)
		this->field_915 = 1;
	this->field_918 = this->DeviceCaps.MaxVertexBlendMatrixIndex;
	if ((this->DeviceCaps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY) != 0) {
		if ((this->DeviceCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC) != 0 && (this->DeviceCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC) != 0 && this->DeviceCaps.MaxAnisotropy >= 2)
			this->field_92C = 1;
	}
	if ((this->DeviceCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP) != 0)
		this->field_921 = 1;
	if ((this->DeviceCaps.TextureCaps & D3DPTEXTURECAPS_POW2) != 0)
		this->field_930 = 1;
	if ((this->DeviceCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) != 0)
		this->field_931 = 1;
	this->field_934 = this->DeviceCaps.MaxTextureWidth;
	this->field_938 = this->DeviceCaps.MaxTextureHeight;
	D3DDISPLAYMODE dcdevice{}, dxinterface {};
	this->DXDevice->GetDisplayMode(&dcdevice);
	this->field_830 = 32;
	switch (dcdevice.Format) {
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
		this->field_830 = 32;
		break;
	case D3DFMT_R5G6B5:
	case D3DFMT_X1R5G5B5:
	case D3DFMT_A1R5G5B5:
		this->field_830 = 16;
		break;
	default:
		break;
	}
	this->DXInterface->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dxinterface);
	this->field_910 = this->DXInterface->CheckDeviceFormat(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		dxinterface.Format,
		NULL,
		D3DRTYPE_TEXTURE,
		D3DFMT_DXT1) >= 0;
	this->field_911 = this->DXInterface->CheckDeviceFormat(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		dxinterface.Format,
		NULL,
		D3DRTYPE_TEXTURE,
		D3DFMT_DXT3) >= 0;
	this->field_912 = this->DXInterface->CheckDeviceFormat(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		dxinterface.Format,
		NULL,
		D3DRTYPE_TEXTURE,
		D3DFMT_P8) >= 0;
	if (this->DXInterface->CheckDeviceMultiSampleType(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		dxinterface.Format,
		false,
		D3DMULTISAMPLE_2_SAMPLES) >= 0)
		this->FullscreenMultisampleCount = 2;
	if (this->DXInterface->CheckDeviceMultiSampleType(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		dxinterface.Format,
		false,
		D3DMULTISAMPLE_4_SAMPLES) >= 0)
		this->FullscreenMultisampleCount = 4;
	if ((this->DeviceCaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0)
		this->field_92D = 1;
	if (((char)(this->DeviceCaps.RasterCaps & 0xFF)) < 0)
		this->field_92E = 1;
	if ((this->DeviceCaps.RasterCaps & D3DPRASTERCAPS_FOGRANGE) != 0)
		this->field_92F = 1;
	this->GPUVendor = 0;
	D3DADAPTER_IDENTIFIER8 ident{};
	this->DXInterface->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 2, &ident);
	if (ident.VendorId == (int)Constants::Enums::GPUVendorID::AMD)
		this->GPUVendor = 1;
	this->field_A00 = -1;
	char FileName[512];
	if (!GetCurrentDirectoryA(sizeof(Globals::g_Str), Globals::g_Str)) {
		for (int i = 0; i < Constants::Values::COUNT_CURSOR; ++i) {
			this->Cursors[i] = LoadCursorA(NULL, MAKEINTRESOURCEA(0x7F00));
		}
	}
	else {
		char* v8 = strrchr(Globals::g_Str, '\\');
		char v7 = 0;
		if (!v8) {
			v8 = strrchr(Globals::g_Str, '/');
			v7 = 1;
		}
		if (v8) {
			if (v8[1]) {
				char* v9 = v8 + strlen(v8) + 1;
				if (v7)
					*(v9 - 1) = '/';
				else
					*(v9 - 1) = '\\';
				*v9 = '\0';
			}
			for (int i = 0; i < Constants::Values::COUNT_CURSOR; ++i) {
				int pos = 0;
				do {
					FileName[pos] = Globals::g_Str[pos];
				} while (Globals::g_Str[pos++]);
				strcat_s(FileName, sizeof(FileName), Globals::g_CursorList[i]);
				HCURSOR thing = LoadCursorFromFileA(FileName);
				this->Cursors[i] = thing;
			}
		}
	}
	this->field_A4C = 0;
	SetCursor(NULL);
	return 1;
}
