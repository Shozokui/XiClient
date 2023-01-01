#include "FFXIDX.h"
#include "RegistryConfig.h"
#include "RuntimeConfig.h"

FFXI::FFXIDX* FFXI::FFXIDX::g_pFFXIDX{ nullptr };

D3DFORMAT D3DFormatTable[][10] = {
	{D3DFMT_X8R8G8B8, D3DFMT_D24S8, D3DFMT_A8R8G8B8, D3DFMT_D24S8, D3DFMT_UNKNOWN, D3DFMT_D24S8, D3DFMT_UNKNOWN, D3DFMT_UNKNOWN, D3DFMT_UNKNOWN, D3DFMT_UNKNOWN},
	{D3DFMT_X1R5G5B5, D3DFMT_D16, D3DFMT_A1R5G5B5, D3DFMT_D16, D3DFMT_R5G6B5, D3DFMT_D16, D3DFMT_UNKNOWN, D3DFMT_D16, D3DFMT_UNKNOWN, D3DFMT_UNKNOWN}
};

int FFXI::FFXIDX::InitHelper2(HWND p_hWnd, int p_xRight, int p_yBottom, bool p_flag1, bool p_flag2, int opt)
{
	D3DCAPS8 DXCaps{};
	if (this->CreateInterface(p_hWnd, p_xRight, p_yBottom, p_flag1, p_flag2, &DXCaps, opt)) {
		this->DXInterface->GetAdapterIdentifier(D3DADAPTER_DEFAULT, D3DENUM_WHQL_LEVEL, &this->AdapterIdentifier);
		return 0;
	}
	else if ((DXCaps.DevCaps & 0x80000) == 0 || p_flag2 == true || (DXCaps.DevCaps & 0x10000) != 0) {
		return 2000;
	}
	
	return 2001;
}

char FFXI::FFXIDX::CreateInterface(HWND p_hWnd, int p_xRight, int p_yBottom, bool p_flag1, bool p_flag2, D3DCAPS8* p_DXCaps, int opt)
{
	D3DDISPLAYMODE v18{ };
	unsigned int v11{ 0 };
	D3DDISPLAYMODE* v13; // eax
	unsigned int v14; // ecx
	D3DDISPLAYMODE* DisplayModes; // eax
	this->DXInterface = d3d8to9::Direct3DCreate8(220);
	if (this->DXInterface) {
		this->AdapterModeCount = this->DXInterface->GetAdapterModeCount(D3DADAPTER_DEFAULT);
		if (this->AdapterModeCount) {
			if (this->DisplayModes) delete this->DisplayModes;
			this->DisplayModes = new D3DDISPLAYMODE[this->AdapterModeCount];
			if (this->DisplayModes) {
				for (int i = 0; i < this->AdapterModeCount; ++i) {
					this->DXInterface->EnumAdapterModes(D3DADAPTER_DEFAULT, i, &v18);
					if (v18.Width >= 0x280
						&& v18.Height >= 0x1E0
						&& v18.Width <= 0x800
						&& v18.Height <= 0x800
						&& (v18.Format == D3DFMT_X8R8G8B8 || v18.Format == D3DFMT_A8R8G8B8))
					{
						if (v11)
						{
							v14 = 0;
							DisplayModes = this->DisplayModes;
							do
							{
								if (DisplayModes->Width == v18.Width && DisplayModes->Height == v18.Height)
									break;
								++v14;
								++DisplayModes;
							} while (v14 < v11);
							if (v14 != v11)
								continue;
							v13 = &this->DisplayModes[v11++];
						}
						else
						{
							v13 = this->DisplayModes;
							v11 = 1;
						}
						*v13 = v18;
					}
				}

				this->AdapterModeCount = v11;
				if (Config::MainRegistryConfig::dword104458A8 == 1)
				{
					if (this->TryCreateDevice(p_hWnd, p_xRight, p_yBottom, p_flag1, p_flag2, p_DXCaps, 1, opt)
						|| this->TryCreateDevice(p_hWnd, p_xRight, p_yBottom, p_flag1, p_flag2, p_DXCaps, 0, opt))
					{
						return 1;
					}
				}
				else if (this->TryCreateDevice(p_hWnd, p_xRight, p_yBottom, p_flag1, p_flag2, p_DXCaps, 0, opt)
					|| this->TryCreateDevice(p_hWnd, p_xRight, p_yBottom, p_flag1, p_flag2, p_DXCaps, 1, opt))
				{
					return 1;
				}

			}
		}
	}
	if (!this->DXInterface) return 0;
	this->DXInterface->Release();
	this->DXInterface = 0;
	return 0;
}

char FFXI::FFXIDX::TryCreateDevice(HWND p_hWnd, int p_xRight, int p_yBottom, bool p_flag1, bool p_flag2, D3DCAPS8* p_DXCaps, int p_offset, int opt)
{
	D3DFORMAT* i; // eax
	D3DFORMAT Format; // esi
	D3DFORMAT v11; // edi
	D3DDISPLAYMODE v13; // [esp+Ch] [ebp-10h] BYREF

	if (this->DXInterface->GetAdapterDisplayMode(0, &v13) < D3D_OK)
		return 0;
	for (i = D3DFormatTable[p_offset]; ; i += 2)
	{
		Format = *i;
		v11 = i[1];
		if (!*i)
		{
			if (!v11)
				return 0;
			Format = v13.Format;
		}
		if (this->VerifyDevice(v13.Format, Format, v11)
			&& this->CreateDevice(p_hWnd, Format, v11, p_xRight, p_yBottom, p_flag1, p_flag2, p_DXCaps, opt))
			break;
	}
	return 1;
}

bool FFXI::FFXIDX::VerifyDevice(D3DFORMAT a2, D3DFORMAT a3, D3DFORMAT a4)
{
	return this->DXInterface->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, a2, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, a4) >= D3D_OK
		&& this->DXInterface->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, a2, a3, a4) >= D3D_OK;
}

char FFXI::FFXIDX::CreateDevice(HWND p_hWnd, D3DFORMAT a3, D3DFORMAT a4, int p_xRight, int p_yBottom, bool p_flag1, bool p_flag2, D3DCAPS8* p_DXCaps, int opt)
{
	if (this->DXInterface->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, p_DXCaps) < D3D_OK)
		return 0;
	memset(&this->PresentParams, 0, sizeof(this->PresentParams));
	this->PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if (p_flag1)
	{
		this->PresentParams.Windowed = false;
		if (opt != 0)
		{
			if ((p_DXCaps->PresentationIntervals & 1) != 0)
			{
				this->PresentParams.FullScreen_PresentationInterval = 1;
				this->PresentParams.SwapEffect = D3DSWAPEFFECT_FLIP;
			}
		}
		else if ((p_DXCaps->PresentationIntervals & 0x80000000) == 0)
		{
			this->PresentParams.FullScreen_PresentationInterval = 0;
		}
		else
		{
			this->PresentParams.FullScreen_PresentationInterval = 0x80000000;
		}
	}
	else
	{
		this->PresentParams.Windowed = 1;
		this->PresentParams.FullScreen_PresentationInterval = 0;
	}
	this->PresentParams.BackBufferWidth = p_xRight;
	this->PresentParams.BackBufferHeight = p_yBottom;
	this->PresentParams.BackBufferFormat = a3;
	this->PresentParams.AutoDepthStencilFormat = a4;
	this->PresentParams.BackBufferCount = 1;
	this->PresentParams.EnableAutoDepthStencil = 1;
	this->PresentParams.hDeviceWindow = p_hWnd;
	if (p_flag2) {
		if (this->DXInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, p_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &this->PresentParams, (IDirect3DDevice8**)&this->DXDevice) >= D3D_OK)
			return 1;
	}
	
	if (this->DXInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, p_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &this->PresentParams, (IDirect3DDevice8**)&this->DXDevice) >= D3D_OK)
		return 1;
	
	return 0;
}

bool FFXI::FFXIDX::InitHelper1(HWND p_hWnd, int p_xRight, int p_yBottom, bool p_flag1, bool p_flag2, int opt)
{
	FFXI::FFXIDX::g_pFFXIDX = new FFXIDX();
	if (FFXI::FFXIDX::g_pFFXIDX == nullptr) {
		return false;
	}

	if (FFXI::FFXIDX::g_pFFXIDX->InitHelper2(p_hWnd, p_xRight, p_yBottom, p_flag1, p_flag2, opt) == false) {
		return false;
	}

	if (p_flag2 == true) {
		if (FFXI::FFXIDX::g_pFFXIDX->AdapterIdentifier.VendorId == (int)Constants::Enums::GPUVendorID::Intel) {
			int v8 = FFXI::FFXIDX::g_pFFXIDX->AdapterIdentifier.DeviceId;
			if (v8 != 0x2562 &&
				v8 != 0x1132 &&
				v8 != 0x7121 &&
				v8 != 0x7123 &&
				v8 != 0x7125 &&
				v8 != 0x7127 &&
				v8 != 0x3577 &&
				v8 != 0x3582 &&
				v8 != 0x2582 &&
				v8 != 0x2782 &&
				v8 != 0x2572) {
				FFXI::FFXIDX::g_pFFXIDX->field_474 = 1;
			}
		}
	}

	return true;
}

int FFXI::FFXIDX::InitializeDX(HWND hwnd)
{
	bool v12 = FFXI::Config::RegistryConfig::g_pOptions->Other.UseSoftwareVertexRendering != 0;
	bool v13 = FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::Fullscreen;
	int opt = FFXI::Config::RegistryConfig::g_pOptions->Other.field_64;
	int init_result = FFXI::FFXIDX::InitHelper1(hwnd, FFXI::Config::RuntimeConfig::instance.resolution_main.X, FFXI::Config::RuntimeConfig::instance.resolution_main.Y, v13, v12, opt);
	if (init_result != 0)
	{
		FFXI::FFXIDX::UninitializeDX();
		init_result = FFXI::FFXIDX::InitHelper1(hwnd, FFXI::Config::RuntimeConfig::instance.resolution_main.X, FFXI::Config::RuntimeConfig::instance.resolution_main.Y, v13, true, opt);
		if (init_result == 0) {
			FFXI::Config::RegistryConfig::g_pOptions->Other.UseSoftwareVertexRendering = 1;
			if (FFXI::FFXIDX::g_pFFXIDX->field_474 == 0)
			{
				init_result = 2001;
			}
			else {
				FFXI::Config::RegistryConfig::g_pOptions->Other.field_58 = 1;
			}
		}
	}

	return init_result;
}

void FFXI::FFXIDX::UninitializeDX()
{
	if (FFXI::FFXIDX::g_pFFXIDX) {
		delete FFXI::FFXIDX::g_pFFXIDX;
		FFXI::FFXIDX::g_pFFXIDX = nullptr;
	}
}

FFXI::FFXIDX::~FFXIDX() {
	if (this->DXDevice) {
		this->DXDevice->Release();
		this->DXDevice = nullptr;
	}
	if (this->DXInterface) {
		this->DXInterface->Release();
		this->DXInterface = nullptr;
	}
	if (this->DisplayModes)
		delete[] this->DisplayModes;
}

FFXI::FFXIDX::FFXIDX()
{
	this->DXInterface = nullptr;
	this->DXDevice = nullptr;
	this->AdapterModeCount = 0;
	this->DisplayModes = 0;
	memset(&this->AdapterIdentifier, 0, sizeof(this->AdapterIdentifier));
	this->field_474 = 0;
}
