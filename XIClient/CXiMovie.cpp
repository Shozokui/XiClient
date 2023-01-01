#define WIN32_LEAN_AND_MEAN
#include "CXiMovie.h"
#include <dshow.h>
#include "VideoRenderer.h"
#include <iostream>
#include <TlHelp32.h>
using namespace FFXI::CYy;

int TextureOrder[] = { 2, 3, 0, 1 };
BYTE FuncSig[] = { 0x74, 0x0C, 0x33 };
//LOCAL FUNCS
void FilterNonDelRelease(IBaseFilter* filter) {
	INonDelegatingUnknown* base = (INonDelegatingUnknown*)filter;
	base->NonDelegatingRelease();
}
void PinNonDelRelease(IPin* pin) {
	INonDelegatingUnknown* base = (INonDelegatingUnknown*)pin;
	base->NonDelegatingRelease();
}

bool PatchFilter() {
	HANDLE snap{ INVALID_HANDLE_VALUE };
	while (true) {
		snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, NULL);
		if (snap) {
			if (snap == INVALID_HANDLE_VALUE) {
				if (GetLastError() == ERROR_BAD_LENGTH)
					continue;
				return false;
			}
			break;
		}
		return false;
	}
	
	MODULEENTRY32 me32{};
	me32.dwSize = sizeof(me32);
	if (!Module32First(snap, &me32)) {
		CloseHandle(snap);
		return false;
	}
	BYTE* baseAddr{ nullptr };
	DWORD baseSize{ 0 };
	do {
		if (!wcscmp(me32.szModule, L"polmvf.dll")) {
			baseAddr = me32.modBaseAddr;
			baseSize = me32.modBaseSize;
		}
	} while (Module32Next(snap, &me32));

	if (!baseAddr || !baseSize) {
		CloseHandle(snap);
		return false;
	}

	int matches = 0;
	DWORD pos = 0;
	while (pos < baseSize) {
		if (baseAddr[pos] == FuncSig[matches]) {
			if (++matches == sizeof(FuncSig))
				break;
		}
		else
			matches = 0;
		++pos;
	}

	CloseHandle(snap);

	if (matches == sizeof(FuncSig)) {
		baseAddr[pos - sizeof(FuncSig) + 1] = 0x75;
		return true;
	}
	return false;
}
//~LOCAL FUNCS
HRESULT CXiMovie::CreateFilterGraph(char* a1) {
	IBaseFilter* v17{ nullptr }, *v18{ nullptr };
	IPin* v19{ nullptr }, *v20{ nullptr };

	if (CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&this->GraphBuilder) < S_OK)
		return E_FAIL;
	
	HRESULT result{};
	this->Renderer = new FFXI::CYy::VideoRenderer(&result);
	
	if (result < S_OK) {
		if (v20)
			v20->Release();
		if (v18)
			v18->Release();
		if (v19)
			v19->Release();
		return E_FAIL;
	}

	v17 = this->Renderer;

	if (v17)
		((INonDelegatingUnknown*)v17)->NonDelegatingAddRef();
	
	result = this->GraphBuilder->AddFilter(this->Renderer, L"TEXTURERENDERER");

	if (result < S_OK) {
		if (v20)
			v20->Release();
		if (v18)
			v18->Release();
		if (v19)
			v19->Release();
		if (v17)
			v17->Release();
		return result;
	}

	WCHAR WideCharStr[260];
	MultiByteToWideChar(0, NULL, a1, -1, WideCharStr, (sizeof(WideCharStr)/sizeof(WideCharStr[0])));
	result = this->GraphBuilder->AddSourceFilter(WideCharStr, L"SOURCE", &v18);
	if (result == E_FAIL) {
		//Possible fail due to proc name protection
		//Patch the filter to skip that check
		if (PatchFilter())
			result = this->GraphBuilder->AddSourceFilter(WideCharStr, L"SOURCE", &v18);
	}
	if (result < S_OK) {
		if (v20)
			v20->Release();
		if (v18)
			v18->Release();
		if (v19)
			v19->Release();
		if (v17)
			v17->Release();
		return result;
	}
	result = this->Renderer->FindPin(L"In", &v19);
	if (result < S_OK) {
		if (v20)
			v20->Release();
		if (v18)
			v18->Release();
		if (v19)
			v19->Release();
		if (v17)
			v17->Release();
		return result;
	}

	result = v18->FindPin(L"Output", &v20);

	if (result < S_OK) {
		if (v20)
			v20->Release();
		if (v18)
			v18->Release();
		if (v19)
			v19->Release();
		if (v17)
			v17->Release();
		return result;
	}

	result = this->GraphBuilder->Connect(v20, v19);
	if (result >= S_OK) {	
		this->GraphBuilder->QueryInterface(IID_IMediaControl, (void**)&this->MediaControl);
		this->GraphBuilder->QueryInterface(IID_IMediaPosition, (void**)&this->MediaPosition);
		this->GraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&this->MediaEvent);
		PinNonDelRelease(v20);
		FilterNonDelRelease(v18);
		PinNonDelRelease(v19);
		FilterNonDelRelease(v17);
		return S_OK;
	}

	if (v20)
		v20->Release();
	if (v18)
		v18->Release();
	PinNonDelRelease(v19);
	FilterNonDelRelease(v17);
	return result;
}


IDirect3DTexture8* FFXI::CYy::CXiMovie::GetTexture()
{
	return this->Renderer->TextureArray[TextureOrder[this->Renderer->TextureIndex]];
}

FFXI::CYy::CXiMovie::CXiMovie()
{
	this->GraphBuilder = nullptr;
	this->MediaControl = 0;
	this->MediaPosition = 0;
	this->MediaEvent = 0;
	this->field_4 = 0;
	this->Renderer = 0;
}

bool CXiMovie::InitFilterGraph(char* a1)
{
	HRESULT res = this->CreateFilterGraph(a1);
	if (res < S_OK)
		return false;

	this->field_4 = 1;
	return true;
}