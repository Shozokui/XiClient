#include "CYyNode.h"
using namespace FFXI::CYy;

const CYyClass FFXI::CYy::CYyNode::CYyNodeClass = {
	"CYyNode", sizeof(CYyNode), &CYyObject::CYyObjectClass
};

const CYyClass* FFXI::CYy::CYyNode::GetRuntimeClass()
{
	return &CYyNode::CYyNodeClass;
}

FFXI::CYy::CYyNode::CYyNode()
{
	this->field_4 = nullptr;
	this->field_8 = nullptr;
}

FFXI::CYy::CYyNode::~CYyNode()
{
	//nullsub
}

void FFXI::CYy::CYyNode::Append(CYyNode** a2)
{
	if (*a2 == nullptr) {
		*a2 = this;
	}
	else {
		CYyNode* nextfree = this->GetNextFreeSpot();
		nextfree->field_4 = this;
		this->field_8 = nextfree;
	}
}

CYyNode* FFXI::CYy::CYyNode::GetNextFreeSpot()
{
	if (this == nullptr)
		return nullptr;

	CYyNode* last = this;
	while (last->field_4 != nullptr) {
		last = last->field_4;
	}
	return last;
}
