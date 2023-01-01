#include "CYyTexMng.h"
#include "CYyTex.h"
#include "Globals.h"
#include "CYyBmp2.h"
#include "CMoResourceMng.h"
#include "CMoResource.h"
using namespace FFXI::CYy;
const CYyClass CYyTexMng::CYyTexMngClass{
	"CYyTexMng", sizeof(CYyTexMng), &CYyObject::CYyObjectClass
};

const CYyClass* FFXI::CYy::CYyTexMng::GetRuntimeClass()
{
	return &CYyTexMngClass;
}

FFXI::CYy::CYyTexMng::~CYyTexMng()
{
	if (this->Tex) {
		delete this->Tex;
		this->Tex = nullptr;
	}

}

FFXI::CYy::CYyTexMng::CYyTexMng()
{
	this->field_4 = 0;
	this->Tex = nullptr;
	this->Tex2 = nullptr;
	this->field_10 = 0;
}

CYyTex* FFXI::CYy::CYyTexMng::GetThing(char* a2, CYyBmp2* a3, int a4)
{
	CYyTex** texaddr = this->GetTexAddr();
	CYyTex* tex = *texaddr;
	if (!tex) return 0;
	while (a4 == 1) {
		if ((tex->field_2C & 8) != 0)
			goto LABEL_6;
	LABEL_11:
		texaddr = &tex->field_38;
		tex = tex->field_38;
		if (!tex) return 0;
	}
	if ((tex->field_2C & 8) != 0)
		goto LABEL_11;
LABEL_6:
	int i = 0;
	for (i = 0; i < 16; ++i) {
		if (tex->Unique[i] != a2[i])
			break;
	}
	if (i != 16 || tex->field_8 != a3)
		goto LABEL_11;
	return *texaddr;
}

CYyTex** FFXI::CYy::CYyTexMng::GetTexAddr()
{
	return &this->Tex;
}

CYyTex** FFXI::CYy::CYyTexMng::AssignTex(CYyTex* a2)
{
	a2->field_3C = 1;
	CYyTex** result = this->GetTexAddr();
	CYyTex* v2 = *result;
	while (v2) {
		result = &v2->field_38;
		v2 = v2->field_38;
	}
	*result = a2;
	return result;
}

CYyTex** FFXI::CYy::CYyTexMng::IsTexIn(CYyTex* a2)
{
	CYyTex** result = this->GetTexAddr();
	CYyTex* v3 = *result;
	if (!v3) return nullptr;
	while (v3 != a2) {
		result = &v3->field_38;
		v3 = v3->field_38;
		if (!v3) return nullptr;
	}
	return result;
}

CYyTex* FFXI::CYy::CYyTexMng::GetByUnique(const char* a2)
{
	CYyTex** ret = this->GetTexAddr();
	if (!*ret) return nullptr;
	while (true) {
		if (((*ret)->field_2C & 8) == 0) {
			int i = 0;
			for (i = 0; i < sizeof((*ret)->Unique); ++i) {
				if ((*ret)->Unique[i] != a2[i])
					break;
			}
			if (i == 16)
				break;
		}
		ret = &(*ret)->field_38;
		if (!*ret) return nullptr;
	}
	return *ret;
}

CYyTex* FFXI::CYy::CYyTexMng::FindTextureUnder(const char* a2, CMoResource* a3)
{
	CMoResource** Unused{ nullptr };
	CMoResource** res = *CMoResourceMng::FindNextUnder(&Unused, a3, FFXI::Constants::Enums::ResourceType::D3s, 0);
	if (!res)
		return nullptr;

	CYyBmp2* tex = *(CYyBmp2**)res;
	while (true) {
		if (!strncmp(tex->Data + 1, a2, 0x10)) 
		{
			CYyTex* Texture = this->Tex;
			while (Texture) {
				if (Texture->field_8 == tex)
					return Texture;
				Texture = Texture->field_38;
			}
		}
		CMoResource** Next = tex->PreHeader.NextRPL;
		if (!Next)
			return nullptr;
		tex = (CYyBmp2*)*Next;
	}
	return nullptr;
}

CYyTex* FFXI::CYy::CYyTexMng::FindTexture(const char* a2)
{
	CYyTex* tex = this->GetByUnique(a2);
	if (tex)
		return tex;

	tex = this->Tex;
	while (tex) {
		if ((tex->field_2C & 8) == 0) {
			if (!memcmp(tex->Unique + 8, a2 + 8, 8))
				return tex;
		}
		tex = tex->field_38;
	}

	return nullptr;
}

CYyTex* FFXI::CYy::CYyTexMng::FindD3sTexUnder(const char*, CMoResource*)
{
	return nullptr;
}

void FFXI::CYy::CYyTexMng::CheckTex()
{
	CYyTex** tex{ nullptr };
	if (this->Tex2) {
		tex = &this->Tex2;
	}
	else {
		tex = this->GetTexAddr();
	}
	CYyTex* i{ nullptr };
	int v2 = 0;
	for (i = *tex; *tex; i = *tex) {
		if (i->field_3C == 1) {
			i->Init();
			this->Tex2 = *tex;
			if (FFXI::GlobalStruct::g_GlobalStruct.field_0 == 96 && !this->field_10 && ++v2 > 20)
				return;
		}
		else if (i->field_3C == 2) {
			if (i->RegTex || i->CubeTex) {
				IDirect3DBaseTexture8* tex2 = i->RegTex;
				if (!tex2)
					tex2 = i->CubeTex;
				tex2->Release();
			}
			if ((*tex)->SurfaceStuff())
				this->field_13 = 3;
			this->Tex2 = *tex;
		}
		tex = &(*tex)->field_38;
	}
}
