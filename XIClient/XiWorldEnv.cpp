#include "XiWorldEnv.h"
#include "PlaceHolders.h"
#include "Globals.h"
using namespace FFXI;

void FFXI::XiWorldEnv::Combine(XiWorldEnv* a2, XiWorldEnv* a3, float a4)
{
	if (Placeholder::enable_change_focus) {
		throw "not implemented";
		/*this->field_4 = sch_focus_far;
		this->field_10 = sch_focus_far_num;
		this->field_8 = sch_focus_near;
		this->field_11 = sch_focus_near_num;*/
	}
	else {
		Globals::MixFloats(&this->field_4, a2->field_4, a3->field_4, a4);
		Globals::MixFloats(&this->field_8, a2->field_8, a3->field_8, a4);
		Globals::MixOneByteFractional(&this->field_10, a2->field_10, a3->field_10, a4);
		Globals::MixOneByteFractional(&this->field_11, a2->field_11, a3->field_11, a4);
	}
	Globals::MixByteFractionals(&this->field_0, &a2->field_0, &a3->field_0, a4);
	Globals::MixFloats(&this->field_4, a2->field_4, a3->field_4, a4); 
	Globals::MixFloats(&this->field_8, a2->field_8, a3->field_8, a4);
	Globals::MixFloats(&this->field_C, a2->field_C, a3->field_C, a4);
	
	Globals::MixOneByteFractional(&this->field_10, a2->field_10, a3->field_10, a4);
	Globals::MixOneByteFractional(&this->field_11, a2->field_11, a3->field_11, a4);
	Globals::MixOneByteFractional(&this->field_12, a2->field_12, a3->field_12, a4);

	Globals::MixFloats(&this->field_1C, a2->field_1C, a3->field_1C, a4);
	Globals::MixByteFractionals(this->field_20 + 0, a2->field_20 + 0, a3->field_20 + 0, a4);
	Globals::MixByteFractionals(this->field_20 + 4, a2->field_20 + 4, a3->field_20 + 4, a4);
	Globals::MixByteFractionals(this->field_20 + 8, a2->field_20 + 8, a3->field_20 + 8, a4);
	Globals::MixByteFractionals(this->field_20 + 12, a2->field_20 + 12, a3->field_20 + 12, a4);
	Globals::MixByteFractionals(this->field_20 + 16, a2->field_20 + 16, a3->field_20 + 16, a4);
	Globals::MixByteFractionals(this->field_20 + 20, a2->field_20 + 20, a3->field_20 + 20, a4);
	Globals::MixByteFractionals(this->field_20 + 24, a2->field_20 + 24, a3->field_20 + 24, a4);
	Globals::MixByteFractionals(this->field_20 + 28, a2->field_20 + 28, a3->field_20 + 28, a4);

	Globals::MixFloats(&this->field_40, a2->field_40, a3->field_40, a4);
	Globals::MixFloats(&this->field_44, a2->field_44, a3->field_44, a4);
	Globals::MixFloats(&this->field_48, a2->field_48, a3->field_48, a4);
	Globals::MixFloats(&this->field_4C, a2->field_4C, a3->field_4C, a4);
	Globals::MixFloats(&this->field_50, a2->field_50, a3->field_50, a4);
	Globals::MixFloats(&this->field_54, a2->field_54, a3->field_54, a4);
	Globals::MixFloats(&this->field_58, a2->field_58, a3->field_58, a4);
	Globals::MixFloats(&this->field_5C, a2->field_5C, a3->field_5C, a4);
}
