#pragma once
#define WIN32_LEAN_AND_MEAN
#include "CYyObject.h"
#include "WMatrix.h"
namespace FFXI {
	namespace CYy {
		class CMoSpline : public CYyObject {
		public:
			CMoSpline();
			virtual ~CMoSpline();
			void ReleaseTable();
			void CalcTable();
			void AllocTable(int, int, int, int);
			void Get(float, D3DXVECTOR3*);
			float SomeFunc(float, void*);
			int field_4;
			float field_8;
			void* field_C[3];
			float* field_18;
			FFXI::Math::WMatrix* field_1C;
			float* field_20;
			char field_24;
			char field_25;
			char field_26;
			char field_27;
		};
	}
}