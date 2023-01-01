#pragma once
#include "DMsg.h"
#include "XIString.h"
#include "Enums.h"

namespace FFXI {
	namespace Text {
		struct DMsgTableEntry {
			int PageIndex;
			DMsg* DataPointer;
		};

		extern char* XIStrTable[];
		
		//Not used?
		extern DMsg GermanArticles, FrenchArticles;

		//Part of DMsgTable
		extern DMsg JobShortNames, ZoneShortNames, ZoneFullNames, Menu1, Menu2, GearSlotNames, OdinFightMobs;
		extern DMsg MiscMenuText, Errors, CharSelectText, CharSelectRaceGender, JobFullNames;
		extern DMsgTableEntry DMsgTables[];

		extern char* XiStrGet(int PageIndex, int EntryIndex);
		extern void CleanStrTables();
		extern bool LoadStringTables();
		extern void InitStrPages();
	}
}