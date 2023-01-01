#include "ResourceList.h"
#include "CYyResourceFile.h"
#include "CMoResourceMng.h"
using namespace FFXI::CYy;

void FFXI::CYy::ResourceList::Init()
{
	if (this->field_14 == nullptr)
		return;

	int v2 = CMoResource::SomeCounter;
	if (this->field_0 != nullptr) {
		CMoResource** nextrpl = this->field_14->PreHeader.NextRPL;
		if (this->field_14->PreHeader.NextRPL == nullptr) {
			this->field_14 = nullptr;
			return;
		}

		this->field_14 = *this->field_14->PreHeader.NextRPL;
		while (this->field_14 != nullptr) {
			if ((this->field_14->Header.Type_Size & 0x7F) == FFXI::Constants::Enums::ResourceType::Rmp) {
				CYyResourceFile* rmp = (CYyResourceFile*)this->field_14;
				if (rmp->field_38 != v2) {
					rmp->field_38 = v2;
					this->field_18 += 1;
				}
				else {
					if (rmp->TerminateNode == nullptr) {
						this->field_14 = nullptr;
					}
					else {
						this->field_14 = *rmp->TerminateNode;
					}
				}
			}
			else if ((this->field_14->Header.Type_Size & 0x7F) == FFXI::Constants::Enums::ResourceType::Terminate) {
				CYyResourceFile* termhead = (CYyResourceFile*)this->field_14->GetHead();
				termhead->field_38 = v2;
				this->field_18 -= 1;
				if (this->field_18 < 0) {
					this->field_14 = nullptr;
					return;
				}
			}

			//LABEL35
			//this->field_14 can be nullptr here as per client
			if ((this->field_14->Header.Type_Size & 0x7F) == this->field_8) {
				if (this->field_C == 0) {
					return;
				}
				else if ((this->field_10 & this->field_14->Header.FourCC) == this->field_C) {
					return;
				}
			}
			if (this->field_14->PreHeader.NextRPL == nullptr) {
				this->field_14 = nullptr;
				return;
			}
			this->field_14 = *this->field_14->PreHeader.NextRPL;
		}
		
	}
	else {
		CMoResource** last = this->field_14->PreHeader.LastResourceCreatedMine;
		if (last == nullptr) {
			this->field_14 = nullptr;
		}
		else {
			this->field_14 = *last;

			if (this->field_C != 0) {
				while (this->field_14 != nullptr) {
					if ((this->field_10 & this->field_14->Header.FourCC) == this->field_C) {
						break;
					}
					if (this->field_14->PreHeader.LastResourceCreatedMine == nullptr) {
						this->field_14 = nullptr;
					}
					else {
						this->field_14 = *this->field_14->PreHeader.LastResourceCreatedMine;
					}
				}
			}
		}
	}
}

void FFXI::CYy::ResourceList::Clear()
{
	this->field_0 = nullptr;
	this->field_4 = nullptr;
	this->field_8 = 0;
}

void FFXI::CYy::ResourceList::PrepareFromResource(CMoResource* a2, int a3, int a4, int a5)
{
	CMoResource::SomeCounter += 1;
	this->field_8 = a3;
	this->field_0 = a2;
	this->field_4 = nullptr;
	this->field_C = a5 & a4;
	this->field_10 = a5;
	this->field_14 = a2;
	int type = a2->Header.Type_Size & 0x7F;
	this->field_18 = -(type != FFXI::Constants::Enums::ResourceType::Rmp);
	if (!this->Ready())
		this->Init();
}

void FFXI::CYy::ResourceList::PrepareFromLastCreated(CMoResourceMng* a2, int a3, int a4, int a5)
{
	++CMoResource::SomeCounter;
	this->field_4 = a2;
	this->field_0 = 0;
	this->field_8 = a3;
	this->field_C = a5 & a4;
	this->field_10 = a5;
	this->field_18 = 0;
	CMoResource** v6 = CMoResourceMng::LastResourceCreatedByType[a3];
	CMoResource* v7{ nullptr };
	if (v6)
		v7 = *v6;
	this->field_14 = v7;
	if (!this->Ready())
		this->Init();
}

bool FFXI::CYy::ResourceList::Ready()
{
	if (this->field_14 == nullptr)
		return false;

	int type = (this->field_14->Header.Type_Size & 0x7F);
	if (this->field_C) {
		if (this->field_8 == type) {
			if ((this->field_10 & this->field_14->Header.FourCC) == this->field_C)
				return true;
		}
	}
	else if (this->field_8 == type)
		return true;

	return false;
}

CMoResource* FFXI::CYy::ResourceList::GetNext(bool a2)
{
	CMoResource* v2 = this->field_14;
	this->Init();
	return v2;
}
