#include "CAcc2.h"
#include <iostream>
using namespace FFXI::CYy;

const CYyClass CAcc2::CAcc2Class{
	"CAcc2", sizeof(CAcc2), &CYyObject::CYyObjectClass
};


const CYyClass* CAcc2::GetRuntimeClass()
{
	return &CAcc2Class;
}

CAcc2::~CAcc2()
{
	if (this->Texture[0]) {
		this->Texture[0]->Release();
		this->Texture[0] = nullptr;

	}
	if (this->Surface) {
		this->Surface->Release();
		this->Surface = nullptr;
	}
	if (this->Texture2) {
		this->Texture2->Release();
		this->Texture2 = nullptr;
	}
	if (this->Texture[1]) {
		this->Texture[1]->Release();
		this->Texture[1] = nullptr;
	}
	if (this->Surface2) {
		this->Surface2->Release();
		this->Surface2 = nullptr;
	}
	if (this->Texture3) {
		this->Texture3->Release();
		this->Texture3 = nullptr;
	}
}

CAcc2::CAcc2()
{
	this->Texture[0] = nullptr;
	this->Surface = nullptr;
	this->Texture2 = nullptr;
	this->Texture[1] = nullptr;
	this->Surface2 = nullptr;
	this->Texture3 = nullptr;
}
