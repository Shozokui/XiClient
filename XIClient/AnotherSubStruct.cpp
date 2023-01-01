#include "AnotherSubStruct.h"
#include "Globals.h"
#include "StorageLists.h"
#include "CMoPathObject.h"
#include "CMoResource.h"
#include "CMoCamera.h"
using namespace FFXI::CYy;

int AnotherSubStruct::camnum{};
FFXI::CYy::AnotherSubStruct::AnotherSubStruct()
{
	this->field_C = 0;
	this->field_14 = 0;
}

FFXI::CYy::AnotherSubStruct::~AnotherSubStruct()
{
	if (this->field_14 != nullptr) {
		this->field_14->field_54.~CMoSpline();
		this->field_14->field_2C.~CMoSpline();
		this->field_14->field_4.~CMoSpline();
		StorageLists::instance->Delete(this->field_14);
		this->field_14 = nullptr;
	}
}

void FFXI::CYy::AnotherSubStruct::Reset()
{
	this->field_C = 0.0;

	if (this->field_14 != nullptr) {
		this->field_14->field_54.~CMoSpline();
		this->field_14->field_2C.~CMoSpline();
		this->field_14->field_4.~CMoSpline();
		StorageLists::instance->Delete(this->field_14);
		this->field_14 = nullptr;
	}
}

void FFXI::CYy::AnotherSubStruct::InitPath(CMoResource** a2)
{
	if (this->field_14 != nullptr) {
		this->field_14->field_54.~CMoSpline();
		this->field_14->field_2C.~CMoSpline();
		this->field_14->field_4.~CMoSpline();
		StorageLists::instance->Delete(this->field_14);
		this->field_14 = nullptr;
	}

	(*a2)->FindResourceUnder((CMoResource***) &this->field_4, FFXI::Constants::Enums::ResourceType::Camera, (AnotherSubStruct::camnum << 24) + '000c');
	if (this->field_4 == nullptr) {
		(*a2)->FindResourceUnder((CMoResource***) &this->field_4, FFXI::Constants::Enums::ResourceType::Camera, '000c');
		if (this->field_4 == nullptr)
			return;
	}

	this->field_14 = CMoPathObject::Create((unsigned int*)(*this->field_4)->Data);
	this->field_8 = 0;
	this->field_C = this->field_14->field_4.field_8;
}
