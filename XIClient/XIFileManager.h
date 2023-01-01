#pragma once
#include "XIFileImpl.h"
#include <cstdint>
#include <wtypes.h>
namespace FFXI {
	namespace File {
		class XiFileStat;
		class XIFileManager {
		public:
			static XIFileManager* g_pXIFileManager;
			static bool fsInitFileSystem(int, LPCSTR);
			static void fsUninitFileSystem();
			static void GetMem(char**, uint32_t);
			virtual ~XIFileManager();
			XIFileManager(int, int);
			int fileSystemInit(LPCSTR, int);
			void initffxiuserPath(LPCSTR);
			int ReadFilenameNow(char*, char*, int, int);
			int ReadNumfileNow(int, char*, int, int);
			int readUserFileNow(const char*, char*, int, int, int);
			int readFile(int, char*, int, int);
			bool isReadFile(int, char*, int);
			int GetNumFileStat(int, XiFileStat*);
			bool isGetStat(int);
			int GetFileSizeByNumfile(int);
			char* GetVersionString();
			XIFileImpl* FileImpl;
			int FsMode;
			int DiskMode;
			char FFXIVersionString[11];
			char field_1B;
			char field_1C;
			char field_1D;
			char field_1E;
			char field_1F;
			char field_20;
			char field_21;
			char field_22;
			char field_23;
			char field_24;
			char field_25;
			char field_26;
			char field_27;
			char field_28;
			char field_29;
			char field_2A;
			char field_2B;
			char field_2C;
			char field_2D;
			char field_2E;
			char field_2F;
			char field_30;
			char field_31;
			char field_32;
			char field_33;
			char field_34;
			char field_35;
			char field_36;
			char field_37;
			char field_38;
			char field_39;
			char field_3A;
			char field_3B;
			char field_3C;
			char field_3D;
			char field_3E;
			char field_3F;
			char field_40;
			char field_41;
			char field_42;
			char field_43;
			char field_44;
			char field_45;
			char field_46;
			char field_47;
			char field_48;
			char field_49;
			char field_4A;
			char field_4B;
			char field_4C;
			char field_4D;
			char field_4E;
			char field_4F;
			char field_50;
			char field_51;
			char field_52;
			char field_53;
			char field_54;
			char field_55;
			char field_56;
			char field_57;
			char field_58;
			char field_59;
			char field_5A;
			char field_5B;
			char field_5C;
			char field_5D;
			char field_5E;
			char field_5F;
			char field_60;
			char field_61;
			char field_62;
			char field_63;
			char field_64;
			char field_65;
			char field_66;
			char field_67;
			char field_68;
			char field_69;
			char field_6A;
			char field_6B;
			char field_6C;
			char field_6D;
			char field_6E;
			char field_6F;
			char field_70;
			char field_71;
			char field_72;
			char field_73;
			char field_74;
			char field_75;
			char field_76;
			char field_77;
			char field_78;
			char field_79;
			char field_7A;
			char field_7B;
			char field_7C;
			char field_7D;
			char field_7E;
			char field_7F;
			char field_80;
			char field_81;
			char field_82;
			char field_83;
			char field_84;
			char field_85;
			char field_86;
			char field_87;
			char field_88;
			char field_89;
			char field_8A;
			char field_8B;
			char field_8C;
			char field_8D;
			char field_8E;
			char field_8F;
		};

	}
}