#pragma once
#include "CMoResource.h"
#include "d3dx9math.h"

namespace FFXI {
	namespace CYy {
		class CYySepRes : public CMoResource {
		public:
			static int Calc3D(float *, int*, D3DXVECTOR3*, float, float, float, unsigned char, int);
			const static CYyClass CYySepResClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			virtual void Close() override final;
			CYySepRes() = default;
			virtual ~CYySepRes() = default;
			int Play(int, int);
			int Play(float*, int);
			void Stop(int);
			bool CheckFourCC();
			int field_30;
			int field_34;
			int field_38;
			unsigned int field_3C;
		};
	}
}