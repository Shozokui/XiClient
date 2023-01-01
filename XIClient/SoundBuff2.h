#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <mmeapi.h>
#include <dsound.h>
#include "WeirdSoundThing.h"
#include "AudioFileHeader.h"
namespace FFXI {
	class SoundBuff1;
	class SoundBuff2 {
	public:
		LPDIRECTSOUNDBUFFER PrimaryBuffer;
		LPDIRECTSOUNDBUFFER SecondaryBuffer;
		AudioFileHeader FileHeader;
		HANDLE Handle;
		DWORD field_3C;
		SoundBuff2* field_40;
		HANDLE HandleCpy;
		int field_48;
		int field_4C;
		int field_50;
		int field_54;
		WeirdSoundThing field_58;
		DWORD* field_68;
		char field_6C;
		char field_6D;
		char field_6E;
		char field_6F;
		char field_70;
		char field_71;
		char field_72;
		char field_73;
		char field_74;
		char field_75;
		char field_76;
		char field_77;
		char field_78;
		char field_79;
		char field_7A;
		char field_7B;
		int field_7C;
		int field_80;
		int field_84;
		int field_88;
		int field_8C;
		char field_90;
		char field_91;
		char field_92;
		char field_93;
		int field_94;
		int field_98;
		int Type0DecodeContext[4];
		int field_AC;
		int field_B0;
		int field_B4;
		unsigned char field_B8;
		char field_B9;
		char field_BA;
		char field_BB;
		char FileName[0x104];
	};
}