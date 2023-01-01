#include "CMoOT.h"
#include "OTStruct.h"
#include "Globals.h"
using namespace FFXI::CYy;

const CYyClass FFXI::CYy::CMoOT::CMoOTClass = {
    "CMoOT", sizeof(CMoOT), &CYyObject::CYyObjectClass
};

D3DMATERIAL8 CMoOT::Material{};
float CMoOT::FogStart{};
float CMoOT::FogEnd{};
D3DCOLOR CMoOT::FogColor{};
D3DCOLOR CMoOT::Ambient{};

const CYyClass* FFXI::CYy::CMoOT::GetRuntimeClass()
{
    return &CMoOT::CMoOTClass;
}

int ot_local_counter{ 0 };

FFXI::CYy::CMoOT::CMoOT()
{
    this->field_4 = nullptr;
}

FFXI::CYy::CMoOT::~CMoOT()
{
    if (this->field_4 != nullptr) {
        CYyObject::Unwrap(this->field_4);
        this->field_4 = nullptr;
    }
}

void FFXI::CYy::CMoOT::Init(int a2, float a3, float a4)
{
    int plusone = a2 + 1;
    this->field_8 = a2;
    this->field_C = a3;
    this->field_10 = a4;
    this->field_14 = (double)plusone / (a4 - a3);
    this->field_4 = (OTStruct*)CYyObject::Wrap(sizeof(OTStruct) * plusone, FFXI::Constants::Enums::MEM_MODE::Ex);
    this->InitOtStructs();

    if (this->field_8 < 0) {
        return;
    }

    double v11 = (a4 - a3) / (double)(this->field_8);
    for (int i = 0; i <= this->field_8; ++i) {
        this->field_4[i].field_0 = (double)i * v11;
        this->field_4[i].Callback = OTStruct::DefaultCallback;
    }
}

void FFXI::CYy::CMoOT::InitOtStructs()
{
    if (this->field_4 != nullptr) {
        if (this->field_8 != 0) {
            for (int i = this->field_8; i; i--) {
                this->field_4[i].previous = this->field_4 + (i - 1);
                this->field_4[i].next = this->field_4 + (i + 1);
                this->field_4[i].field_14 = 0;
            }
        }

        this->field_4[0].next = this->field_4 + 1;
        this->field_4[0].previous = nullptr;
        this->field_4[0].field_14 = 0;
        this->field_4[this->field_8].next = nullptr;
    }

    ot_local_counter = 0;
}

void FFXI::CYy::CMoOT::Insert(OTStruct* a2)
{
    if (Globals::CheckFloat(a2->field_0) == false) {
        return;
    }

    if (a2->field_14 != 0) {
        a2->field_14 += 1;
        return;
    }

    double v5{};
    if (a2->field_0 >= this->field_C) {
        if (a2->field_0 <= this->field_10) {
            v5 = a2->field_0;
        }
        else {
            v5 = this->field_10;
        }
    }
    else {
        v5 = this->field_C;
    }

    float a2a = v5 - this->field_C;
    float v16 = a2a * this->field_14 + 8388608.0;
    int b = *reinterpret_cast<int*>(&v16);
    b -= 0x4AFFFFFF;
    if (b > this->field_8) {
        b = this->field_8;
    }
    OTStruct* ots = this->field_4 + b;
    OTStruct* prev = ots->previous;
    while (prev != nullptr) {
        if (prev->field_0 < a2a) {
            break;
        }
        ots = prev;
        prev = prev->previous;
    }

    a2->next = ots;
    a2->previous = prev;
    if (ots->previous != nullptr) {
        ots->previous->next = a2;
    }
    ots->previous = a2;
    a2->field_14 += 1;
    ot_local_counter += 1;
}
