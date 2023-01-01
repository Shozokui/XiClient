#pragma once
#include "StorageListObject.h"
namespace FFXI {
	namespace CYy {
		class Subtitles; class CXiMovie;
		class CXiOpening : public StorageListObject {
		public:
			const static CYyClass CXiOpeningClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CXiOpening(char*);
			virtual ~CXiOpening();
			static int MovieFileIndex;
			static CXiMovie* XiMovie;
			static void DrawMovie(int, int, int, int, float*);
			static void CleanXiMovie();
			bool IsFinished();
			void Update();
			char MoviePath[260];
			int State;
			char field_10C;
			char field_10D;
			char field_10E;
			char field_10F;
			int field_110;
			float field_114;
			float field_118;
			float field_11C;
			Subtitles* Subtitle;
			int field_124;
		};
	}
}