#pragma once

namespace FFXI {
	namespace Text {
		class DMsg {
		public:
			~DMsg();
			static bool LoadFromFile(DMsg*, int);
			void LoadFromData(char*, int);
			void Parse(char*, int);
			char* GetEntry0(int);
			char* GetEntry(int, int);
			char* GetTable(int);
			static char* GetTableEntry(char*, int);
			char* field_0;
			char* Buffer;
			char* EntryListPos;
			char* FirstEntryPos;
		};
	}
}