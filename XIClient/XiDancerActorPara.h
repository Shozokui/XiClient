#pragma once
namespace FFXI {
	namespace CYy {
		class XiDancerActorPara {
		public:
			XiDancerActorPara(char*, short*);
			char Name[16];
			short Equipment[9];
			short RaceGender;
			char Size;
			char field_25;
			char field_26;
			char field_27;
		};
	}
}