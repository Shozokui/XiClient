#pragma once

namespace FFXI {
	namespace Constants {
		class DatIndices {
		public:
			static const unsigned short TEX_OpeningMovieSubs_JP = 0x9A6B;
			static const unsigned short TEX_OpeningMovieSubs_GE = 0xD9C3;
			static const unsigned short TEX_OpeningMovieSubs_FR = 0xDB67;
			static const unsigned short TEX_Icons1 = 0x9A7F;
			static const unsigned short TEX_Icons2 = 0x9A88;
			static const unsigned short DMSG_GermanArticles = 0xD8C9;
			static const unsigned short DMSG_FrenchArticles = 0xD8CA;
			static const unsigned short DMSG_ZoneFullNames = 0xD8A9;
			static const unsigned short DMSG_ZoneShortNames = 0xD8AA;
			static const unsigned short DMSG_GearSlotNames = 0xD8AF;
			static const unsigned short DMSG_JobShortNames = 0xD8AC;
			static const unsigned short DMSG_OdinFightMobs = 0xD8B0;

			//Language dependent dats
			static const unsigned int JapaneseDatTable[110];
			static const unsigned int EnglishDatTable[110];
			static const unsigned int FrenchDatTable[110];
			static const unsigned int GermanDatTable[110];

			//Model related dats
			static const unsigned int base_skeleton_no_tab[];
			static const unsigned int mot_waist_type1[18];
			static const unsigned int mot_waist_type2[18];
			static const unsigned int mot_waist_typeX[18];
			static const unsigned int mot_upperbody_type1[10];
			static const unsigned int mot_upperbody_type2[10];
			static const unsigned int mot_upperbody_typeX[10];
		};
	}
}