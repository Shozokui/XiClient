#include "CTkNode.h"
#include "CTkObject.h"

using namespace FFXI::CTk;

FFXI::CTk::CTkNode::CTkNode()
{
	this->field_14 = true;
	this->field_4 = 0;
	this->field_0 = 0;
	this->field_C = 0;
	this->field_8 = 0;
	this->field_10 = 0;
}

CTkObject* FFXI::CTk::CTkNode::GetTkObject()
{
	return this->field_10;
}
