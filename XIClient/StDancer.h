#pragma once
namespace FFXI {
	namespace CTk {
		class StDancer {
		public:
			static bool isdanceractive;
			StDancer();
			~StDancer();
			bool IsCheatActive(int);
			void Update();
			void SetVisible(int, int, int, int);
			int field_0;
			int field_4;
			int field_8;
			int field_C;
			int field_10;
			int field_14;
			int field_18;
			int field_1C;
			int field_20;
		};
	}
}