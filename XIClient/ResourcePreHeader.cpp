#include "ResourcePreHeader.h"
#include "CMoResource.h"

using namespace FFXI::CYy;

void ResourcePreHeader::Init() {
	this->field_14 = 0x8000;
	this->PreviousRPL = 0;
	this->NextRPL = 0;
	this->WeirdPointer1 = 0;
	this->LastResourceCreatedMine = 0;
	this->field_16 = 0;
	this->field_18 = 0;
}

void ResourcePreHeader::SetNextRPL(CMoResource* p_res) {
	if (!p_res) return;

	this->NextRPL = p_res->PreHeader.PointerToThisInRPL;
}

void FFXI::CYy::ResourcePreHeader::SetPrevRPL(CMoResource* p_res)
{
	if (!p_res) return;
	this->PreviousRPL = p_res->PreHeader.PointerToThisInRPL;
}
