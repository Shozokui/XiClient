#pragma once
namespace FFXI {
	namespace CYy {
		class CMoKeyframe;
		class CYyResourceFile;
		class WeatherResource {
		public:
			int FourCC;
			CYyResourceFile* field_4;
			CMoKeyframe** Keyframes[32];
			int KeyframeCount;
		};
	}
}