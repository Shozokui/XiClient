#include "RegistryConfig.h"
#include "Enums.h"
#include <stdio.h>
#include <iostream>

using namespace FFXI::Config;
RegistryConfig* FFXI::Config::RegistryConfig::g_pOptions{ nullptr };

//int MainRegistryConfig::BackgroundXResolution = 512;
//int MainRegistryConfig::BackgroundYResolution = 512;
//int MainRegistryConfig::XResolution = 640;
//int MainRegistryConfig::YResolution = 480;
//int MainRegistryConfig::UIXResolution = 0;
//int MainRegistryConfig::UIYResolution = 0;

int MainRegistryConfig::MIPMapping = 0;
int MainRegistryConfig::dword104458F4 = 0;
int MainRegistryConfig::dword104458A0 = 50;
int MainRegistryConfig::EnableSound = 1;
int MainRegistryConfig::WindowOutOfFocusSound = 0;
float MainRegistryConfig::flt10445884 = 1.0;
float MainRegistryConfig::flt10445894 = 1.0;
float MainRegistryConfig::SomeGeneratorScalar = 1.0;
int MainRegistryConfig::EnvironmentalAnimationQuality = 1;
int MainRegistryConfig::FFXIProcessPriority = 0;
int MainRegistryConfig::dword10445898 = 1;
float MainRegistryConfig::flt104458E0 = 1.0;
float MainRegistryConfig::flt104458D0 = 1.0;
int MainRegistryConfig::dword104458A8 = 0;
int MainRegistryConfig::BumpMapping = 0;
int MainRegistryConfig::TextureQuality = 1;
int MainRegistryConfig::MapQuality = 1;
int MainRegistryConfig::FontQuality = 0;
int MainRegistryConfig::dword10445A28 = 1;
int MainRegistryConfig::HardwareMouse = 0;
int MainRegistryConfig::PlayOpeningMovie = 1;
int MainRegistryConfig::SimplifyCharCreateVisuals = 0;
int MainRegistryConfig::dword10445A14 = 0;
int MainRegistryConfig::dword10445900 = 0;
int MainRegistryConfig::dword104458EC = 0;
int MainRegistryConfig::dword10445A10 = 0;
float MainRegistryConfig::GammaBase = 0.0;
int MainRegistryConfig::MaxConcurrentSoundEffects = 12;
int MainRegistryConfig::dword104458E4 = 0;
int MainRegistryConfig::dword104458FC = 0x3BC49BA6;
int MainRegistryConfig::dword104458DC = 0;
int MainRegistryConfig::dword1044588C = 0;
int MainRegistryConfig::dword10445A1C = 1;
int MainRegistryConfig::GraphicsStabilization = 0;
int MainRegistryConfig::dword104458B4 = 0;
char MainRegistryConfig::ScreenshotPath[260]{ 0 };
int MainRegistryConfig::ScreenshotUseScreenResolution = 0;
DWORD CafeConfig::InCafe{ 0 };
struct RegTableEntry {
    int Type{ 0 };
    LPVOID Pointer{ nullptr };
    int TypeName{ 0 };
};

RegTableEntry RegTable[32] =
{
  { 0, &MainRegistryConfig::MIPMapping, 0 },
  { 0, &MainRegistryConfig::EnableSound, 7 },
  { 0, &MainRegistryConfig::WindowOutOfFocusSound, 35 },
  { 0, &MainRegistryConfig::EnvironmentalAnimationQuality, 11 },
  { 0, &MainRegistryConfig::BumpMapping, 17 },
  { 0, &MainRegistryConfig::TextureQuality, 18 },
  { 0, &MainRegistryConfig::MapQuality, 19 },
  { 0, &MainRegistryConfig::FontQuality, 36 },
  { 0, &MainRegistryConfig::dword10445A28, 20 },
  { 0, &MainRegistryConfig::HardwareMouse, 21 },
  { 0, &MainRegistryConfig::PlayOpeningMovie, 22 },
  { 0, &MainRegistryConfig::SimplifyCharCreateVisuals, 23 },
  { 0, &MainRegistryConfig::dword10445A14, 24 },
  { 0, &MainRegistryConfig::GammaBase, 28 },
  { 0, &MainRegistryConfig::MaxConcurrentSoundEffects, 29 },
  { 0, &MainRegistryConfig::dword104458E4, 30 },
  { 0, &MainRegistryConfig::dword104458FC, 31 },
  { 0, &MainRegistryConfig::dword104458DC, 32 },
  { 0, &MainRegistryConfig::dword1044588C, 33 },
  { 0, &MainRegistryConfig::dword10445A1C, 39 },
  { 0, &MainRegistryConfig::GraphicsStabilization, 40 },
  { 0, &MainRegistryConfig::dword104458B4, 41 },
  { 1, &MainRegistryConfig::ScreenshotPath, 42 },
  { 0, &MainRegistryConfig::ScreenshotUseScreenResolution, 43 },
  { -1, NULL, -1 }
};

char MainRegistryConfig::InitFromRegistry() {
    int Status; // eax
    int i; // ebp
    int v4; // eax
    bool v5; // al
    bool KeyDWORD; // al
    int* v7; // ecx
    char v10; // [esp+7h] [ebp-115h]
    CHAR ValueName[8]; // [esp+8h] [ebp-114h] BYREF
    DWORD cbData; // [esp+10h] [ebp-10Ch] BYREF
    BYTE Data[4]; // [esp+14h] [ebp-108h] BYREF
    BYTE v14[260]; // [esp+18h] [ebp-104h] BYREF

    Status = this->FFXIRegistry.Status;
    v10 = 1;
    if (Status > 0 && Status <= 2 && RegTable[0].Pointer)
    {
        for (i = 0; ; ++i)
        {
            RegTableEntry entry = RegTable[i];
            _snprintf_s(ValueName, 5u, "%04d", entry.TypeName);
            v4 = entry.Type;
            if (v4)
            {
                if (v4 == 1)
                {
                    cbData = 260;
                    if (!this->FFXIRegistry.QueryKeyString(v14, ValueName, &cbData))
                    {
                        v5 = this->FFXIRegistry.SetKeyString((LPCSTR)entry.Pointer, ValueName);
                        goto LABEL_13;
                    }
                    memcpy(entry.Pointer, v14, cbData);
                }
            }
            else
            {
                KeyDWORD = this->FFXIRegistry.QueryKeyDWORD(Data, ValueName);
                v7 = (int*)entry.Pointer;
                if (!KeyDWORD)
                {
                    v5 = this->FFXIRegistry.SetKeyDWORD(*v7, ValueName);
                LABEL_13:
                    if (!v5)
                        v10 = 0;
                    goto LABEL_15;
                }
                *v7 = *(int*)Data;
            }
        LABEL_15:
            if (!RegTable[i+1].Pointer)
                return v10;
        }
    }
    return 1;
}

FFXI::Config::MainRegistryConfig::~MainRegistryConfig()
{
}

void InitStaticVars() {
    MainRegistryConfig::MIPMapping = 0;
    MainRegistryConfig::dword104458F4 = 0;
    MainRegistryConfig::dword104458A0 = 50;
    MainRegistryConfig::EnableSound = 1;
    MainRegistryConfig::WindowOutOfFocusSound = 0;
    MainRegistryConfig::flt10445884 = 1.0;
    MainRegistryConfig::flt10445894 = 1.0;
    MainRegistryConfig::SomeGeneratorScalar = 1.0;
    MainRegistryConfig::EnvironmentalAnimationQuality = 1;
    MainRegistryConfig::FFXIProcessPriority = 0;
    MainRegistryConfig::dword10445898 = 1;
    MainRegistryConfig::flt104458E0 = 1.0;
    MainRegistryConfig::flt104458D0 = 1.0;
    MainRegistryConfig::dword104458A8 = 0;
    MainRegistryConfig::BumpMapping = 0;
    MainRegistryConfig::TextureQuality = 1;
    MainRegistryConfig::MapQuality = 1;
    MainRegistryConfig::FontQuality = 0;
    MainRegistryConfig::dword10445A28 = 1;
    MainRegistryConfig::HardwareMouse = 0;
    MainRegistryConfig::PlayOpeningMovie = 1;
    MainRegistryConfig::SimplifyCharCreateVisuals = 0;
    MainRegistryConfig::dword10445A14 = 0;
    MainRegistryConfig::dword10445900 = 0;
    MainRegistryConfig::dword104458EC = 0;
    MainRegistryConfig::dword10445A10 = 0;
    MainRegistryConfig::GammaBase = 0.0;
    MainRegistryConfig::MaxConcurrentSoundEffects = 12;
    MainRegistryConfig::dword104458E4 = 0;
    MainRegistryConfig::dword104458FC = 0x3BC49BA6;
    MainRegistryConfig::dword104458DC = 0;
    MainRegistryConfig::dword1044588C = 0;
    MainRegistryConfig::dword10445A1C = 1;
    MainRegistryConfig::GraphicsStabilization = 0;
    MainRegistryConfig::dword104458B4 = 0;
    MainRegistryConfig::ScreenshotPath[0] = 0;
    MainRegistryConfig::ScreenshotUseScreenResolution = 0;
}
FFXI::Config::MainRegistryConfig::MainRegistryConfig()
{
    this->Initialized = 0;
    InitStaticVars();
    this->Initialized = this->InitFromRegistry();
}

FFXI::Config::RegistryConfig::~RegistryConfig()
{

}

FFXI::Config::RegistryConfig::RegistryConfig()
{
    //TODO
    //Load pad mode
}

FFXI::Config::CafeConfig::CafeConfig()
{
    CafeConfig::InCafe = 0;
    BYTE Data[4];
    if (this->FFXIRegistry.QueryKeyDWORD(Data, "Cafe"))
        CafeConfig::InCafe = *(DWORD*)Data;
}
