#pragma once
namespace FFXI {
	namespace Config {
		class FsCallbackListLink;
		class FsConfigSubject {
		public:
			enum type {
				TIG = 6
			};
			FsConfigSubject();
			int Index;
			int field_8;
			int type;
			int field_10;
			int field_14;
			int field_18;
			int field_1C;
			int field_20;
			FsCallbackListLink* field_24;
			int field_28;
		};
	}
}