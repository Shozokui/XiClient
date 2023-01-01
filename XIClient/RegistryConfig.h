#pragma once
#include "UnknownStructSub1.h"
#include "FFXIRegistry.h"
#include "Enums.h"
namespace FFXI {
	namespace Config {
		struct MainRegistryConfig {
			char InitFromRegistry();
		public:
			virtual ~MainRegistryConfig();
			MainRegistryConfig();
			int field_4;
			int field_8;
			FFXIRegistry FFXIRegistry;
			int Initialized;
			static int MIPMapping;
			static int dword104458F4;
			static int dword104458A0;
			static int EnableSound;
			static int WindowOutOfFocusSound;
			static float flt10445884;
			static float flt10445894;
			static float SomeGeneratorScalar;
			static int EnvironmentalAnimationQuality;
			static int FFXIProcessPriority;
			static int dword10445898;
			static float flt104458E0;
			static float flt104458D0;
			static int dword104458A8;
			static int BumpMapping;
			static int TextureQuality;
			static int MapQuality;
			static int FontQuality;
			static int dword10445A28;
			static int HardwareMouse;
			static int PlayOpeningMovie;
			static int SimplifyCharCreateVisuals;
			static int dword10445A14;
			static int dword10445900;
			static int dword104458EC;
			static int dword10445A10;
			static float GammaBase;
			static int MaxConcurrentSoundEffects;
			static int dword104458E4;
			static int dword104458FC;
			static int dword104458DC;
			static int dword1044588C;
			static int dword10445A1C;
			static int GraphicsStabilization;
			static int dword104458B4;
			static char ScreenshotPath[260];
			static int ScreenshotUseScreenResolution;
		};
		struct CafeConfig {
		public:
			CafeConfig();
			FFXIRegistry FFXIRegistry;
			static DWORD InCafe;
		};
		struct RegistryConfig {
		public:
			static RegistryConfig* g_pOptions;
			virtual ~RegistryConfig();
			RegistryConfig();
			UnknownStructSub1 Other{};
			MainRegistryConfig Main{};
			CafeConfig Cafe{};
		};
	}
}