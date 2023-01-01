#include "XiAtelBuff.h"
#include "CXiControlActor.h"
#include "Strings.h"
#include <string>
#include "CXiSkeletonActor.h"
#include "Values.h"
#include "Globals.h"
using namespace FFXI::CYy;

XiAtelBuff* XiAtelBuff::AtelBuffers[0x900];

FFXI::CYy::XiAtelBuff::XiAtelBuff()
{
	this->Reset(0);
}

void FFXI::CYy::XiAtelBuff::Reset(int a2)
{
    this->field_DC = 100;
    this->field_E0 = 100;
    this->field_120 &= 0xFFFFE07F;
    this->field_11C = 0;
    this->field_4 = { 0.0, 0.0, 0.0, 1.0 };
    this->field_14 = { 0.0, 0.0, 0.0, 1.0 };
    this->field_24 = { 0.0, 0.0, 0.0, 1.0 };
    this->GlobalBufferIndex = 0;
    this->field_78 = 0;
    this->Name[0] = 0;
    this->Actor = nullptr;
    memset(this->ActorPointers, 0, sizeof(this->ActorPointers));
    this->field_E8 = 0;
    this->field_120 &= 0xFFFFFFCF;
    this->ActorType = 0;
    this->RaceGender = 0;
    this->field_162 = 0;
    this->field_164 = 880111469;
    memset(&this->Equipment, 0, sizeof(this->Equipment));
    this->field_F4 = 0;
    this->field_F6 = 0;
    this->field_D8 = 0;
    this->GameStatus = 0;
    this->field_168 = 0;
    this->field_174 = 0;
    this->field_178 = 0;
    this->field_158 = 5.0;;
    this->field_98 = 5.0;
    this->field_9C = 5.0;
    this->field_F0 = 0;
    this->field_1B4 = 0;
    this->field_184 = 0;
    this->field_180 = 0;
    this->field_17C = 0;
    this->field_160 = 0;
    this->field_15C = 0;
    this->field_E4 = 0;
    this->field_1B8 = 0;
    this->field_1B9 = 0;
    this->field_1D0 = 0;
    this->field_1D4 = 23;
    this->field_1CC = 0;
    this->field_144 = 0;
    this->field_14A = 0;
    this->field_14C = 0;
    this->field_120 &= 0x1FB0;
    this->field_140 = 0;
    this->field_124 = 0x880;
    this->field_142 = 0;
    this->field_146 = 0;
    this->field_148 = 0;
    this->field_130 &= 0xFFFF7F00;
    this->field_12C &= 0x300002;
    this->field_138 &= 0x87FFFFFF;
    this->field_13C &= 0xFEC00800;
    this->field_13C |= 8;
    this->field_1EC = 0;
    this->field_1FB = 0;
    this->field_1FC = 0;
    this->field_1FD = 0;
    this->field_1D6 = 0;
    this->field_128 = 131073;
    this->field_141 = 0;
    this->field_1F4 = 0;
    this->field_2A2 = 0;
    this->field_1F6 = 0;
    this->field_2A0 = 0;
    this->field_1FA = 0;
    this->field_1F8 = 0;
    this->field_200 = -1.0;
    this->field_204 = 0;
    this->field_208 = 0;
    this->field_20C = 0;
    memset(&this->MonsterNumber, 0, 0x2Cu);
    this->field_13C &= 0xFF3FFFFF;
    this->field_188 = 0;
    this->field_130 &= 0xFFFC80FF;
    memset(&this->field_278, 0, 0x24u);
    this->field_134 &= 0xFF0FFC02;
    this->field_130 &= 0x103FFFF;
    this->field_29C = 0;
    this->field_134 &= 0x00F003FD;
    this->field_1FE = 0;
    this->field_138 &= 0xF8000000;
    memcpy(this->field_18C, FFXI::Constants::Strings::DefaultAnimations, sizeof(FFXI::Constants::Strings::DefaultAnimations));
    if (a2 == 0x1000000)
    {
        this->field_78 = 0x1000000;
        this->GlobalBufferIndex = 2304;
        this->field_1CC |= 2;
        this->field_120 |= 0x200;
    }
    else
    {
        this->MotionSetChg();
        this->IdleDefMotionReset();
    }
    this->field_D4 = 0;
    this->field_F8 = 0;
    this->field_1BC = -1;
    this->field_1C0 = 0;
    this->field_1C4 = 0;
    this->field_1C8 = 0;
    this->field_1E8 = 0;
    this->field_1F0 = 0;
    this->field_1D8 = 0;
    this->field_1DC = 0;
    this->field_1E0 = 0;
    this->field_1E4 = 0;
    this->field_2A4 = 0;
    memset(&this->field_210, 0, 0x20u);

    //something not quite right here
    this->field_230 = 0;
    memset((char*)&this->field_230 + 1, 0, 0x18u);
    this->field_249 = 0;
}

void FFXI::CYy::XiAtelBuff::MotionSetChg()
{
    if (this->Actor) {
        if (this->Actor->InOwnActorPointers() && this->CheckStatus()) {
            this->field_18C[0] = ' ihc';
        }
    }

    if (this->ActorType == 8)
        this->field_18C[0] = ' ihc';

    if (this->GetSomeFlag() == 2) {
        memcpy(this->field_1A0, FFXI::Constants::Strings::SomeAnimations, sizeof(FFXI::Constants::Strings::SomeAnimations));
    }
    else {
        int v4{};
        if (this->TestSomething(255)) {
            if (this->Test74Over700()
                || (this->field_120 & 0x40000000) != 0
                || (this->field_12C & 0x80000000) != 0) {
                v4 = this->field_18C[1];
            }
            else
                v4 = this->field_18C[4];
        }
        else if (this->GameStatus == 47)
            v4 = this->field_18C[1];
        else {
            v4 = this->field_18C[0];
        }

        this->field_1A0[0] = v4;
        this->field_1A0[1] = v4;
        this->field_1A0[2] = v4;
        this->field_1A0[3] = this->field_18C[2];
        this->field_1A0[4] = this->field_18C[3];

        if (this->ActorType == 2 || this->ActorType == 7) {
            unsigned short v8 = this->Equipment[0];
            if (v8 >= 0x3C0u && v8 <= 0x3C9u
                || v8 >= 0x979u && v8 <= 0x97Du
                || v8 >= 0x9BFu && v8 <= 0x9C3u) {
                this->field_1A0[2] = this->field_18C[0];
            }
        }
    }

    if ((this->field_120 & 0x200) != 0) {
        if (this->Actor->IsKindOf(&CXiSkeletonActor::CXiSkeletonActorClass)) {
            CXiSkeletonActor* skele = (CXiSkeletonActor*)this->Actor;
            skele->field_7C8[0] = this->field_1A0[0];
            skele->field_7C8[1] = this->field_1A0[3];
            skele->field_7C8[2] = this->field_1A0[4];
        }
    }
}

void FFXI::CYy::XiAtelBuff::IdleDefMotionReset()
{
    if ((this->field_120 & 0x2000) != 0)
        this->field_1B6 = 0;
    else
        this->field_1B6 = rand() % 600 + 600;

    this->field_1A0[0] = this->field_1A0[1];

    if ((this->field_120 & 0x200) != 0) {
        if (this->Actor->IsKindOf(&CXiSkeletonActor::CXiSkeletonActorClass)) {
            CXiSkeletonActor* skele = (CXiSkeletonActor*)this->Actor;
            if (skele->field_7C8[0] != this->field_1A0[0])
                skele->field_7C8[0] = this->field_1A0[0];
       }
    }
}

unsigned char FFXI::CYy::XiAtelBuff::GetSomeFlag()
{
    if ((this->field_12C & 0x80000) != 0)
        return (this->field_12C >> 20) & 3;
    
    return (this->field_124 >> 13) & 3;
}

bool FFXI::CYy::XiAtelBuff::TestSomething(int a2)
{
    int v2 = a2;
    if (v2 == 255)
        v2 = this->GameStatus;

    if (v2 >= 63 && v2 <= 83)
        return true;

    return false;
}

bool FFXI::CYy::XiAtelBuff::Test74Over700()
{
    return this->GlobalBufferIndex >= 0x700;
}

void FFXI::CYy::XiAtelBuff::SetName(char* a2)
{
    if (this->field_20C != 0) return;

    memcpy(this->Name, a2, sizeof(this->Name) - 1);
    this->Name[sizeof(this->Name)-1] = 0;
}

bool FFXI::CYy::XiAtelBuff::CheckStatus()
{
    if (this->CheckGameStatus85(255) == true) return false;
    char v3 = (this->field_120 >> 24) & 7;
    return v3 == 2 || v3 == 3;
}

bool FFXI::CYy::XiAtelBuff::CheckGameStatus5(int a2)
{
    if (a2 == 255)
        a2 = this->GameStatus;

    return a2 == 5;
}

bool FFXI::CYy::XiAtelBuff::CheckGameStatus85(int a2)
{
    if (a2 == 255)
        a2 = this->GameStatus;

    return a2 == 85;
}

void FFXI::CYy::XiAtelBuff::CopyAllSPos()
{
    this->field_4 = this->field_24;
    this->field_14 = this->field_34;
    this->CopyAllPos();
}

void FFXI::CYy::XiAtelBuff::CopyAllPos()
{
    if ((this->field_120 & 0x200) == 0) return;

    int actorType = this->ActorType;
    if (actorType == 0 || actorType == 1 || actorType == 2 
        || actorType == 6 || actorType == 7 || actorType == 8) {

        CXiSkeletonActor* skele = (CXiSkeletonActor*)this->Actor;
        skele->field_34 = this->field_4;
        skele->field_5FC = this->field_4;
        skele->field_5FC.w = 1.0;
        skele->field_5C4 = this->field_4;
        skele->SetPos(&this->field_4);
        skele->field_C4 = this->field_4;
        skele->field_44 = this->field_14;
        skele->field_E4 = this->field_14;

        Globals::PiClamp(&skele->field_44);

        if (skele->IsDirectionLock() == false) {
            skele->field_61C = this->field_14;
            Globals::PiClamp(&skele->field_61C);
        }
    }
}

CXiActor* FFXI::CYy::XiAtelBuff::GetSomeActor()
{
    if (this->field_29E == 0)
        return nullptr;

    return XiAtelBuff::ActorPointers[this->field_29E];
}
