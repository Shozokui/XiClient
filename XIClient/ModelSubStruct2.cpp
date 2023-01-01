#include "ModelSubStruct2.h"
#include "Globals.h"
#include "CDx.h"
#include "CEnv.h"
using namespace FFXI::CYy;

bool ModelSubStruct2::some_flag{ false };
D3DCOLOR ModelSubStruct2::some_color{};
void FFXI::CYy::ModelSubStruct2::SetStatics(D3DCOLOR a2, bool a3)
{
	some_color = a2;
	some_flag = a3;
}
FFXI::CYy::ModelSubStruct2::ModelSubStruct2()
{
	this->field_8[0] = Globals::g_pSomeTexture1;
	if (this->field_8[0] != nullptr)
		this->field_8[0]->AddRef();

	this->field_8[1] = Globals::g_pSomeTexture3;
	if (this->field_8[1] != nullptr)
		this->field_8[1]->AddRef();

	this->field_8[2] = Globals::g_pSomeTexture2;
	if (this->field_8[2] != nullptr)
		this->field_8[2]->AddRef();

	this->field_8[3] = Globals::g_pSomeTexture4;
	if (this->field_8[3] != nullptr)
		this->field_8[3]->AddRef();

	this->field_4 = false;
	ModelSubStruct2::some_flag = false;
}

FFXI::CYy::ModelSubStruct2::~ModelSubStruct2()
{
	for (int i = 0; i < sizeof(this->field_8) / sizeof(this->field_8[0]); ++i) {
		if (this->field_8[i] != nullptr) {
			this->field_8[i]->Release();
			this->field_8[i] = nullptr;
		}
	}
}

void FFXI::CYy::ModelSubStruct2::PrepareViewport()
{
	if (this->field_8[0] == nullptr)
		return;

	if (ModelSubStruct2::some_flag == false) {
		if ((ModelSubStruct2::some_color & 0xFF000000) == 0x80000000)
			return;
	}

	if (this->field_4 == true) {
		if (FFXI::CYy::CDx::instance->ViewportIndex == 0) {
			FFXI::CYy::CDx::instance->AddViewportAtOrigin(this->field_8[0], this->field_8[2], FFXI::CYy::CDx::instance->StencilDepthSurface);
		}
		else {
			FFXI::CYy::CDx::instance->AddViewportAtOrigin(this->field_8[0], this->field_8[2], FFXI::CYy::CDx::instance->Stencils[FFXI::CYy::CDx::instance->ViewportIndex]);
		}
	}
	else {
		if (Globals::g_pCenv->CheckField18() == true) {
			FFXI::CYy::CDx::instance->AddViewportAtOrigin(this->field_8[1], this->field_8[3], nullptr);
			FFXI::CYy::CDx::instance->ClearViewport(0, nullptr, 1, 0, 1.0, 0);
			FFXI::CYy::CDx::instance->RevertStage();
		}
		if (FFXI::CYy::CDx::instance->ViewportIndex == 0) {
			FFXI::CYy::CDx::instance->AddViewportAtOrigin(this->field_8[0], this->field_8[2], FFXI::CYy::CDx::instance->StencilDepthSurface);
		}
		else {
			FFXI::CYy::CDx::instance->AddViewportAtOrigin(this->field_8[0], this->field_8[2], FFXI::CYy::CDx::instance->Stencils[FFXI::CYy::CDx::instance->ViewportIndex]);
		}
		FFXI::CYy::CDx::instance->ClearViewport(0, nullptr, 1, 0, 1.0, 0);
		this->field_4 = true;
	}
}

void FFXI::CYy::ModelSubStruct2::CleanViewport()
{
	if (this->field_8[0] == nullptr)
		return;

	if (ModelSubStruct2::some_flag == false) {
		if ((ModelSubStruct2::some_color & 0xFF000000) == 0x80000000)
			return;
	}

	FFXI::CYy::CDx::instance->RevertStage();
}

void FFXI::CYy::ModelSubStruct2::DrawSomething()
{
	if (this->field_8[0] == nullptr)
		return;

	if (this->field_4 == false)
		return;

	if (ModelSubStruct2::some_flag == false) {
		if ((ModelSubStruct2::some_color & 0xFF000000) == 0x80000000) {
			return;
		}
	}

	exit(0x1001C5C3);
}
