#pragma once
namespace FFXI {
	namespace CYy { class CYyResourceFile; }
	namespace CTk { class _49SubList;	}
	namespace Yk {
		class YkManager {
		public:
			static char SomeChar;
			static FFXI::CTk::_49SubList*** g_shape[18];
			static CYy::CYyResourceFile** SomeFile;
			static void YkInit();
			static void YkEnd();
			static void YkCommonInit();
			static FFXI::CTk::_49SubList*** GetMenuRes(int);
		};
	}
}