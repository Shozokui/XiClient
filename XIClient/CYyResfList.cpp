#include "CYyResfList.h"
#include "CYyResourceFile.h"
using namespace FFXI::CYy;

const CYyClass FFXI::CYy::CYyResfList::CYyResfListClass = {
    "CYyResfList", sizeof(CYyResfList), &CYyNode::CYyNodeClass
};

const CYyClass* FFXI::CYy::CYyResfList::GetRuntimeClass()
{
    return nullptr;
}

FFXI::CYy::CYyResfList::CYyResfList()
{
    this->field_10 = -1;
    this->field_18 = 0;
    this->field_1C = nullptr;
    this->field_20 = false;
}

FFXI::CYy::CYyResfList::~CYyResfList()
{
    if (this->field_20 == false)
        return;

    if (this->field_1C == nullptr)
        return;

    (*this->field_1C)->SomeClean3();
    CMoResource::UnlinkFromManager((CMoResource***) & this->field_1C);
}
