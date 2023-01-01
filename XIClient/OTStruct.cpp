#include "OTStruct.h"
#include <stdlib.h>

void __cdecl NullCallback(FFXI::CYy::CMoTask*) {
	//nullsub
}

void(__cdecl* FFXI::OTStruct::DefaultCallback)(FFXI::CYy::CMoTask*) = NullCallback;

FFXI::OTStruct::OTStruct()
{
	this->field_14 = 0;
	this->next = nullptr;
	this->previous = nullptr;
}
FFXI::OTStruct::~OTStruct()
{
	if (this->field_14 == 0)
		return;

	if (this->previous != nullptr) {
		this->previous->next = this->next;
	}

	if (this->next != nullptr) {
		this->next->previous = this->previous;
	}
}
