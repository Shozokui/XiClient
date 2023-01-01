#include "CMoSphRes.h"

const FFXI::CYy::CYyClass FFXI::CYy::CMoSphRes::CMoSphResClass{
	"CMoSphRes", sizeof(CMoSphRes), &FFXI::CYy::CMoResource::CMoResourceClass
};

const FFXI::CYy::CYyClass* FFXI::CYy::CMoSphRes::GetRuntimeClass()
{
	return &CMoSphRes::CMoSphResClass;
}

void FFXI::CYy::CMoSphRes::Open()
{
	this->path.field_10 = (float*)((char*)this->path.field_10 + (int)&this->path);
	this->path.field_14 = (int*)((char*)this->path.field_14 + (int)&this->path);
	this->path.field_18 += (int)&this->path;
	this->path.field_1C += (int)&this->path;
}

void FFXI::CYy::CMoSphRes::Close()
{
	//nullsub
}

void FFXI::CYy::CMoSphRes::VObj6()
{
	//nullsub
}
