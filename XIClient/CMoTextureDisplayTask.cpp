#include "CMoTextureDisplayTask.h"
#include "CYyDb.h"
#include "CMoDx.h"
using namespace FFXI::CYy;

CMoTextureDisplayTask* CMoTextureDisplayTask::CurrentTextureDisplayTask{ nullptr };
float CMoTextureDisplayTask::progress{ 0.0 };

FFXI::CYy::CMoTextureDisplayTask::CMoTextureDisplayTask(CMoTask* a2)
{
    this->DoIt(a2);
}

FFXI::CYy::CMoTextureDisplayTask::~CMoTextureDisplayTask()
{
    for (int i = 0; i < sizeof(CYyDb::TextureDisplayTaskArr) / sizeof(CYyDb::TextureDisplayTaskArr[0]); ++i) {
        IDirect3DTexture8** tex = CYyDb::TextureDisplayTaskArr + i;
        if (*tex != nullptr) {
            (*tex)->Release();
            *tex = nullptr;
        }
    }

    CMoTextureDisplayTask::progress = 0.0;
    CYyDb::TextureDisplayState = 0;

    if (CMoTextureDisplayTask::CurrentTextureDisplayTask == this) {
        CMoTextureDisplayTask::CurrentTextureDisplayTask = nullptr;
    }
}

char FFXI::CYy::CMoTextureDisplayTask::OnMove()
{
    this->TimeRemaining -= CYyDb::g_pCMoDx->field_EB0;
    if (this->TimeRemaining <= 0.0) {
        delete this;
        return 1;
    }
    else {
        CMoTextureDisplayTask::progress = this->TimeRemaining / this->Duration;
        return 0;
    }
    return 0;
}
