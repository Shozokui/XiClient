#include "XiColorEnv.h"
#include "Globals.h"
using namespace FFXI;

void FFXI::XiColorEnv::Combine(XiColorEnv* a2, XiColorEnv* a3, float a4)
{
	Globals::MixByteFractionals(this->field_0, a2->field_0, a3->field_0, a4);
	Globals::MixByteFractionals(this->field_0 + 4, a2->field_0 + 4, a3->field_0 + 4, a4);
	Globals::MixByteFractionals(this->field_0 + 8, a2->field_0 + 8, a3->field_0 + 8, a4);
	Globals::MixByteFractionals(this->field_0 + 12, a2->field_0 + 12, a3->field_0 + 12, a4);
	Globals::MixFloats(&this->field_10, a2->field_10, a3->field_10, a4);
	Globals::MixFloats(&this->field_14, a2->field_14, a3->field_14, a4);
	Globals::MixFloats(&this->field_18, a2->field_18, a3->field_18, a4);
}
