#pragma once

namespace FFXI {
	namespace Text {
		class XIString {
		public:
			static bool LoadFromFile(int, int);
			static char* GetEntry(char*, int);

			XIString();
			XIString(const char*);
			XIString(const char*, int);
			~XIString();
			void AppendStrLen(const char*, int);
			void AppendStr(const char*);
			const char* c_str();
			void __as(const char*);
			void __asLen(const char*, int);
			void ClearText();
			char* Text;
			int TextLength;
			int BufferLength;
		};
	}
}