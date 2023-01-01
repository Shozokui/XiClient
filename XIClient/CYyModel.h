#pragma once
#include "StorageListObject.h"
#include "ModelSubStruct1.h"
#include "ModelSubStruct2.h"
#include "ModelSubStruct5.h"
#include "WMatrix.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CYyModelBase;
		class CXiActor;
		class CYyModel : public StorageListObject {
		public:
			struct tex_struct {
				int field_0;
				IDirect3DBaseTexture8* field_4;
			};

			static const CYyClass CYyModelClass;
			static short g_some_short;
			static FFXI::Math::WMatrix FIRST_tex_transform;
			static FFXI::Math::WMatrix SECOND_tex_transform;
			static FFXI::Math::WMatrix view_transform;
			static FFXI::Math::WMatrix temp_proj_mtx;
			static FFXI::Math::WMatrix temp_view_mtx;
			static FFXI::Math::WMatrix temp_wrld_mtx;
			static D3DMATERIAL8 material;
			static D3DCOLOR ambient_color;
			static int maybe_polys_drawn;
			static float some_x_val;
			static float some_y_val;
			virtual const CYyClass* GetRuntimeClass() override final;
			CYyModel();
			CYyModelBase* CreateBase();
			CYyModelBase** GetBase();
			void Draw(CXiActor*, FFXI::Math::WMatrix*, D3DXVECTOR4*, D3DXVECTOR4*, D3DXVECTOR4*, D3DXVECTOR4*, unsigned int);
			void DrawBaseList(CXiActor*, float);
			bool DoingSomething(CXiActor*, FFXI::Math::WMatrix*, float, D3DXVECTOR4*);
			int GetSomeBaseIndex(int, int);
			bool GetSomething(int, int, D3DXVECTOR4*);
			int GetOs2ResId(int);
			void IsHideOs2(int, int, int);
			D3DXVECTOR4 field_4;
			int field_14;
			int field_18;
			int field_1C;
			int field_20;
			D3DXVECTOR3 field_24;
			int field_30;
			int field_34;
			int field_38;
			int field_3C;
			int field_40;
			CYyModelBase* Base;
			ModelSubStruct1 SubStruct1;
			ModelSubStruct2 SubStruct2;
			int field_8C;
			tex_struct field_90;
			int field_98;
			ModelSubStruct5 SubStruct5;
			char field_A8;
			char field_A9;
			char field_AA;
			char field_AB;
			char field_AC;
			char field_AD;
			char field_AE;
			char field_AF;
			char field_B0;
			char field_B1;
			char field_B2;
			char field_B3;
			int field_B4;
			char field_B8;
			char field_B9;
			char field_BA;
			char field_BB;
		};
	}
}