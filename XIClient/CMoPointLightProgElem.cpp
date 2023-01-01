#include "CMoPointLightProgElem.h"
#include "CYyDb.h"
#include "BaseProcessor.h"
#include "CTsZoneMap.h"
#include "BlendStruct.h"
using namespace FFXI::CYy;

const CYyClass FFXI::CYy::CMoPointLightProgElem::CMoPointLightProgElemClass{
	"CMoPointLightProgElem", sizeof(CMoPointLightProgElem), &CMoElem::CMoElemClass
};

const CYyClass* FFXI::CYy::CMoPointLightProgElem::GetRuntimeClass()
{
	return &CMoPointLightProgElem::CMoPointLightProgElemClass;
}

void UpdateLight(int a1, D3DXVECTOR3* a2, FFXI::Util::BlendStruct* a3, float a4, float a5) {
    if (a1 < 0) {
        return;
    }

    if (a1 >= (sizeof(CTsZoneMap::SomeStructs) / sizeof(CTsZoneMap::SomeStructs[0]))) {
        return;
    }

    SomeStruct* ss = CTsZoneMap::SomeStructs + a1;
    ss->field_8.Position = *a2;
    ss->field_8.Range = a5;

    if (a4 <= 0.0) {
        ss->field_8.Diffuse = { 0.0, 0.0, 0.0, 0.0 };
        ss->field_8.Attenuation2 = 1.0;
        ss->field_8.Theta = 0.0;
    }
    else {
        ss->field_8.Diffuse.a = 0.0;
        ss->field_8.Diffuse.r = a3->field_2 * 0.0078125;
        ss->field_8.Diffuse.g = a3->field_1 * 0.0078125;
        ss->field_8.Diffuse.b = a3->field_0 * 0.0078125;
        ss->field_8.Attenuation2 = 1.0 / a4;
        ss->field_8.Theta = a4;
    }
}

int InitLight(int a1, D3DXVECTOR3* a2, FFXI::Util::BlendStruct* a3, float a4, float a5) 
{
    SomeStruct* ss = CTsZoneMap::SomeStructs;
    int index = 0;
    while (ss->field_0 != a1) {
        index += 1;
        ss += 1;
        if (index >= (sizeof(CTsZoneMap::SomeStructs) / sizeof(CTsZoneMap::SomeStructs[0]))) {
            return -1;
        }
    }

    memset(&ss->field_8, 0, sizeof(ss->field_8));

    ss->field_0 = a1;
    ss->field_8.Type = D3DLIGHT_POINT;
    ss->field_8.Diffuse.a = 0.5;
    ss->field_8.Specular = { 0.0, 0.0, 0.0, 0.0 };
    ss->field_8.Ambient = { 0.0, 0.0, 0.0, 0.0 };
    ss->field_8.Diffuse.r = a3->field_2 * 0.0078125;
    ss->field_8.Diffuse.g = a3->field_1 * 0.0078125;
    ss->field_8.Diffuse.b = a3->field_0 * 0.0078125;
    ss->field_8.Position = *a2;
    ss->field_8.Range = a5;
    ss->field_8.Theta = a4;
    ss->field_8.Attenuation2 = 1.0;
    if (a4 != 0.0) {
        ss->field_8.Attenuation2 /= a4;
    }
    return index;
}

FFXI::CYy::CMoPointLightProgElem::CMoPointLightProgElem(int a2, int a3)
{
    this->field_194 = a2;
    this->field_176 = sizeof(CMoPointLightProgElem);
    this->field_1B0 = 0;
    this->field_1AC = -1;
    this->field_198 = a3;
    if ((unsigned __int8)a2 == 99 || (unsigned __int8)a2 == 67)
    {
        this->field_1B1 = 1;
    }
    else
    {
        this->field_1B1 = 0;
    }
}

FFXI::CYy::CMoPointLightProgElem::~CMoPointLightProgElem()
{
    FFXI::Util::BlendStruct v1{};
    this->field_1AC = InitLight(this->field_194, &this->field_54, &v1, 0.0, 0.0);
}

bool FFXI::CYy::CMoPointLightProgElem::VirtElem1(FFXI::Constants::Enums::ElemType a2)
{
    return a2 == FFXI::Constants::Enums::ElemType::PointLight;
}

double FFXI::CYy::CMoPointLightProgElem::VirtElem6()
{
    return CYyDb::g_pCYyDb->pMoProcessor->Vec3Magnitude(&this->field_188);
}

void FFXI::CYy::CMoPointLightProgElem::OnDraw()
{
    D3DXVECTOR3 v11 = this->field_54;
    float v8 = this->field_1A8 * this->field_19C;
    if (this->field_1B0 != 0) {
        float v6 = this->field_1A0;
        if (v6 < 0) {
            v6 = 0.0;
        }
        float v7 = this->field_1A4 * this->field_138 * v6;
        if (v7 < 0.0) {
            v7 = 0.0;
        }
        UpdateLight(this->field_1AC, &v11, &this->field_F8, v7, v8);
    }
    else {
        this->field_1AC = InitLight(this->field_194, &v11, &this->field_F8, 0.0, v8);
        this->field_1B0 = 1;
    }
}
