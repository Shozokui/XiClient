#pragma once

namespace FFXI {
	namespace CYy {
		class TaskBuff;
		class CYyResourceFile;
		class CMoResource;
	}

	namespace CTk {
		class TKRECT;
		class IwManager {
		public:
			static void IwInit();
			static void IwLobbyInit(bool);
			static void IwPutMes(char**, TKRECT*);
			static int TaskBuffUpdate();
			static void IwDestroy();
			static int IwNetLoginPhase(int, int, int, int, int);
			static int IwGetPolIrcStts();
			static int IwLoadCharInfo();
			static void IwErrorConst(int);
			static void IwSetErrorStringMain(int);
			static void IwOkMenuConst(const char*);
			static void iwfdi();
			static void iwfdi2();
			static void iwfdo();
			static void iwfdo2();
			static void IwSaMainIdle();
			static void IwSaMainIdle2();
			static CYy::CYyResourceFile*** BootTitleDemo(CYy::CYyResourceFile***, int);
			static void IwBootDemo(int);
			static void IwReleaseDemo();
			static char* g_ErrMsg;
			static bool patch_skip;
			static int lobby_result;
			static int polircstts;
			static char SomeByte1;
			static char SomeByte2;
			static char SomeByte3;
			static FFXI::CYy::TaskBuff* task_buff;
			static CYy::CYyResourceFile** RES_1;
			static CYy::CYyResourceFile** title_demo;
		};
	}
}