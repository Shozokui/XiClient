#define WIN32_LEAN_AND_MEAN
#include "CYyTex.h"
#include "CYyBmp2.h"
#include "Globals.h"
#include "XIFileManager.h"
#include "CDx.h"
#include "CYyDb.h"
#include "CYyTexMng.h"
#include "d3d8to9/d3d8to9.hpp"
#include <string>
using namespace FFXI::CYy;

const CYyClass CYyTex::CYyTexClass{
	"CYyTex", sizeof(CYyTex), &CYyTexBase::CYyTexBaseClass
};

int CYyTex::SomeFlag{ false };

char cubemap[] = "cubemap ";
int Types[] = { //DXT1-5
	0x44585431,
	0x44585432,
	0x44585433,
	0x44585434,
	0x44585435
};
D3DFORMAT Types2[] = {
	D3DFMT_DXT1,
	D3DFMT_DXT2,
	D3DFMT_DXT3,
	D3DFMT_DXT4,
	D3DFMT_DXT5
};

const CYyClass* FFXI::CYy::CYyTex::GetRuntimeClass()
{
	return &CYyTexClass;
}

FFXI::CYy::CYyTex::CYyTex()
{
	//not sure if field 30 is here or in texbase
	this->field_30 = 0;
	this->field_38 = 0;
	this->field_3C = 0;
	this->RegTex = nullptr;
	this->CubeTex = nullptr;
	this->field_34 = CYyTex::SomeFlag;
}

FFXI::CYy::CYyTex::~CYyTex()
{
	//sub //TODO
}

void FFXI::CYy::CYyTex::DoThing(char* a2, char a3, char a4)
{
	unsigned short* wordData = (unsigned short*)a2;
	memcpy(this->Unique, a2, sizeof(this->Unique));
	if (!memcmp(this->Unique, cubemap, sizeof(cubemap) - 1))
		this->field_2C = this->field_2C & 0xFFFFFFF8 | 5;

	void* v4 = a2 + 56;
	this->ImageWidth = wordData[10];
	this->ImageHeight = wordData[12];
	this->field_28 = a2[30];
	this->BitDepth = (unsigned char)a2[52];
	char v6{ 0 };
	int i{ 0 }, v7{ 0 }, v8{ 0 };
	void* v10{ nullptr };
	char* v4c{ nullptr };
	int v13{ 0 };
	int TexType = *(int*)v4;
	switch (a4) {
	case 0:
	case 1:
		break;
	case 2:
		if (a3 == 0 || TexType == 0) return;

		while (true) {
			if (Types[i] == TexType) {
				if (i == 0) {
					if (FFXI::CYy::CDx::instance->field_910)
						break;
				}
				else if (i == 2) {
					if (FFXI::CYy::CDx::instance->field_911)
						break;
				}
			}

			if (++i >= sizeof(Types) / sizeof(Types[0]))
				return;
		}

		this->field_20 = a2 + 60;
		this->BitDepth = i | 0x80;
		return;
	case 3:
		v7 = this->field_2C;
		v7 &= 0xFFFFFF0F;
		v8 = v7 ^ (16 * (*(char*)v4 & 0xF));
		this->field_2C = v8;
		v4 = a2 + 60;
	break;
	default:
		return;
	}

	switch (this->field_28) {
	case 4:
		this->field_1C = v4;
		v4c = (char*)v4;
		switch (this->BitDepth) {
		case 16:
			v4c += 32;
			break;
		case 24:
			v4c += 48;
			break;
		case 32:
			v4c += 64;
			break;
		}
		v4 = (void*)v4c;
		v10 = &v4c[(this->ImageWidth * this->ImageHeight + 1) / 2];
		break;
	case 8:
		this->field_1C = v4;
		v4c = (char*)v4;
		switch (this->BitDepth) {
		case 16:
			v4c += 512;
			break;
		case 24:
			v4c += 768;
			break;
		case 32:
			v4c += 1024;
			break;
		}
		v4 = (void*)v4c;
		v10 = &v4c[this->ImageWidth * this->ImageHeight];
		break;
	case 0x10:
		v4c = (char*)v4;
		v10 = &v4c[this->ImageWidth * this->ImageHeight];
		break;
	case 0x18:
	{
		v4c = (char*)v4;
		unsigned int v12 = this->ImageWidth * this->ImageHeight;
		v10 = &v4c[2 * v12 + v12];
	}
	break;
	case 0x20:
		v4c = (char*)v4;
		v10 = &v4c[4 * this->ImageWidth * this->ImageHeight];
		break;
	default:
		break;
	}
	this->field_20 = v4;
	if (!a3 || !*(int*)v10)
		return;
	v13 = 0;
	char v14{ 0 };
	while (true) {
		if (*(int*)v10 == Types[v13]) {
			if (v13 == 0) {
				if (FFXI::CYy::CDx::instance->field_910) {
					this->field_20 = (char*)v10 + 4;
					this->BitDepth = v13 | 0x80;
					return;
				}
			}
			else if (v13 == 2) {
				if (FFXI::CYy::CDx::instance->field_911) {
					this->field_20 = (char*)v10 + 4;
					this->BitDepth = v13 | 0x80;
					return;
				}
			}
		}
		if (++v13 >= 5) {
			if (v13 == 5)
				return;
			this->field_20 = (char*)v10 + 4;
			this->BitDepth = v13 | 0x80;
		}
	}
}

void FFXI::CYy::CYyTex::Init()
{
	if (!this->CreateTextures()) return;

	CYyBmp2* v3 = this->field_8;
	this->field_3C = 3;
	if (v3) {
		CMoResource* res = v3->DoSomething3();
		res->PreHeader.field_18 -= 1;
	}

}

bool FFXI::CYy::CYyTex::CreateTextures()
{
	unsigned short v2 = this->ImageHeight;
	char v18 = 0;
	unsigned int v20 = 0;
	unsigned int v21 = 0;
	unsigned short v3 = this->ImageWidth;
	int v4 = 2;
	int v5 = v3;
	unsigned int v22 = 12;
	HRESULT v9{};
	IDirect3DTexture9* newTex{ nullptr };
	IDirect3DCubeTexture9* newCube{ nullptr };
	do {
		if (v3 == 1)
			v20 = 1;
		if (v2 == 1)
			v21 = 1;
		if (v3 == v4)
			v20 = 1;
		if (v2 == v4)
			v21 = 1;
		v4 *= 2;
		--v22;
	} while (v22);
	if (!v20 || !v21)
		v18 = 1;
	if (this->field_2A != 1 && (v3 < 0x20u || v2 < 0x20u))
		this->field_2A = 1;
	if (!this->field_20 && !this->field_1C)
		return true;

	unsigned int v7 = this->field_2C >> 4;
	char v19 = v7 & 0xF;
	int v10{ 0 };
	if (v19 != 9) {
		v22 = this->BitDepth;
		if ((v22 & 0x80u) != 0 && *(int*)this->field_20)
			goto LABEL_33;
		if (v19 == 10) {
			if (Config::MainRegistryConfig::MapQuality)
				goto LABEL_26;
		LABEL_33:
			v10 = 0;
			if ((v22 & 0x80) != 0)
				v10 = v22 & 0x7F;
			else if (v22 == 16)
				v10 = 0;
			else if (v22 == 24 || v22 == 32)
				v10 = 2;
			if (v10) {
				if (v10 != 2)
					goto LABEL_58;
				if ((this->field_2C & 7) != 0)
					goto LABEL_30;
				unsigned int v23 = (unsigned short)v3;
				int v11 = (unsigned char)this->field_2A;
				v20 = v2;
				v22 = D3DFMT_DXT3;
				v21 = v11;
				if (D3DXCheckTextureRequirements(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(),
					&v23, &v20, &v21, 0, (D3DFORMAT*)&v22, D3DPOOL_MANAGED) >= D3D_OK) {

					newTex = nullptr;
					v9 = D3DXCreateTexture(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(),
						v23, v20, v21, 0, (D3DFORMAT)v22, D3DPOOL_MANAGED, &newTex);
					this->RegTex = new Direct3DTexture8(FFXI::CYy::CDx::instance->DXDevice, newTex);

					goto LABEL_57;
				}
			}
			else {
				if ((this->field_2C & 7) != 0)
					goto LABEL_55;
				v20 = v3;
				int v13 = (unsigned char)this->field_2A;
				v21 = v2;
				D3DFORMAT v23 = D3DFMT_DXT1;
				v22 = v13;
				if (D3DXCheckTextureRequirements(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(),
					&v20, &v21, &v22, 0, &v23, D3DPOOL_MANAGED) >= D3D_OK) {
					newTex = nullptr;
					v9 = D3DXCreateTexture(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(),
						v20, v21, v22, 0, v23, D3DPOOL_MANAGED, &newTex);
					this->RegTex = new Direct3DTexture8(FFXI::CYy::CDx::instance->DXDevice, newTex);
					goto LABEL_57;
				}
			}
			return false;
		}
		if (!v19 || (v7 & 0xF) > Config::MainRegistryConfig::TextureQuality)
			goto LABEL_33;
	}
LABEL_26:
	switch (this->BitDepth) {
	case 16:
		if ((this->field_2C & 7) == 0) {
			newTex = nullptr;
			v9 = D3DXCreateTexture(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(),
				v5, v2, this->field_2A, 0, D3DFMT_A1R5G5B5, D3DPOOL_MANAGED, &newTex);
			this->RegTex = new Direct3DTexture8(FFXI::CYy::CDx::instance->DXDevice, newTex);
			break;
		}
	LABEL_55:
		newCube = nullptr;
		v9 = D3DXCreateCubeTexture(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(),
			v5, this->field_2A, 0, D3DFMT_A1R5G5B5, D3DPOOL_MANAGED, &newCube);
		this->CubeTex = new Direct3DCubeTexture8(FFXI::CYy::CDx::instance->DXDevice, newCube);
		break;
	case 24:
		if ((this->field_2C & 7) != 0) {
			newCube = nullptr;
			v9 = D3DXCreateCubeTexture(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(),
				v5, this->field_2A, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &newCube);
			this->CubeTex = new Direct3DCubeTexture8(FFXI::CYy::CDx::instance->DXDevice, newCube);
		}
		else {
			newTex = nullptr;
			v9 = D3DXCreateTexture(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(),
				v5, v2, this->field_2A, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &newTex);
			this->RegTex = new Direct3DTexture8(FFXI::CYy::CDx::instance->DXDevice, newTex);
		}
		break;
	case 32:
		if ((this->field_2C & 7) != 0) {
		LABEL_30:
			newCube = nullptr;
			v9 = D3DXCreateCubeTexture(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(),
				v5, this->field_2A, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &newCube);
			this->CubeTex = new Direct3DCubeTexture8(FFXI::CYy::CDx::instance->DXDevice, newCube);
			break;
		}
		newTex = nullptr;
		v9 = D3DXCreateTexture(FFXI::CYy::CDx::instance->DXDevice->GetProxyInterface(),
			v5, v2, this->field_2A, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &newTex);
		this->RegTex = new Direct3DTexture8(FFXI::CYy::CDx::instance->DXDevice, newTex);
		break;
	default:
		goto LABEL_58;
	}
LABEL_57:
	if (v9 < D3D_OK)
		return false;
LABEL_58:
	if (!this->SurfaceStuff())
		return false;
	if (!v18)
		return true;
	if (!this->RegTex)
		return true;
	IDirect3DSurface8* surf{ nullptr };
	this->RegTex->GetSurfaceLevel(0, &surf);
	if (!surf)
		return true;
	D3DSURFACE_DESC8 v24{};
	surf->GetDesc(&v24);
	surf->Release();
	this->ImageWidth = v24.Width;
	this->ImageHeight = v24.Height;
	return false;
}

bool FFXI::CYy::CYyTex::SurfaceStuff()
{
	Direct3DSurface8* v51{ nullptr };
	Direct3DSurface8* v52{ nullptr };
	IDirect3DTexture9* tex{ nullptr };
	D3DLOCKED_RECT lockedRect{};
	Direct3DTexture8* tex2{ nullptr };
	RECT v58{};
	HRESULT result{};
	CDx* CDx = FFXI::CYy::CDx::instance;
	int v50 = 0;
	if (this->BitDepth < 0 && *(int*)this->field_20) {
		v50 = 1;
	}
	else {
		switch (this->BitDepth) {
		case 16:
			result = D3DXCreateTexture(CDx->DXDevice->GetProxyInterface(),
				this->ImageWidth, this->ImageHeight,
				0, 0, D3DFMT_A1R5G5B5, D3DPOOL_MANAGED, &tex);
			if (result < D3D_OK)
				goto LABEL_68;

		case 24:
			result = D3DXCreateTexture(CDx->DXDevice->GetProxyInterface(),
				this->ImageWidth, this->ImageHeight,
				0, 0, D3DFMT_R8G8B8, D3DPOOL_MANAGED, &tex);
			if (result < D3D_OK)
				goto LABEL_68;
		case 32:
			result = D3DXCreateTexture(CDx->DXDevice->GetProxyInterface(),
				this->ImageWidth, this->ImageHeight,
				0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex);
			if (result < D3D_OK)
				goto LABEL_68;
			break;
		}

		tex->LockRect(0, &lockedRect, 0, 0);
		switch (this->BitDepth) {
		case 16:
			throw "NOT IMPLEMENTED";

		case 24:
			throw "NOT IMPLEMENTED";
		case 32:
			if ((this->field_2C & 8) != 0) {
				throw "NOT IMPLEMENTED";
			}
			else {
				for (int x = 0; x < this->ImageHeight; ++x) {
					char* v7 = (char*)lockedRect.pBits + x * lockedRect.Pitch;
					for (int y = 0; y < this->ImageWidth; ++y) {
						switch (this->field_28) {
						case 8:
						{
							int v11 = *((int*)this->field_1C + *((unsigned char*)this->field_20 + this->ImageWidth * (this->ImageHeight - x - 1) + y));
							*(int*)v7 = v11;
							v7 += 4;
						}
							break;
						case 32: {
							int v11 = *((int*)this->field_20 + y + this->ImageWidth * (this->ImageHeight - x - 1));
							*(int*)v7 = v11;
							v7 += 4;
						}
							break;
						default:
							throw "NOT IMPLEMENTED";
						}
					}
				}
			}
		}
		tex->UnlockRect(0);
	}
	
	v58.top = 0;
	v58.bottom = this->ImageHeight;
	v58.left = 0;
	v58.right = this->ImageWidth;
	if ((this->field_2C & 7) == 0) {
		unsigned int v49 = 0;
		if (!this->RegTex->GetLevelCount())
			goto LABEL_137;
		while (true) {
			if (v49) {
				throw "NOT IMPLEMENTED";
			}
			else {
				if (!v50) {
					tex2 = new Direct3DTexture8(CDx->DXDevice, tex);
					tex2->GetSurfaceLevel(0, (IDirect3DSurface8**)&v52);
					this->RegTex->GetSurfaceLevel(0, (IDirect3DSurface8**)&v51);
					D3DXLoadSurfaceFromSurface(v51->GetProxyInterface(), NULL, NULL,
						v52->GetProxyInterface(), NULL, NULL, D3DX_FILTER_NONE, 0);
					if (tex2) {
						tex2->Release();
						tex2 = nullptr;
					}
					if (v52) {
						v52->Release();
						v52 = nullptr;
					}
					
					if (v51) {
					LABEL_135:
						v51->Release();
						v51 = nullptr;
					}

					goto LABEL_136;
				}
				this->RegTex->GetSurfaceLevel(0, (IDirect3DSurface8**)&v51);

				HRESULT result = D3DXLoadSurfaceFromMemory(v51->GetProxyInterface(),
					NULL, NULL,
					(char*)this->field_20 + 8,
					Types2[this->BitDepth & 0x7F],
					*((UINT*)this->field_20 + 1),
					NULL, &v58, D3DX_FILTER_NONE, NULL);
			}
			if (v51)
				goto LABEL_135;
		LABEL_136:
			if (++v49 >= this->RegTex->GetLevelCount())
				goto LABEL_137;
		}
	}
	//sub //todo
	if (false) { //createtex
	LABEL_137:
		if (v52) {
			v52->Release();
			v52 = nullptr;
		}
		if (v51) {
			v51->Release();
			v51 = nullptr;
		}
		if (tex)
			tex->Release();

		return true;
	}

LABEL_68:
	if (v52) {
		v52->Release();
		v52 = nullptr;
	}
	if (v51) {
		v51->Release();
		v51 = nullptr;
	}

	if (tex)
		tex->Release();
	return false;
}

void FFXI::CYy::CYyTex::Clean()
{
	CYyTex** v2 = CYyDb::g_pCYyDb->pCYyTexMng->IsTexIn(this);
	if (!v2) return;

	*v2 = (*v2)->field_38;
	if (this->RegTex) {
		this->RegTex->Release();
		this->RegTex = nullptr;
	}

	if (this->CubeTex) {
		this->CubeTex->Release();
		this->CubeTex = nullptr;
	}

	this->field_38 = 0;
	delete this;
}

bool FFXI::CYy::CYyTex::LoadFromNumFile(int a1)
{
	if (!this) return false;

	Direct3DSurface8* surf{ nullptr };
	if (this->RegTex) {
		this->RegTex->GetSurfaceLevel(0, (IDirect3DSurface8**)&surf);
	}
	else if (this->CubeTex) {
		throw "NOT IMPLEMENTED";
		//sub //todo Not sure if they support cube texture here
		//this->CubeTex->GetCubeMapSurface(0, 0, &surf);
	}
	else
		return false;

	surf->Release();

	int filesize = FFXI::File::XIFileManager::g_pXIFileManager->GetFileSizeByNumfile(a1);
	if (!filesize)
		return false;

	char* buffer = new char[filesize];
	FFXI::File::XIFileManager::g_pXIFileManager->ReadNumfileNow(a1, buffer, filesize, 0);
	bool result = D3DXLoadSurfaceFromFileInMemory(surf->GetProxyInterface(), NULL, NULL, buffer, filesize, NULL, D3DX_FILTER_NONE, 0, NULL) == D3D_OK;
	
	delete[] buffer;
	return result;
}
