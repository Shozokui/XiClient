#pragma once
#include "StorageListObject.h"
#include "CYySkl.h"
#include "CYyMotionMan.h"
#include "WMatrix.h"
#include "d3dx9math.h"
namespace FFXI {
	namespace CYy {
		class CMoSk2;
		class CMoOs2;
		class CYyModelDt;
		class CXiActor;
		class CYyModel;
		class CYyTex;
		class CYyModelBase : public StorageListObject {
		private:
			void AddDt(CYyModelDt*);
		public:
			static const CYyClass CYyModelBaseClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CYyModelBase();
			virtual ~CYyModelBase();
			void SetSkeleton(CMoSk2**);
			void Init();
			CYyModelDt** GetModelDt();
			int GetSomeIndex(unsigned int);
			void LinkOs2(CMoOs2**);
			bool DoingSomething(CXiActor*, FFXI::Math::WMatrix*, float, D3DXVECTOR4*, int);
			void GetSomeVector(int, D3DXVECTOR3*);
			void TWO_GetSomeVector(int, D3DXVECTOR4*);
			void Draw(CXiActor*, CYyModel*, float);
			CYyModelBase* Previous;
			CYySkl Skeleton;
			CYyModelDt* ModelDt;
			CYyMotionMan field_24;
			D3DXVECTOR4 field_B0;
			int field_C0;
			CYyTex* field_C4;
		};
	}
}