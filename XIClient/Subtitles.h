#pragma once
#include "streams.h"
namespace FFXI {
	namespace CTk { class _49SubList; }
	namespace CYy {
		class CMoResource;
		class CYyResourceFile;
		class Subtitles {
		public:
			Subtitles(int);
			~Subtitles();
			void Init();
			void Update();
			CYyResourceFile** field_4;
			CTk::_49SubList*** SubLists;
			int field_C;
			float field_10;
		};
	}
}