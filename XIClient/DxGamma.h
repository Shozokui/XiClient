#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class DxGamma  {
		public:
			virtual ~DxGamma();
			DxGamma();
			int Init();
			void SetGamma();
			void GetTriplet(float*, float*, float*);
			void SetTriplet(float, float, float);
			void GetLightMultipliers(float*, float*, float*);
			D3DGAMMARAMP GammaRamp{};
			float field_604{ 0 };
			float field_608{ 0 };
			float field_60C{ 0 };
			float RedOffset{ 0 };
			float GreenOffset{ 0 };
			float BlueOffset{ 0 };
		};
	}
}