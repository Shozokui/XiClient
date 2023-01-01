#pragma once
#include "StorageListObject.h"

namespace FFXI {
	namespace CYy {
		class CYyBmp2;
		class CYyTex;
		class CMoResource;
		class CYyTexMng : public StorageListObject {
		public:
			const static CYyClass CYyTexMngClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			~CYyTexMng();
			CYyTexMng();
			CYyTex* GetThing(char*, CYyBmp2*, int);
			CYyTex** GetTexAddr();
			CYyTex** AssignTex(CYyTex*);
			CYyTex** IsTexIn(CYyTex*);
			CYyTex* GetByUnique(const char*);
			CYyTex* FindTextureUnder(const char*, CMoResource*);
			CYyTex* FindTexture(const char*);
			CYyTex* FindD3sTexUnder(const char*, CMoResource*);
			void CheckTex();
			short field_4;
			char field_6;
			char field_7;
			CYyTex* Tex;
			CYyTex* Tex2;
			char field_10;
			char field_11;
			char field_12;
			char field_13;
		};
	}
}