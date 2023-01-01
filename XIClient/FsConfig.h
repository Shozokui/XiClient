#pragma once
#include "Enums.h"
namespace FFXI {
	namespace CTk { class CTkMenuPrimitive; };
	namespace Config {
		class FsConfigSubject;
		class FsConfig {
			static void RegisterTextColorFunctions();
			int getConfig(int);
		public:
			static void FsConfigInit();
			static void FsConfigUninit();
			static void RegisterConfigCallback(FFXI::Constants::Enums::FsConfigSubjects, void (__cdecl *)(int, int, CTk::CTkMenuPrimitive*), CTk::CTkMenuPrimitive*);
			static int GetConfig(FFXI::Constants::Enums::FsConfigSubjects);
			FsConfig();
			~FsConfig();
			void initConfig(Constants::Enums::FsConfigSubjects, int, int, int*, int, int, int, int);
			void initConfig();
			void loadTIGConfig();
			void setAllConfig(bool);
			void setConfig(FsConfigSubject*, int);
			void registerConfigCallback(int, void(__cdecl*)(int, int, CTk::CTkMenuPrimitive*), CTk::CTkMenuPrimitive*);
			
			FsConfigSubject* findConfigSubject(unsigned int);
			FsConfigSubject* subjects;
			char field_8;
			char field_9;
			char field_A;
			char field_B;
			char field_C;
			char field_D;
			char field_E;
			char field_F;
			char field_10;
			char field_11;
			char field_12;
			char field_13;
			char field_14;
			char field_15;
			char field_16;
			char field_17;
			char field_18;
			char field_19;
			char field_1A;
			char field_1B;
			char field_1C;
			char field_1D;
			char field_1E;
			char field_1F;
		};
	}
}