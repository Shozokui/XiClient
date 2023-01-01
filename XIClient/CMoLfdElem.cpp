#include "CMoLfdElem.h"
#include "CYyDb.h"
#include "CYyCamMng2.h"
#include "BaseProcessor.h"
#include "CMoDx.h"
#include "CDx.h"
#include "Globals.h"
#include "SomeCMoObj.h"
using namespace FFXI::CYy;

const CYyClass FFXI::CYy::CMoLfdElem::CMoLfdElemClass{
    "CMoLfdElem", sizeof(CMoLfdElem), &CMoElem::CMoElemClass
};

const CYyClass* FFXI::CYy::CMoLfdElem::GetRuntimeClass()
{
    return &CMoLfdElem::CMoLfdElemClass;
}

FFXI::CYy::CMoLfdElem::CMoLfdElem()
{
    this->field_176 = sizeof(CMoLfdElem);
}

void FFXI::CYy::CMoLfdElem::VirtOt1()
{
    this->field_F8.GetWithScaledAlpha(&CMoElem::SomeColor, this->field_138 * this->field_134);
    CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;
    BaseProcessor* proc = CYyDb::g_pCYyDb->pMoProcessor;
    CMoDx* cmodx = CYyDb::g_pCYyDb->g_pCMoDx;

    FFXI::Math::WMatrix* somemtx = this->GetSomeMatrix();
    D3DXVECTOR3 v38{};
    if (somemtx != nullptr && cammng != nullptr) {
        v38.x = somemtx->m[3][0];
        v38.y = somemtx->m[3][1];
        v38.z = somemtx->m[3][2];
        v38 -= cammng->Position;
        proc->Vec3Normalize(&v38);
        float scale = CYyDb::g_pCYyDb->field_2E0;
        v38.x *= scale;
        v38.y *= scale;
        v38.z *= scale;
    }
    else {
        v38 = this->field_54;
    }

    D3DXVECTOR3 v39{};
    proc->VirtProcessor27(&v38, &v39, 1, &cmodx->g_pSomeCMoObj->field_10, &cmodx->field_B10, cmodx->field_DC0, cmodx->field_DC4);
    if (v39.z < 0.0) {
        return;
    }

    D3DVIEWPORT8* vp = cmodx->field_DC8->GetTopViewport();
    float width = vp->Width / 32.0;
    float height = vp->Height / 32.0;
    D3DXVECTOR4 v40{};
    v40.x = v39.x - width;
    v40.y = v39.y - height;
    v40.z = v39.x + width;
    v40.w = v39.y + height;
    int v6 = this->field_17F.Update(&v40, v39.z,
        (float)CYyDb::GetBackgroundXRes(),
        (float)CYyDb::GetBackgroundYRes(),
        Globals::g_pSomeTexture1,
        Globals::g_pSomeTexture2,
        1, 0, 0);

    if (this->res == nullptr) {
        return;
    }

    if (v6 <= 0) {
        return;
    }

    FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_TEXTUREFACTOR, CMoElem::SomeColor);
    vp = cmodx->field_DC8->GetTopViewport();
    //sub  //TODO
    vp = cmodx->field_DC8->GetTopViewport();

    float v33 = (double)((int)vp->Width / 2) - v39.x;
    float v37 = (double)((int)vp->Height / 2) - v39.y;
    float v1 = v33 / (double)((int)vp->Width / 2);
    float v2 = v37 / (double)((int)vp->Height / 2);
    long double v14 = 1.0 - sqrt(v2 * v2 + v1 * v1) / 1.5;
    if (v14 < 0.0) {
        v14 = 0.0;
    }
    
    int v35 = this->field_19C;
    if (v35 == 0) {
        v35 = 0x80808080;
    }

    long double v15 = v14 * 0.5 * (double)(v6 >> 3) / 128.0;

    long double v16 = (double)((v35 >> 16) & 0xFF) * v15;
    v35 &= 0xFF00FFFF;
    if (v16 >= 0.0) {
        v35 |= ((int)v16 & 0xFF) << 16;
    }

    v16 = (double)((v35 >> 8) & 0xFF) * v15;
    v35 &= 0xFFFF00FF;
    if (v16 >= 0.0) {
        v35 |= ((int)v16 & 0xFF) << 8;
    }

    v16 = (double)((v35 >> 0) & 0xFF) * v15;
    v35 &= 0xFFFFFF00;
    if (v16 >= 0.0) {
        v35 |= ((int)v16 & 0xFF) << 0;
    }

    v16 = (double)((v35 >> 24) & 0xFF) * v15;
    v35 &= 0x00FFFFFF;
    if (v16 >= 0.0) {
        v35 |= ((int)v16 & 0xFF) << 24;
    }

    CYyDb::g_pCYyDb->Set324(v35);
}

bool FFXI::CYy::CMoLfdElem::VirtElem1(FFXI::Constants::Enums::ElemType a2)
{
    return a2 == FFXI::Constants::Enums::ElemType::Lfd;
}
