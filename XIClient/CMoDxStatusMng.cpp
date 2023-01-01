#include "CMoDxStatusMng.h"
using namespace FFXI::CYy;

const CYyClass CMoDxStatusMng::CMoDxStatusMngClass{
	"CMoDxStatusMng", sizeof(CMoDxStatusMng), &CYyObject::CYyObjectClass
};

const CYyClass* FFXI::CYy::CMoDxStatusMng::GetRuntimeClass()
{
	return &CMoDxStatusMng::CMoDxStatusMngClass;
}

FFXI::CYy::CMoDxStatusMng::CMoDxStatusMng()
{
	//nullsub
}

FFXI::CYy::CMoDxStatusMng::~CMoDxStatusMng()
{
	StatusNode* node = this->field_4.field_8;
	while (node) {
		StatusNode* v3 = node->field_8;
		node->~StatusNode();
		node = v3;
	}
}
