#pragma once

namespace FFXI {
	namespace Input {
		class FsKeyboard {
		public:
			//Setters
			void SetFlag1(unsigned char);
			void SetFlag2(unsigned char);
			void SetFlag4(unsigned char);
			void SetFlag8(unsigned char);
			void SetFlag10(unsigned char);

			//Checkers
			bool rept(unsigned char);
			bool just(unsigned char);
			bool CheckFlag4(unsigned char);
			bool CheckFlag8(unsigned char);
			bool CheckFlag10(unsigned char);

			//Clearers
			bool reptoff(unsigned char);
			bool justoff(unsigned char);
			void ClearFlags821(unsigned char);
			void ClearFlags(unsigned char);

			char KeyFlags[256];
			bool field_100;
		};
	}
}