#include "CMoOs2.h"
#include "Globals.h"
#include "CDx.h"
#include "StorageLists.h"
#include "CYyOs2VtxBuffer.h"
#include "CMoTaskMng.h"
#include "CXiSkeletonActor.h"
#include "CYyModel.h"
#include "CEnv.h"
#include "CYyDb.h"
#include "CYySkl.h"
#include "CYyTexMng.h"

using namespace FFXI::CYy;

FFXI::Math::WMatrix* CMoOs2::mat_matrix{ nullptr };

unsigned short short1{}, short2{};
UINT DrawOneStrides[5]{};
IDirect3DVertexBuffer8* DrawOneBuffs[5]{};

bool CMoOs2::DrawBasicFlagOne{ false };
bool CMoOs2::DrawBasicFlagTwo{ false };
FFXI::Math::WMatrix CMoOs2::DrawBasicMatrixOne{};
FFXI::Math::WMatrix CMoOs2::DrawBasicMatrixTwo{};

int local_int_array[2] = { 0, 0 };

void SetVertexShader(int a2) {
	if (a2 != 0)
		FFXI::CYy::CDx::instance->DXDevice->SetVertexShader(a2);
}
void FFXI::CYy::CMoOs2::Open()
{
	if (this->Data[0] != 0 && this->Data[0] != 1) {
		//version error
		return;
	}

	if (this->Data[1] != 0)
		return;

	if ((this->Data[2] & 0x7F) == 0) {
		*(CYyOs2VtxBuffer**)(this->Data + 46) = nullptr;
		this->InitTypeZero();
	}
	else if ((this->Data[2] & 0x7F) == 1) {
		this->InitTypeOne();
	}
	else
		return;

	this->Data[1] = 1;
}

void FFXI::CYy::CMoOs2::Close()
{
	if ((this->Data[2] & 0x7F) == 0) {
		//Type 0 cleanup
		CYyOs2VtxBuffer* buf = *(CYyOs2VtxBuffer**)(this->Data + 46);
		if (buf != nullptr) {
			delete buf;
			*(CYyOs2VtxBuffer**)(this->Data + 46) = nullptr;
		}
	}
}

void FFXI::CYy::CMoOs2::InitTypeZero()
{
	*(unsigned short**)(this->Data + 6) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 6));
	*(unsigned short**)(this->Data + 12) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 12));
	*(unsigned short**)(this->Data + 18) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 18));
	*(unsigned short**)(this->Data + 24) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 24));
	*(unsigned short**)(this->Data + 30) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 30));
	*(unsigned short**)(this->Data + 36) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 36));

	if ((this->Data[4] & 2) != 0)
		return;

	char* mem = StorageLists::instance->Get(sizeof(CYyOs2VtxBuffer), FFXI::Constants::Enums::MEM_MODE::Ex);
	if (mem == nullptr)
		return;
	
	CMoTaskMng::DeleteThisTask = true;
	CYyOs2VtxBuffer* buf = new (mem) CYyOs2VtxBuffer((CMoOs2**)this->PreHeader.PointerToThisInRPL);
	*(CYyOs2VtxBuffer**)(this->Data + 46) = buf;
	
	this->Data[4] |= 2;
}

void FFXI::CYy::CMoOs2::InitTypeOne()
{
	*(unsigned short**)(this->Data + 6) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 6));
	*(unsigned short**)(this->Data + 12) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 12));
	*(unsigned short**)(this->Data + 18) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 18));
	*(unsigned short**)(this->Data + 24) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 24));
	*(unsigned short**)(this->Data + 30) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 30));
	*(unsigned short**)(this->Data + 36) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 36));
	
	*(unsigned short**)(this->Data + 46) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 46));
	*(unsigned short**)(this->Data + 76) = (unsigned short*)(this->Data + 2 * *(int*)(this->Data + 76));
}

void VBSetup(int a1, IDirect3DVertexBuffer8* a2, int a3) {
	DrawOneBuffs[a1] = a2;
	DrawOneStrides[a1] = a3;
	if (a2 != nullptr) {
		FFXI::CYy::CDx::instance->DXDevice->SetStreamSource(a1, a2, a3);
	}
}
//Breakup funcs for DrawBasic
void Draw54Init(D3DPRIMITIVETYPE a1, CMoOs2* _this, CYyOs2VtxBuffer* v10, int v9, int v26) {
	FFXI::CYy::CDx::instance->DXDevice->SetVertexShader(v10->ShaderHandle);
	
	VBSetup(0, v10->field_44, 12);
	VBSetup(1, v10->field_48, 12);
	VBSetup(2, v10->VertexBuffer, 8);
	VBSetup(3, nullptr, 0);

	FFXI::CYy::CDx::instance->DXDevice->SetIndices(v10->IndexBuffer, 0);
	FFXI::CYy::CDx::instance->DrawStreamIndexed(a1, 0, v10->field_60 + short1 + short2, v9, v26);
	CYyModel::maybe_polys_drawn += v26;

	if ((_this->Data[4] & 1) != 0) {
		VBSetup(0, v10->field_50, 12);
		VBSetup(1, v10->field_54, 12);
		FFXI::CYy::CDx::instance->DrawStreamIndexed(a1, 0, v10->field_60 + short1 + short2, v9, v26);
		CYyModel::maybe_polys_drawn += v26;
	}
}

bool PrepDrawBasicOne(int* a1, D3DCOLOR a2, FFXI::Math::WMatrix* a3) {
	FFXI::Math::WMatrix* v4{};
	//sub //TODO
	return false;
}

bool PrepDrawBasicTwo(CYyModel::tex_struct* a0, int* a1, D3DCOLOR a2, FFXI::Math::WMatrix* a3) {
	FFXI::Math::WMatrix* v6{};
	if (a0->field_4 != nullptr) {
		return true;
	}

	return false;
}

void Draw5453(CMoOs2* _this, CYyModel* a3, CYyOs2VtxBuffer* v10, D3DCOLOR color) 
{
	if (CMoOs2::DrawBasicFlagOne == true) {
		if (PrepDrawBasicOne(local_int_array, color , &CMoOs2::DrawBasicMatrixOne) == true) {
			exit(0x1002DF0A);
		}
	}
	
	if (CMoOs2::DrawBasicFlagTwo == true) {
		if (PrepDrawBasicTwo(&a3->field_90, local_int_array, color, &CMoOs2::DrawBasicMatrixTwo) == true) {
			exit(0x1002E042);
		}
	}

	if (a3->SubStruct1.PrepareToRender(local_int_array, color) == true) {
		exit(0x1002E164);
	}
}

void Draw54(CMoOs2* _this, CYyModel* a3, CYyOs2VtxBuffer* v10, D3DCOLOR color)
{
	if (CMoOs2::DrawBasicFlagOne == true) {
		if (PrepDrawBasicOne(local_int_array, color, &CMoOs2::DrawBasicMatrixOne) == true) {
			exit(0x1002DF0A);
		}
	}

	if (CMoOs2::DrawBasicFlagTwo == true) {
		if (PrepDrawBasicTwo(&a3->field_90, local_int_array, color, &CMoOs2::DrawBasicMatrixTwo) == true) {
			exit(0x1002E042);
		}
	}

	if (a3->SubStruct1.PrepareToRender(local_int_array, color) == true) {
		exit(0x1002E164);
	}
}

void FFXI::CYy::CMoOs2::DrawBasic(CXiActor* a2, CYyModel* a3, CYyModelBase* a4, float a5, int* a6, int* a7)
{
	CMoOs2::mat_matrix = FFXI::CYy::CYySkl::g_mat;
	*a6 = 0;
	*a7 = 0;
	unsigned short* sptr = *(unsigned short**)(this->Data + 18);
	short1 = sptr[0];
	short2 = sptr[1];
	CYyOs2VtxBuffer::DataPointer1 = *(unsigned short**)(this->Data + 24);
	CYyOs2VtxBuffer::DataPointer2 = CYyOs2VtxBuffer::DataPointer1 + 2 * *(unsigned short*)(this->Data + 18);
	CYyOs2VtxBuffer* v10 = *(CYyOs2VtxBuffer**)(this->Data + 46);
	if (v10->IndexBuffer == nullptr)
		return;

	if (v10->VertexBuffer == nullptr)
		return;

	if (CXiSkeletonActor::somebool == true
		|| a3->field_B8 != 1) {
		if (v10->field_44 == nullptr)
			return;

		if (v10->field_48 == nullptr)
			return;

		while (v10->field_44->Lock(0, 12 * v10->SomeCount, (BYTE**) &v10->field_8E, D3DLOCK_DISCARD) != D3D_OK)
			Sleep(0);

		v10->field_92 = 0;
		while (v10->field_48->Lock(0, 12 * v10->SomeCount, (BYTE**) &v10->field_92, D3DLOCK_DISCARD) != D3D_OK)
			Sleep(0);

		if ((this->Data[4] & 1) != 0) {
			if (v10->field_50 == 0 || v10->field_54 == 0) {
				while (v10->field_44->Unlock() != D3D_OK)
					Sleep(0);
				while (v10->field_48->Unlock() != D3D_OK)
					Sleep(0);

				return;
			}

			while (v10->field_50->Lock(0, 12 * v10->SomeCount, (BYTE**)&v10->field_96, D3DLOCK_DISCARD) != D3D_OK)
				Sleep(0);

			v10->field_9A = 0;
			while (v10->field_54->Lock(0, 12 * v10->SomeCount, (BYTE**)&v10->field_9A, D3DLOCK_DISCARD) != D3D_OK)
				Sleep(0);
			
			this->FillVertexBuffers();
			a3->field_B8 = 1;
			while (v10->field_44->Unlock() != D3D_OK)
				Sleep(0);
			while (v10->field_48->Unlock() != D3D_OK)
				Sleep(0);

			if ((this->Data[4] & 1) != 0)
			{
				while (v10->field_50->Unlock() != D3D_OK)
					Sleep(0);
				while (v10->field_54->Unlock() != D3D_OK)
					Sleep(0);
			}
		}
	}

	CDx* cdx = FFXI::CYy::CDx::instance;
	cdx->DXDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0);
	cdx->DXDevice->SetTexture(0, NULL);
	local_int_array[0] = 0;
	local_int_array[1] = 0;
	cdx->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	cdx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
	cdx->DXDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	cdx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE4X);
	cdx->DXDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	cdx->DXDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	cdx->DXDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);

	switch (this->Data[3]) {
	case 2u:
		a3->field_A9 = 1;
		break;
	case 3u:
		a3->field_A9 = 1;
		a3->field_AA = 1;
		break;
	case 4u:
		a3->field_A9 = 1;
		a3->field_AA = 1;
		a3->field_AB = 1;
		break;
	case 5u:
		a3->field_A9 = 1;
		a3->field_AA = 1;
		a3->field_AB = 1;
		a3->field_B0 = 1;
		a3->field_AC = 1;
		break;
	case 6u:
		a3->field_A9 = 1;
		a3->field_AA = 1;
		a3->field_AB = 1;
		a3->field_B0 = 1;
		break;
	case 0x12u:
		a3->field_AD = 1;
		break;
	case 0x22u:
		a3->field_AF = 1;
		break;
	case 0x32u:
		a3->field_AE = 1;
		break;
	default:
		break;
	}

	unsigned short* v32 = *(unsigned short**)(this->Data + 6);
	bool v29{ false };
	int v9 = 0;
	D3DCOLOR color = 0; //sub //TODO
	while (v32[0] != 0xFFFF) {
		if ((v32[0] & 0x8000) == 0) {
			switch (v32[0]) {
			case 0x5453:
				if (v29 == false) {
					if (v10->field_68 != 0) {
						Draw54Init(D3DPT_TRIANGLESTRIP, this, v10, v9, v32[1]);
					}
					Draw5453(this, a3, v10, color);
				}

				v9 += v32[1] + 2;
				*a7 += v32[1] + 2;
				v32 += 5 * v32[1] + 12;
				break;
			case 0x4353:
				v32 += v32[1] + 6;
				break;
			case 0x43:
				v32 += 5 * v32[1] + 2;
				break;
			case 0x54:
				if (v29 == false) {
					if (v10->field_68 != 0) {
						Draw54Init(D3DPT_TRIANGLELIST, this, v10, v9, v32[1]);
					}
					Draw54(this, a3, v10, color);
				}
				*a6 += 3 * v32[1];
				v9 += 3 * v32[1];
				//sub //TODO questionable, should this be below draw54init?
				v32 += 15 * v32[1] + 2;
			}
		}
		else {
			int code = v32[0] & 0xFF;
			switch (code >> 4) {
			case 0:
			{
				CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_COLORARG1, 2);
				CDx::instance->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2);
				if ((code & 1) == 0) {
					v32[0] |= 1;
					*(CYyTex**)(v32 + 1) = CYyDb::g_pCYyDb->pCYyTexMng->FindD3sTexUnder((char*)v32 + 2, this);
				}
				CYyTex* sometex = *(CYyTex**)(v32 + 1);
				if (sometex != nullptr) {
					exit(0x1002D824);
				}
				v32 += 9;
				break;
			}
			case 1:
				//sub //todo light
				v32 += 23;
				break;
			default:
				exit(0x01010101);
			}
		}
	}
}

//breakup functions for FillVertexBuffers
inline void FillBufferOne(CMoOs2* _this) {
	exit(0x1002E3CD);
}

void FillBufferTwo(CMoOs2* _this) {
	if (FFXI::CYyDb::g_pCYyDb->SomeStatic != 0) {
		exit(0x1002E421);
		return;
	}

	CYyOs2VtxBuffer* buff = *((CYyOs2VtxBuffer**)(_this->Data + 46));
	unsigned short* sp24 = *(unsigned short**)(_this->Data + 24);
	unsigned short* sp12 = *(unsigned short**)(_this->Data + 12);

	int v14{}, v10 {-1};
	for (unsigned short i = 0; i < short1; ++i) {
		unsigned short v12 = sp24[2 * i];
		if ((_this->Data[2] & 0x80) != 0) {
			v14 = 16 * sp12[2 * (v12 & 0x7F)];
			if ((_this->Data[4] & 1) != 0) {
				v10 = 16 * sp12[2 * ((v12 >> 7) & 0x7F)];
			}
		}
		else {
			v14 = 16 * (v12 & 0x7F);
			if ((_this->Data[4] & 1) != 0) {
				v10 = (v12 >> 3) & 0x7F0;
			}
		}

		//CMoOs2::mat_matrix->Vec3TransformDrop4(buff->field_8E[i], buff->field_6E[2 * i]);
		//CMoOs2::mat_matrix->Vec3TransformDrop4(buff->field_92[i], buff->field_6E[2 * i + 1]);
	}

	for (int i = 0; i < short2; ++i) {

	}

	for (int i = 0; i < buff->field_60; ++i) {

	}
}

void FFXI::CYy::CMoOs2::FillVertexBuffers()
{
	if (Globals::g_pCenv->field_9 != 0 || Globals::g_pCenv->field_C != 0) {
		FillBufferOne(this);
	}
	else {
		FillBufferTwo(this);
	}
}
