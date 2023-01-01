#pragma once
#include <cstdint>
namespace FFXI {
	namespace Constants {
		class Values {
		public:
			//Network
			const static uint16_t PORT_LOBBY = 54001;
			const static uint32_t LOBBY_PROTOCOL_TAG = 'FFXI';

			const static uint8_t INDEX_ROM_MAX = 13;
			const static uint8_t INDEX_SOUND_MAX = 16;
			const static uint8_t INDEX_SOUND_REQUEST_MAX = 12;
			const static uint8_t INDEX_SOUND_REPORT_MAX = 64;

			const static uint8_t COUNT_CURSOR = 18;
			const static uint8_t COUNT_RESOURCE_TYPES = 97;
			const static uint8_t COUNT_MENU_LAYERS = 9;

			const static uint8_t SJIS_NEWLINE_CHAR = '\x0A';

			//floats defined in Floats.cpp
			const static float ANGLE_PI;
			const static float ANGLE_2PI;
			const static float ANGLE_PI_OVER_2;
			const static float ANGLE_3PI_OVER_2;
			const static float ANGLE_7PI_OVER_8;
			const static float ANGLE_5PI_OVER_8;
			const static float ANGLE_3PI_OVER_8;
			const static float ANGLE_PI_OVER_8;
			const static float ANGLE_PI_OVER_3;

			const static float ANGLE_MINUS_PI;
			const static float ANGLE_MINUS_2PI;
			const static float ANGLE_MINUS_PI_OVER_2;
			const static float ANGLE_MINUS_7PI_OVER_8;
			const static float ANGLE_MINUS_5PI_OVER_8;
			const static float ANGLE_MINUS_3PI_OVER_8;
			const static float ANGLE_MINUS_PI_OVER_8;

		};
	}
}
