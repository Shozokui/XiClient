#pragma once
namespace FFXI {
	class XiColorEnv {
	public:
		void Combine(XiColorEnv*, XiColorEnv*, float);
		unsigned char field_0[16];
		float field_10;
		float field_14;
		float field_18;
		int field_1C;
	};
}