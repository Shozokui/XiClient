#pragma once
#include "MmbManager.h"
namespace FFXI {
	namespace CYy {
		class CYyResourceFile;
		class MapChunk;
		class UnderscoreAtStruct;
		class CTsZoneMapSubStruct1 {
		public:
			void MakeChunkList();
			void InitUnderscoreAtStructs();
			void BuildFourCCList();
			void GetChunky(char*, int*);
			void CleanFourCCList();
			int field_0;
			int ZoneID;
			void* data;
			int* field_C;
			int field_10;
			int field_14;
			int field_18;
			int field_1C;
			int field_20;
			unsigned int MapChunkCount;
			MapChunk* MapChunks;
			int field_2C;
			int* field_30;
			int field_34;
			int* field_38;
			UnderscoreAtStruct* UnderscoreAtStructs;
			unsigned int field_40;
			MmbManager MMBManager;
			CYyResourceFile* ZoneDat;
			CYyResourceFile* field_58;
			CYyResourceFile* field_5C;
			int field_60;
			void* field_64;
			int FourCCListSize;
			int* FourCCList;
			int field_70;
			int field_74;
		};
	}
}