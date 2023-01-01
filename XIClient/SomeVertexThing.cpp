#include "SomeVertexThing.h"
#include "CYyDb.h"
#include "CMoDx.h"
#include "CDx.h"
#include "Globals.h"
#include "SVTSStorage.h"
FFXI::CYy::SomeVertexThing::SomeVertexThing()
{
	this->field_4[0].field_0 = nullptr;
	this->field_4[1].field_0 = nullptr;

	this->field_44 = 0;
	this->field_45 = 0;
}

FFXI::CYy::SomeVertexThing::~SomeVertexThing()
{
	this->Clean();
}

void FFXI::CYy::SomeVertexThing::Init(int a2, unsigned int a3)
{
	this->Clean();
	this->field_44 = 1;

	for (int i = 0; i < this->field_44; ++i) {
		SomeVertexThingSub* sub = this->field_4 + i;

		sub->field_4 = 0;
		sub->FVF = a3;
		sub->field_18 = a2;
		sub->Stride = Globals::GetStrideFromFVF(a3);
		SomeVertexThingSub* stored = CMoDx::svtsstorage->TryWithdraw(sub);
		if (stored != nullptr) {
			*sub = *stored;
		}
		else {
			FFXI::CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->CreateVertexBuffer(
				a2 * sub->Stride,
				D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
				a3,
				D3DPOOL_DEFAULT,
				&sub->field_0 );
		}
	}
}

void FFXI::CYy::SomeVertexThing::Clean()
{
	for (int i = 0; i < sizeof(this->field_4) / sizeof(this->field_4[0]); ++i) {
		SomeVertexThingSub* sub = this->field_4 + i;

		if (sub->field_0 != nullptr) {
			if (CMoDx::svtsstorage->TryDeposit(sub) == false) {
				sub->field_0->Release();
			}

			sub->field_0 = nullptr;
		}
	}
}

void FFXI::CYy::SomeVertexThing::UpdateSubValues(int a2)
{
	if (this->field_4[0].field_0 == nullptr)
		return;

	SomeVertexThingSub* sub = this->field_4;
	sub->field_C = a2 * sub->Stride;
	sub->field_8 = sub->field_4;
	sub->field_10 = 0;
	if (a2 + sub->field_4 > sub->field_18) {
		sub->field_8 = 0;
		sub->field_10 = 0x2000;
	}
	sub->field_4 = a2 * sub->field_8;
}

BYTE* FFXI::CYy::SomeVertexThing::LockBuffer()
{
	SomeVertexThingSub* sub = this->field_4;
	if (sub->field_0 == nullptr)
		return nullptr;

	int flags = sub->field_10;
	if (flags == 0)
		flags = D3DLOCK_NOOVERWRITE;
	
	BYTE* data{ nullptr };
	sub->field_0->Lock(sub->field_8 * sub->Stride, sub->field_C, &data, flags);
	return data;
}

void FFXI::CYy::SomeVertexThing::UnlockBuffer()
{
	if (this->field_4[0].field_0 != nullptr) {
		this->field_4[0].field_0->Unlock();
	}
}

void FFXI::CYy::SomeVertexThing::Render(int a2, int a3, D3DPRIMITIVETYPE a4, int a5, int a6)
{
	SomeVertexThingSub* sub = this->field_4;
	if (sub->field_0 == nullptr)
		return;

	if (a5 == 0) {
		CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetVertexShader(sub->FVF);
		CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetStreamSource(0, sub->field_0, sub->Stride);
	}
	else {
		int stride = Globals::GetStrideFromFVF(a5);
		CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetVertexShader(a5);
		CYyDb::g_pCYyDb->g_pCMoDx->field_EBC->SetStreamSource(0, sub->field_0, stride);
	}

	CYyDb::g_pCYyDb->g_pCMoDx->field_DC8->DrawStream(a4, a2 + sub->field_8, a3);
}

void FFXI::CYy::SomeVertexThing::Toggle45()
{
	this->field_45 = 1 - this->field_45;
}
