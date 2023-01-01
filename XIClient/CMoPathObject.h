#pragma once
#include "CYyObject.h"
#include "CMoSpline.h"
#include "d3dx9math.h"
namespace FFXI {
	namespace CYy {
		class CMoPathObject : public CYyObject {
		public:
			static CMoPathObject* Create(unsigned int*);
			void GetSplines(float, D3DXVECTOR3*, D3DXVECTOR3*, D3DXVECTOR3*);
			CMoSpline field_4;
			CMoSpline field_2C;
			CMoSpline field_54;
		};
	}
}