#pragma once
#include <cstdint>
#include <wtypes.h>
namespace FFXI {
	namespace File {
		class XiFileStat;
		class FsFileThreadManager;
		class XIFileImpl {
		public:
			virtual ~XIFileImpl();
			XIFileImpl(int, int);
			int Init(LPCSTR, int);
			//Load vtable/ftable
			int readNumFile();
			int ReadFilenameNow(const char*, char*, int, int);
			//Read a file immediately without adding an event to the queue
			int ReadNumfileNow(int, char*, int, int);
			int readUserFileNow(const char*, char*, int, int, int);
			int getUserFileName(const char*, char*, int, int);
			//Add a new read event to the queue
			int readFile(int, char*, int, int);
			//Check if a read event already exists in the queue
			bool isReadFile(int, char*, int);
			int GetFileSizeByIndex(int);
			int GetFileSizeByName(LPCSTR);
			int GetFileName(int, char*, int);
			int GetFullName(char*, int, LPCSTR);
			int GetNumFileStat(int, XiFileStat*);
			bool isGetStat(int);
			int strcpy(char*, int, const char*);
			int FsMode;
			int DiskMode;
			int* SomeArray1[14];
			int field_44;
			int SomeArray2[14];
			int MaxROMIndexLoaded;
			int SomeArray3[14];
			char* VTableData;
			int VTableSize;
			unsigned short* FTableData;
			int TableEntryCount;
			int FTableSize;
			char CurrentDirectory[256];
			char field_1D0;
			char field_1D1;
			char field_1D2;
			char field_1D3;
			char field_1D4;
			char field_1D5;
			char field_1D6;
			char field_1D7;
			char field_1D8;
			char field_1D9;
			char field_1DA;
			char field_1DB;
			char field_1DC;
			char field_1DD;
			char field_1DE;
			char field_1DF;
			char field_1E0;
			char field_1E1;
			char field_1E2;
			char field_1E3;
			char field_1E4;
			char field_1E5;
			char field_1E6;
			char field_1E7;
			char field_1E8;
			char field_1E9;
			char field_1EA;
			char field_1EB;
			char field_1EC;
			char field_1ED;
			char field_1EE;
			char field_1EF;
			FsFileThreadManager* ThreadManager;
			char UserPath[256];
			char TempPath[256];
			char SystemPath[256];
			char ffxiuserPath[256];
		};
	}
}