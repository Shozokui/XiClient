#define WIN32_LEAN_AND_MEAN
#include "VideoRenderer.h"
#include <ks.h>
#include <ksmedia.h>
#include <iostream>
#include "Globals.h"
#include "CDx.h"
#include "d3d8to9/d3d8to9.hpp"
using namespace FFXI::CYy;

const IID VideoRenderer::thisCLSID = {
	0x3EBC303,
	0x863C,
	0x474C,
	0xBF, 0x98, 0x74, 0xC4, 0x57, 0xE1, 0xEB, 0x74
};

int VideoRenderer::FrameCopyLock{ 0 };

FFXI::CYy::VideoRenderer::VideoRenderer(HRESULT* a)
	: CBaseVideoRenderer(thisCLSID, 0, nullptr, a)
{
}

HRESULT FFXI::CYy::VideoRenderer::DoRenderSample(IMediaSample* pMediaSample)
{
	if (VideoRenderer::FrameCopyLock == 1)
		return S_OK;

	VideoRenderer::FrameCopyLock = 1;

	BYTE* ppBuffer{ nullptr };
	pMediaSample->GetPointer(&ppBuffer);

	IDirect3DTexture8* tex = this->TextureArray[this->TextureIndex];
	
	D3DLOCKED_RECT v22{};
	if (tex->LockRect(0, &v22, nullptr, NULL) < D3D_OK) {
		VideoRenderer::FrameCopyLock = 0;
		return E_FAIL;
	}
	BYTE* v4 = static_cast<BYTE*>(v22.pBits);
	BYTE* v6 = ppBuffer;
	BYTE* v8{ nullptr }, * i{ nullptr };
	int v18{ 0 }, v5{ this->field_160 };
	if (this->field_16C == 21 && this->field_164 > 0)
	{
		v18 = 0;
		v6 = ppBuffer;
		do
		{
			int v7 = 0;
			v8 = v6;
			for (i = v4; v7 < v5; ppBuffer += 3)
			{
				*v4 = *v6;
				v4 += 4;
				*(v4 - 3) = ppBuffer[1];
				*(v4 - 2) = ppBuffer[2];
				v5 = this->field_160;
				v6 = ppBuffer + 3;
				++v7;
			}
			v6 = &v8[this->field_168];
			v4 = &i[v22.Pitch];
			ppBuffer = v6;
			++v18;
		} while (v18 < this->field_164);
	}

	if (this->field_16C == 25)
	{
		int v19 = 0;
		if (this->field_164 > 0)
		{
			int v11 = this->field_160;
			do
			{
				int v12 = 0;
				BYTE* v13 = v6;
				BYTE* j{ nullptr };
				for (j = v4; v12 < v11; ppBuffer += 3)
				{
					v4 += 2;
					WORD* v4w = reinterpret_cast<WORD*>(v4);
					*(v4w - 1) = (*v6 >> 3) + 4 * ((v6[1] & 0xF8) + 32 * ((v6[2] & 0xF8) - 256));
					v11 = this->field_160;
					v6 = ppBuffer + 3;
					++v12;
				}
				v4 = j + v22.Pitch;
				v6 = &v13[this->field_168];
				ppBuffer = v6;
				++v19;
			} while (v19 < this->field_164);
		}
	}

	if (tex->UnlockRect(0) < D3D_OK) {
		VideoRenderer::FrameCopyLock = 0;
		return E_FAIL;
	}
	this->TextureIndex += 1;
	this->TextureIndex &= 3;
	VideoRenderer::FrameCopyLock = 0;
	return S_OK;
}

HRESULT FFXI::CYy::VideoRenderer::CheckMediaType(const CMediaType* a1)
{
	if (a1->formattype != KSDATAFORMAT_SPECIFIER_VIDEOINFO)
		return E_INVALIDARG;
	if (a1->majortype == MEDIATYPE_Video
		&& a1->subtype == MEDIASUBTYPE_RGB24) {
		return S_OK;
	}
	
	return E_FAIL;
}

HRESULT FFXI::CYy::VideoRenderer::SetMediaType(const CMediaType* pmt)
{
	int* fmtdata = reinterpret_cast<int*>(pmt->pbFormat);
	this->field_160 = fmtdata[13];
	this->field_164 = abs(fmtdata[14]);
	this->field_168 = (3 * (this->field_160 + 1)) & 0xFFFFFFFC;
	HRESULT result{};
	
	for (int i = 0; i < sizeof(this->TextureArray) / sizeof(this->TextureArray[0]); ++i) {
		IDirect3DTexture9* newTex{ nullptr };
		result = D3DXCreateTexture(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(), 512, 512, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &newTex);
		this->TextureArray[i] = new Direct3DTexture8(FFXI::CYy::CDx::instance->DXDevice, newTex);
		if (result < S_OK) return result;
	}

	D3DSURFACE_DESC8 v10{};
	result = this->TextureArray[0]->GetLevelDesc(0, &v10);
	if (result < D3D_OK) return result;
	this->field_16C = v10.Format;
	
	if (v10.Format == D3DFMT_A8R8G8B8 || v10.Format == D3DFMT_A1R5G5B5)
		return S_OK;

	return VFW_E_TYPE_NOT_ACCEPTED;
}