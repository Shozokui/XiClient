#include "CAcc.h"
#include <iostream>
using namespace FFXI::CYy;

const CYyClass CAcc::CAccClass{
	"CAcc", sizeof(CAcc), &CYyObject::CYyObjectClass
};

const CYyClass* CAcc::GetRuntimeClass()
{
	return &CAccClass;
}

CAcc::CAcc()
{
	this->SomeTexture = nullptr;
	this->Surface = nullptr;
	this->Texture2 = nullptr;
}

CAcc::~CAcc() {
	if (this->SomeTexture) {
		this->SomeTexture->Release();
		this->SomeTexture = 0;
	}
	if (this->Surface) {
		this->Surface->Release();
		this->Surface = 0;
	}
	if (this->Texture2) {
		this->Texture2->Release();
		this->Texture2 = 0;
	}
}