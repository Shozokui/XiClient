/**
 * Copyright (C) 2015 Patrick Mours. All rights reserved.
 * License: https://github.com/crosire/d3d8to9#license
 */

#include "d3dx9.hpp"
#include "d3d8to9.hpp"
#define D3D8TO9NOLOG
PFN_D3DXAssembleShader D3DXAssembleShader = nullptr;
PFN_D3DXDisassembleShader D3DXDisassembleShader = nullptr;
PFN_D3DXLoadSurfaceFromSurface D3DXLoadSurfaceFromSurface = nullptr;
PFN_D3DXLoadSurfaceFromMemory D3DXLoadSurfaceFromMemory = nullptr;
PFN_D3DXLoadSurfaceFromFileInMemory D3DXLoadSurfaceFromFileInMemory = nullptr;
PFN_D3DXCreateTexture D3DXCreateTexture = nullptr;
PFN_D3DXCheckTextureRequirements D3DXCheckTextureRequirements = nullptr;
PFN_D3DXCreateCubeTexture D3DXCreateCubeTexture = nullptr;
PFN_D3DXCheckCubeTextureRequirements D3DXCheckCubeTextureRequirements = nullptr;

 // Very simple logging for the purpose of debugging only.
std::ofstream LOG;

IDirect3D8* d3d8to9::Direct3DCreate8(UINT SDKVersion)
{
#ifndef D3D8TO9NOLOG
	static bool LogMessageFlag = true;

	if (!LOG.is_open())
	{
		LOG.open("d3d8.log", std::ios::trunc);
	}

	if (!LOG.is_open() && LogMessageFlag)
	{
		LogMessageFlag = false;
		MessageBox(nullptr, TEXT("Failed to open debug log file \"d3d8.log\"!"), nullptr, MB_ICONWARNING);
	}

	LOG << "Redirecting '" << "Direct3DCreate8" << "(" << SDKVersion << ")' ..." << std::endl;
	LOG << "> Passing on to 'Direct3DCreate9':" << std::endl;
#endif

	IDirect3D9 *const d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (d3d == nullptr)
	{
		return nullptr;
	}

	// Load D3DX
	if (!D3DXAssembleShader || !D3DXDisassembleShader || !D3DXLoadSurfaceFromSurface || 
		!D3DXLoadSurfaceFromMemory || !D3DXLoadSurfaceFromFileInMemory ||
		!D3DXCreateTexture || !D3DXCheckTextureRequirements ||
		!D3DXCreateCubeTexture || !D3DXCheckCubeTextureRequirements)
	{
		const HMODULE module = LoadLibrary(TEXT("d3dx9_43.dll"));

		if (module != nullptr)
		{
			D3DXAssembleShader = reinterpret_cast<PFN_D3DXAssembleShader>(GetProcAddress(module, "D3DXAssembleShader"));
			D3DXDisassembleShader = reinterpret_cast<PFN_D3DXDisassembleShader>(GetProcAddress(module, "D3DXDisassembleShader"));
			D3DXLoadSurfaceFromSurface = reinterpret_cast<PFN_D3DXLoadSurfaceFromSurface>(GetProcAddress(module, "D3DXLoadSurfaceFromSurface"));
			D3DXLoadSurfaceFromFileInMemory = reinterpret_cast<PFN_D3DXLoadSurfaceFromFileInMemory>(GetProcAddress(module, "D3DXLoadSurfaceFromFileInMemory"));
			D3DXLoadSurfaceFromMemory = reinterpret_cast<PFN_D3DXLoadSurfaceFromMemory>(GetProcAddress(module, "D3DXLoadSurfaceFromMemory"));
			D3DXCreateTexture = reinterpret_cast<PFN_D3DXCreateTexture>(GetProcAddress(module, "D3DXCreateTexture"));
			D3DXCheckTextureRequirements = reinterpret_cast<PFN_D3DXCheckTextureRequirements>(GetProcAddress(module, "D3DXCheckTextureRequirements"));
			D3DXCreateCubeTexture = reinterpret_cast<PFN_D3DXCreateCubeTexture>(GetProcAddress(module, "D3DXCreateCubeTexture"));
			D3DXCheckCubeTextureRequirements = reinterpret_cast<PFN_D3DXCheckCubeTextureRequirements>(GetProcAddress(module, "D3DXCheckCubeTextureRequirements"));
		}
		else
		{
#ifndef D3D8TO9NOLOG
			LOG << "Failed to load d3dx9_43.dll! Some features will not work correctly." << std::endl;
#endif
			if (MessageBox(nullptr, TEXT(
				"Failed to load d3dx9_43.dll! Some features will not work correctly.\n\n"
				"It's required to install the \"Microsoft DirectX End-User Runtime\" in order to use d3d8to9.\n\n"
				"Please click \"OK\" to open the official download page or \"CANCEL\" to continue anyway."), nullptr, MB_ICONWARNING | MB_TOPMOST | MB_SETFOREGROUND | MB_OKCANCEL | MB_DEFBUTTON1) == IDOK)
			{
				ShellExecute(nullptr, TEXT("open"), TEXT("https://www.microsoft.com/download/details.aspx?id=35"), nullptr, nullptr, SW_SHOW);

				return nullptr;
			}
		}
	}

	return new Direct3D8(d3d);
}
