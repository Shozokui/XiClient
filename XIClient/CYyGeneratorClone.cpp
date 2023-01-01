#include "CYyGeneratorClone.h"
#include "CYyDb.h"
#include "CMoResourceMng.h"
#include "GeneratorTask.h"
using namespace FFXI::CYy;

const CYyClass CYyGeneratorClone::CYyGeneratorCloneClass = {
    "CYyGeneratorClone", sizeof(CYyGeneratorClone), &CMoResource::CMoResourceClass
};

const CYyClass* FFXI::CYy::CYyGeneratorClone::GetRuntimeClass()
{
    return &CYyGeneratorClone::CYyGeneratorCloneClass;
}

void FFXI::CYy::CYyGeneratorClone::VObj2(int* a2)
{
    if (*a2 == 1 || *a2 == 4) {
       this->DeactivateClone();
    }
}

const CYyClass* FFXI::CYy::CYyGeneratorClone::AttachedTo()
{
    return this->GetRuntimeClass();
}

FFXI::CYy::CYyGeneratorClone::~CYyGeneratorClone()
{
    this->KillAll(nullptr);
    this->CMoAttachments::ClearCasterTarget();
    if (this->field_A8 != nullptr) {
        delete this->field_A8;
        this->field_A8 = 0;
    }

    if (this->field_F8 != 0) {
        exit(0x10053383);
    }

    CYyDb::g_pCYyDb->pCMoResourceMng->RemoveRes(this->PreHeader.PointerToThisInRPL);
}

void FFXI::CYy::CYyGeneratorClone::DeactivateClone()
{
    this->KillAll(nullptr);
    this->CMoAttachments::ClearCasterTarget();
    if (this->field_A8 != nullptr) {
        delete this->field_A8;
        this->field_A8 = nullptr;
    }
    while ((this->PreHeader.field_14 & 0x7FFF) != 0) {
        this->SomeClean3();
    }

    CYyDb::g_pCYyDb->pCMoResourceMng->Unlink(this->PreHeader.PointerToThisInRPL);
}

bool FFXI::CYy::CYyGeneratorClone::VirtAttach3()
{
    this->DeactivateClone();
    return true;
}
bool FFXI::CYy::CYyGeneratorClone::VirtAttach4()
{
    this->DeactivateClone();
    return true;
}

CYyObject* FFXI::CYy::CYyGeneratorClone::GetCYyObject()
{
    return this;
}
