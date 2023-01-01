#pragma once
#include "d3d8to9/d3d8to9.hpp"

namespace FFXI {
	class FFXIDX {
	private:
		char CreateInterface(HWND, int, int, bool, bool, D3DCAPS8*, int);
		char TryCreateDevice(HWND, int, int, bool, bool, D3DCAPS8*, int, int);
		bool VerifyDevice(D3DFORMAT, D3DFORMAT, D3DFORMAT);
		char CreateDevice(HWND, D3DFORMAT, D3DFORMAT, int, int, bool, bool, D3DCAPS8*, int);
		static bool InitHelper1(HWND, int, int, bool, bool, int);
		int InitHelper2(HWND, int, int, bool, bool, int);
	public:
		static FFXIDX* g_pFFXIDX;
		static int InitializeDX(HWND);
		static void UninitializeDX();
		virtual ~FFXIDX();
		FFXIDX();


		IDirect3D8* DXInterface{ nullptr };
		Direct3DDevice8* DXDevice{ nullptr };
		D3DPRESENT_PARAMETERS8 PresentParams{};
		int AdapterModeCount{ 0 };
		D3DDISPLAYMODE* DisplayModes{ nullptr };
		D3DADAPTER_IDENTIFIER8 AdapterIdentifier{ };
		int field_474{ 0 };
	};
}