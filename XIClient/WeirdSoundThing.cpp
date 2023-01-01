#include "WeirdSoundThing.h"

FFXI::WeirdSoundThing::WeirdSoundThing()
{
}

FFXI::WeirdSoundThing::~WeirdSoundThing()
{
	Clean();
}

void FFXI::WeirdSoundThing::Clean()
{
	if (this->field_4) {
		delete this->field_4;
		this->field_4 = nullptr;
	}

	if (this->field_8) {
		delete this->field_4;
		this->field_4 = nullptr;
	}

	if (this->field_C) {
		delete this->field_4;
		this->field_4 = nullptr;
	}
}
