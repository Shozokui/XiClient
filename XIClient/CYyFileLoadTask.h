#pragma once
#include "CMoTask.h"
namespace FFXI {
	namespace File { class XiFileStat; }
	namespace CYy {
		class CYyResourceFile;
		class CYyFileLoadTask : public CMoTask {
		public:
			virtual void VObj2(int*) override final;
			virtual void OptionalMove() override final;
			CYyFileLoadTask() = default;
			virtual ~CYyFileLoadTask();
			void Init(int);
			int VTableIndex;
			char* FileDataBuffer;
			char AbortFlag;
			CYyResourceFile** ResourceFile;
			int State;
			File::XiFileStat* FileStat;
			char field_49;
			char field_4A;
			char field_4B;
		};
	}
}