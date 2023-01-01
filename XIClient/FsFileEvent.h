#pragma once

namespace FFXI {
	namespace File {
		class FsFileEvent {
		public:
			enum class EventType {
				Null = 0,
				Read = 1,
				ReadEX = 2,
				Write = 3,
				GetFileSize = 4,
				GetStats = 5,
				Remove = 6,
				Exists = 7,
				Unknown = 8,
				Wait = 9,
				Dunno = 10,
				Exit = 11

			};
			FsFileEvent();
			FsFileEvent(FsFileEvent*);
			EventType Type;
			char FileName[256];
			int BufferOffset;
			void* Buffer;
			int BufferSize;
			int field_114;
			int SomeFunction;
			void(__cdecl* CallBack)(char*, int, char*);
			char* FileNameJustRead;
		};
	}
}