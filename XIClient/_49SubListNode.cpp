#define WIN32_LEAN_AND_MEAN
#include "_49SubListNode.h"
#include "Globals.h"
#include "CDx.h"
#include "CYyTexMng.h"
#include "TkManager.h"
#include "CYyTex.h"
#include "d3d8to9/d3d8.hpp"
#include "CYyDb.h"
#include <string>
#include "WMatrix.h"
using namespace FFXI::CTk;

FFXI::CTk::_49SubListNode::_49SubListNode()
{
	memset(this->field_32, 0, sizeof(this->field_32));
	this->Tex = nullptr;
}

void _49SubListNode::Fill(void** Data) {
	int v5 = 4;

	int vertCount = sizeof(this->Verts) / sizeof(this->Verts[0]);
	for (int i = 0; i < vertCount; ++i) {
		Globals::CopyAdvance(this->VertX + i, Data, sizeof(this->VertX[0]));
		Globals::CopyAdvance(this->VertY + i, Data, sizeof(this->VertY[0]));
	}
	
	Globals::CopyAdvance(&this->field_8, Data, sizeof(this->field_8));
	Globals::CopyAdvance(&this->field_A, Data, sizeof(this->field_A));
	Globals::CopyAdvance(&this->field_C, Data, sizeof(this->field_C));
	Globals::CopyAdvance(&this->field_E, Data, sizeof(this->field_E));
	Globals::CopyAdvance(&this->field_1C, Data, sizeof(this->field_1C));

	for (int i = 0; i < vertCount; ++i) {
		Globals::CopyAdvance(this->VertReds + i, Data, 1);
		if (this->VertReds[i] != 0xFF)
			++this->VertReds[i];
		Globals::CopyAdvance(this->VertGreens + i, Data, 1);
		if (this->VertGreens[i] != 0xFF)
			++this->VertGreens[i];
		Globals::CopyAdvance(this->VertBlues + i, Data, 1);
		if (this->VertBlues[i] != 0xFF)
			++this->VertBlues[i];
		Globals::CopyAdvance(this->VertAlphas + i, Data, 1);
		if (this->VertAlphas[i] && this->VertAlphas[i] != 0xFF) {
			++this->VertAlphas[i];
			float v11 = (float)this->VertAlphas[i] * 1.5f;
			if (v11 > 255.0)
				v11 = 255.0;
			this->VertAlphas[i] = (long long)(v11 + 0.5);
		}
	}

	Globals::CopyAdvance(&this->field_2D, Data, 1);
	Globals::CopyAdvance(&this->field_2E, Data, 1);
	Globals::CopyAdvance(&this->field_2F, Data, 1);
	Globals::CopyAdvance(&this->field_30, Data, 1);
	Globals::CopyAdvance(&this->field_32, Data, 16);
	this->Verts[3].Z = 0;
	this->Verts[2].Z = 0;
	this->Verts[1].Z = 0;
	this->Verts[0].Z = 0;
	this->Verts[3].RHW = 1.0;
	this->Verts[2].RHW = 1.0;
	this->Verts[1].RHW = 1.0;
	this->Verts[0].RHW = 1.0;

	for (int i = 0; i < vertCount; ++i) {
		this->Verts[i].DiffuseColor = this->VertAlphas[i];
		this->Verts[i].DiffuseColor <<= 8;
		this->Verts[i].DiffuseColor |= this->VertReds[i];
		this->Verts[i].DiffuseColor <<= 8;
		this->Verts[i].DiffuseColor |= this->VertGreens[i];
		this->Verts[i].DiffuseColor <<= 8;
		this->Verts[i].DiffuseColor |= this->VertBlues[i];
		
	}
	this->TexInit();
}

void FFXI::CTk::_49SubListNode::TexInit()
{
	_strlwr_s(this->field_32);
	this->Tex = CYyDb::g_pCYyDb->pCYyTexMng->GetByUnique(this->field_32);
	if (!this->Tex) return;

	if ((this->field_1C & 1) != 0) {
		this->Verts[2].TexVertX = (double)(this->field_C + this->field_8) / this->Tex->ImageWidth;
		this->Verts[3].TexVertX = (double)(this->field_C) / this->Tex->ImageWidth;
	}
	else {
		this->Verts[2].TexVertX = (double)this->field_C / this->Tex->ImageWidth;
		this->Verts[3].TexVertX = (double)(this->field_C + this->field_8) / this->Tex->ImageWidth;
	}

	this->Verts[0].TexVertX = this->Verts[2].TexVertX;
	this->Verts[1].TexVertX = this->Verts[3].TexVertX;

	if ((this->field_1C & 2) == 0) {
		this->Verts[1].TexVertY = (double)this->field_E / this->Tex->ImageHeight;
		this->Verts[3].TexVertY = (double)(this->field_E + this->field_A) / this->Tex->ImageHeight;
	}
	else {
		this->Verts[1].TexVertY = (double)(this->field_E + this->field_A) / this->Tex->ImageHeight;
		this->Verts[3].TexVertY = (double)this->field_E / this->Tex->ImageHeight;
	}

	this->Verts[0].TexVertY = this->Verts[1].TexVertY;
	this->Verts[2].TexVertY = this->Verts[3].TexVertY;
	
}

void FFXI::CTk::_49SubListNode::Draw(int a2, int a3, float a4, float a5, D3DCOLOR a6, int a7)
{
	if (this->Tex) {
		if (this->Tex->RegTex)
			FFXI::CYy::CDx::instance->DXDevice->SetTexture(0, this->Tex->RegTex);
		else
			FFXI::CYy::CDx::instance->DXDevice->SetTexture(0, this->Tex->CubeTex);
		
	}
	else {
		_strlwr_s(this->field_32, sizeof(this->field_32));
		this->Tex = CYyDb::g_pCYyDb->pCYyTexMng->GetByUnique(this->field_32);
		if (!this->Tex) {
			return;
		}
	}

	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, 1);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, 1);
	
	const int vertCount = sizeof(this->Verts) / sizeof(this->Verts[0]);
	D3DCOLOR colbackup[vertCount];
	float TexXbackup[vertCount];
	float TexYbackup[vertCount];

	for (int i = 0; i < vertCount; ++i) {
		colbackup[i] = this->Verts[i].DiffuseColor;
		TexXbackup[i] = this->Verts[i].TexVertX;
		TexYbackup[i] = this->Verts[i].TexVertY;

		this->Verts[i].DiffuseColor = Globals::ColorMult(colbackup[i], a6);

		double x = (double)this->VertX[i] * a4;
		double y = (double)this->VertY[i] * a5;

		this->Verts[i].X = (float)a2 + x - 0.5;
		this->Verts[i].Y = (float)a3 + y - 0.5;
	}

	float v67 = (double)(TkManager::g_CTkMenuMng.UIXRes - TkManager::g_CTkMenuMng.field_7C) - 1.0;
	float v69 = (double)(TkManager::g_CTkMenuMng.UIYRes - TkManager::g_CTkMenuMng.field_7E) - 1.0;
	if (this->Verts[0].X < 0.0) {
		double v20 = this->Verts[1].X - this->Verts[0].X;
		double v22 = v20;
		if (v20 < 0.0f)
			v22 = -v20;
		if (v22 < 1.0)
			goto LABEL_35;
		double v25 = this->Verts[1].X + 0.5;
		this->Verts[0].X = -0.5;
		this->Verts[2].X = -0.5;
		double v26 = v25 / v20 * (this->Verts[1].TexVertX - this->Verts[0].TexVertX);
		this->Verts[0].TexVertX = this->Verts[1].TexVertX - v26;
		this->Verts[2].TexVertX = this->Verts[3].TexVertX - v26;
	}
	
	if (this->Verts[1].X > v67) {
		double v32 = this->Verts[1].X - this->Verts[0].X;
		double v34 = v32;
		if (v32 < 0.0f)
			v34 = -v32;
		if (v34 < 1.0)
			goto LABEL_35;
		double v37 = v67 - 0.5;
		this->Verts[1].X = v37;
		this->Verts[3].X = v37;
		double v38 = (v37 - this->Verts[0].X) / v32 * (this->Verts[1].TexVertX - this->Verts[0].TexVertX) + this->Verts[0].TexVertX;
		this->Verts[1].TexVertX = v38;
		this->Verts[3].TexVertX = v38;
	}
	if (this->Verts[0].Y < 0.0)
	{
		double v39 = this->Verts[2].Y - this->Verts[0].Y;
		double v41 = v39;
		if (v39 < 0.0f)
			v41 = -v39;
		if (v41 < 1.0)
			goto LABEL_35;
		double v44 = Verts[2].Y + 0.5;
		this->Verts[0].Y = -0.5;
		this->Verts[1].Y = -0.5;
		double v45 = v44 / v39 * (this->Verts[2].TexVertY- this->Verts[0].TexVertY);
		this->Verts[0].TexVertY = this->Verts[2].TexVertY - v45;
		this->Verts[1].TexVertY = this->Verts[3].TexVertY - v45;
	}
	if (this->Verts[2].Y > v69) {
		double v49 = this->Verts[2].Y - this->Verts[0].Y;
		if (v49 < 0.0 && -v49 < 1.0)
			goto LABEL_35;
		
		double v54 = v69 - 0.5;
		this->Verts[2].Y = v54;
		this->Verts[3].Y = v54;

		double v55 = (v54 - this->Verts[0].Y) / v49 * (this->Verts[2].TexVertY - this->Verts[0].TexVertY) + this->Verts[0].TexVertY;
		this->Verts[2].TexVertY = v55;
		this->Verts[3].TexVertY = v55;
	}

	if (a7) {
		this->RenderTex(this->Verts, 1, a7 & 3, (a7 >> 2) & 3, (a7 >> 6) & 3);
	}
	else if (this->field_2D) {
		this->RenderTex(this->Verts, 1, this->field_2E, this->field_2F, this->field_30);;
	}
	else {
		this->RenderTex(this->Verts, this->field_2D, this->field_2E, this->field_2F, this->field_30);
	}
LABEL_35:


	for (int i = 0; i < vertCount; ++i)
	{
		this->Verts[i].DiffuseColor = colbackup[i];
		this->Verts[i].TexVertX = TexXbackup[i];
		this->Verts[i].TexVertY = TexYbackup[i];
	}

	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, 3);
	FFXI::CYy::CDx::instance->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, 3);
}

void FFXI::CTk::_49SubListNode::DrawRotate(int a2, int a3, FFXI::Math::WMatrix* a4, D3DCOLOR a5, int a6)
{
	IDirect3DBaseTexture8* drawTex{ nullptr };
	if (this->Tex) {
		if (this->Tex->RegTex)
			drawTex = this->Tex->RegTex;
		else
			drawTex = this->Tex->CubeTex;
		FFXI::CYy::CDx::instance->DXDevice->SetTexture(0, drawTex);
	}
	else {
		_strlwr_s(this->field_32, sizeof(this->field_32));
		this->Tex = CYyDb::g_pCYyDb->pCYyTexMng->GetByUnique(this->field_32);
		if (!this->Tex)
			return;
	}

	D3DCOLOR colorBackups[4];
	for (int i = 0; i < sizeof(this->Verts) / sizeof(this->Verts[0]); ++i) {
		colorBackups[i] = this->Verts[i].DiffuseColor;

		this->Verts[i].DiffuseColor = Globals::ColorMult(this->Verts[i].DiffuseColor, a5);
		this->Verts[i].Z = 1.0;
		this->Verts[i].X = this->VertX[i];
		this->Verts[i].Y = this->VertY[i];
		a4->Vec3TransformNormalSelf((D3DXVECTOR3*)&this->Verts[i].X);
		this->Verts[i].X += ((double)a2 - 0.5);
		this->Verts[i].Y += ((double)a3 - 0.5);
		this->Verts[i].Z = 0;

	}

	if (a6)
		this->RenderTex(this->Verts, 1, a6 & 3, (a6 >> 2) & 3, (a6 >> 6) & 3);
	else 
		this->RenderTex(this->Verts, this->field_2D, this->field_2E, this->field_2F, this->field_30);
	

	for (int i = 0; i < sizeof(this->Verts) / sizeof(this->Verts[0]); ++i) {
		this->Verts[i].DiffuseColor = colorBackups[i];
	}

}

void FFXI::CTk::_49SubListNode::RenderTex(const void* a2, int a3, int a4, int a5, int a6)
{
	FFXI::CYy::CDx* CDx = FFXI::CYy::CDx::instance;
	int v6 = 0;

	if (!a3) {
		CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
	else {
		CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		if (CDx->field_913)
			CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		if (a4 == 0) {
			if (a5 == 0) {
				if (!a6 || a6 == 1 || a6 == 2)
					;//Print some debug message
			}
			else if (a5 == 1) {
				if (a6 == 0) {
					if (!CDx->field_913)
						CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
					else {
						CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
						CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
						CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
					}
				}
				else if (a6 == 1) {
					CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				}
				else if (a6 == 2) {
					;//Print some debug message
				}
			}
			else if (a5 == 2) {
				if (a6 == 0) {
					CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
				}
				else if (a6 == 1) {
					CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
				}
				else if (a6 == 2) {
					CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
				}
			}
		}
		else if (a4 == 1) {
			if (a5 == 0) {
				if (a6 == 0) {
					CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
					CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
				}
				else if (a6 == 1) {
					if (CDx->field_913) {
						CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
						CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
					}
					else {
						CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
						CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
						CDx->DXDevice->SetTexture(0, nullptr);

						const int vertCount = sizeof(this->Verts) / sizeof(this->Verts[0]);
						D3DCOLOR backups[vertCount];

						for (int i = 0; i < vertCount; ++i) {
							backups[i] = this->Verts[i].DiffuseColor;
							this->Verts[i].DiffuseColor = 0xFFFFFFFF;
						}

						CDx->DrawVerts(D3DPT_TRIANGLESTRIP, 2, this->Verts, sizeof(this->Verts[0]));

						for (int i = 0; i < vertCount; ++i) {
							this->Verts[i].DiffuseColor = backups[i];
						}

						CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);

						IDirect3DBaseTexture8* texture = this->Tex->RegTex;
						if (!texture)
							texture = this->Tex->CubeTex;

						CDx->DXDevice->SetTexture(0, texture);
						CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
						v6 = 1;
					}
				}
				else if (a6 == 2) {
					;//print some debug message
				}
			}
			else if (a5 == 1) {
				if (a6 == 0) {
					CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
				}
				else if (a6 == 1 || a6 == 2) {
					//print some debug message
				}
			}
			else if (a5 == 2) {
				if (a6 == 0) {
					CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
					CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
				}
				else if (a6 == 1) {
					CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
					CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
				}
				else if (a6 == 2) {
					CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
					CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
				}
			}
		}
		else if (a4 == 2) {
			if (a5 == 0) {
				if (a6 == 0) {
					;//print some debug message
				}
				else if (a6 == 1) {
					if (CDx->field_913) {
						CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
						CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
					}
					else {
						CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
						CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
						CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
						CDx->DXDevice->SetTexture(0, nullptr);

						const int vertCount = sizeof(this->Verts) / sizeof(this->Verts[0]);
						D3DCOLOR backups[vertCount];

						for (int i = 0; i < vertCount; ++i) {
							backups[i] = this->Verts[i].DiffuseColor;
							this->Verts[i].DiffuseColor = 0xFFFFFFFF;
						}

						CDx->DrawVerts(D3DPT_TRIANGLESTRIP, 2, this->Verts, sizeof(this->Verts[0]));

						for (int i = 0; i < vertCount; ++i) {
							this->Verts[i].DiffuseColor = backups[i];
						}

						CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);

						IDirect3DBaseTexture8* texture = this->Tex->RegTex;
						if (!texture)
							texture = this->Tex->CubeTex;

						CDx->DXDevice->SetTexture(0, texture);
						CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
						v6 = 1;
					}
				}
				else if (a6 == 2) {
					//print some debug message
				}
			}
			else if (a5 == 1) {
				if (a6 == 0) {
					if (!CDx->field_913) {
						CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
					}
					else {
						CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
						CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
						CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
					}
				}
				else if (a6 == 1) {
					if (CDx->field_913) {
						CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
						CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					}
					else {
						CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
					}
				} else if (a6 == 2) {
					;//print some debug message
				}
			}
			else if (a5 == 2) {
				if (!a6 || a6 == 1 || a6 == 2)
					;//print some debug message
			}
		}
	}

	CDx->DXDevice->SetVertexShader(FFXI::CYy::FVF144Vertex::FVF);
	CDx->DrawVerts(D3DPT_TRIANGLESTRIP, 2, a2, sizeof(FFXI::CYy::FVF144Vertex));

	if (v6 == 1) {
		CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		CDx->DXDevice->SetTexture(0, nullptr);

		const int vertCount = sizeof(this->Verts) / sizeof(this->Verts[0]);
		D3DCOLOR backups[vertCount];

		for (int i = 0; i < vertCount; ++i) {
			backups[i] = this->Verts[i].DiffuseColor;
			this->Verts[i].DiffuseColor = 0xFFFFFFFF;
		}

		CDx->DrawVerts(D3DPT_TRIANGLESTRIP, 2, this->Verts, sizeof(this->Verts[0]));

		for (int i = 0; i < vertCount; ++i) {
			this->Verts[i].DiffuseColor = backups[i];
		}

		CDx->DXDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
	}

	CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	if (CDx->field_913)
		CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

void FFXI::CTk::_49SubListNode::DrawTile(int a2, int a3, int a4, int a5, unsigned int a6, unsigned int a7, float a8, float a9, unsigned int a10, unsigned int a11, char a12)
{
	CYy::CDx* CDx = FFXI::CYy::CDx::instance;

	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, 1);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, 1);

	if (this->Tex) {
		if (this->Tex->RegTex)
			CDx->DXDevice->SetTexture(0, this->Tex->RegTex);
		else
			CDx->DXDevice->SetTexture(0, this->Tex->CubeTex);
	}
	else {
		_strlwr_s(this->field_32, sizeof(this->field_32));
		this->Tex = CYyDb::g_pCYyDb->pCYyTexMng->GetByUnique(this->field_32);
		if (!this->Tex)
			return;
	}

	const int arrSize = sizeof(this->Verts) / sizeof(this->Verts[0]);
	int ColorBackup[arrSize];
	float TexXBackup[arrSize];
	float TexYBackup[arrSize];

	for (int i = 0; i < arrSize; ++i) {
		ColorBackup[i] = this->Verts[i].DiffuseColor;
		TexXBackup[i] = this->Verts[i].TexVertX;
		TexYBackup[i] = this->Verts[i].TexVertY;

		this->Verts[i].DiffuseColor = Globals::ColorMult(this->Verts[i].DiffuseColor, a10);
	}

	double v26{}, v27{};

	if (a12) {
		v26 = (double)this->field_8 / (double)this->Tex->ImageWidth + this->Verts[0].TexVertX;
		v27 = (double)this->field_A / (double)this->Tex->ImageHeight + this->Verts[0].TexVertY;
	}
	else {
		v26 = (double)a6 / (double)this->Tex->ImageWidth + this->Verts[0].TexVertX;
		v27 = (double)a7 / (double)this->Tex->ImageHeight + this->Verts[0].TexVertY;
	}
	
	this->Verts[0].X = (float)a4 * a8 + (double)a2 - 0.5;
	this->Verts[1].X = ((float)a4 + (double)a6) * a8 + (double)a2 - 0.5;
	this->Verts[2].X = this->Verts[0].X;
	this->Verts[3].X = this->Verts[1].X;

	this->Verts[0].Y = (float)a5 * a9 + (double)a3 - 0.5;
	this->Verts[1].Y = this->Verts[0].Y;
	this->Verts[2].Y = ((float)a5 + (double)a7) * a9 + (double)a3 - 0.5;
	this->Verts[3].Y = this->Verts[2].Y;

	if ((this->field_1C & 1) != 0) {
		this->Verts[1].TexVertX = this->Verts[0].TexVertX;
		this->Verts[3].TexVertX = this->Verts[0].TexVertX;

		this->Verts[0].TexVertX = v26;
		this->Verts[2].TexVertX = v26;
	}
	else {
		this->Verts[1].TexVertX = v26;
		this->Verts[3].TexVertX = v26;
		this->Verts[2].TexVertX = this->Verts[0].TexVertX;
	}

	if ((this->field_1C & 2) != 0) {
		this->Verts[2].TexVertY = this->Verts[0].TexVertY;
		this->Verts[3].TexVertY = this->Verts[0].TexVertY;

		this->Verts[0].TexVertY = v27;
		this->Verts[1].TexVertY = v27;
	}
	else
	{
		this->Verts[1].TexVertY = this->Verts[0].TexVertY;

		this->Verts[2].TexVertY = v27;
		this->Verts[3].TexVertY = v27;
	}

	if (a11) {
		this->RenderTex(this->Verts, 1, a11 & 3, (a11 >> 2) & 3, (unsigned char)a11 >> 6);
	}
	else if (this->field_2D) {
		this->RenderTex(this->Verts, 1, this->field_2E, this->field_2F, this->field_30);
	}
	else {
		this->RenderTex(this->Verts, this->field_2D, this->field_2E, this->field_2F, this->field_30);
	}

	for (int i = 0; i < arrSize; ++i) {
		this->Verts[i].DiffuseColor = ColorBackup[i];
		this->Verts[i].TexVertX = TexXBackup[i];
		this->Verts[i].TexVertY = TexYBackup[i];
	}

	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSU, 3);
	CDx->DXDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_ADDRESSV, 3);
}
