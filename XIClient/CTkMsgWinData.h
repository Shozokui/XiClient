#pragma once

namespace FFXI {
	class CTkMsgWinData {
	public:
		CTkMsgWinData();
		static int fsGetFontWidth(unsigned char*);
		static int fsGetStrWidth(unsigned char*);
		static unsigned char SomeValues[8];
		void InitAll();
		void Init();
		int field_0;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		__int16 field_14;
		__int16 field_16;
		__int16 field_18;
		__int16 field_1A;
		int field_1C;
		char field_20[0x32000];
		char field_32020[0x32000];
		int field_64020;
		int field_64024;
		int field_64028;
		int field_6402C;
		int field_64030;
		int field_64034;
		int field_64038;
		int field_6403C;
		int field_64040;
		int field_64044;
		char field_64048;
		char field_64049;
		__int16 field_6404A;
		__int16 field_6404C;
		__int16 field_6404E;
	};
}