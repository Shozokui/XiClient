#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <mmeapi.h>
#include <dsound.h>
#include "Values.h"
namespace FFXI {
	namespace CYy { class CYySepRes; class CMoResource; }
	class SoundController;
	class SoundMng {
	public:
		class ThreadParam {
		public:
			HANDLE handle;
			DWORD field_4;
			DWORD shouldRun;
			DWORD field_C;
		};
		struct MusicTableEntry {
		public:
			int Index;
			char SubIndex;
		};

		struct MusicTable2Entry {
		public:
			int One;
			int Two;
			int Three;
		};
		static SoundMng* g_pSoundMng;
		static void ParseCLA();
		static bool InitSound(HWND);
		static void UninitSound();
		SoundMng();
		~SoundMng();
		static void SqsSeVolumeDirect(int, int, int);
		static void SqsSePanDirect(int, int, int);
		static void QueueNewSoundCommand(int, int, int, int, int, int, int, int);
		static void PlayMusicFromDisk(int, char, int);
		static int GetMusicTable2Index(int);
		static bool Init(HWND, char*);
		static void CleanThread(ThreadParam*);
		static void GetPanAndVolume(int*, int*);
		static void SetBufferVolume(LPDIRECTSOUNDBUFFER, int);
		static void SetBufferPan(LPDIRECTSOUNDBUFFER, int);
		static bool CanPlaySoundResource(CYy::CYySepRes**);
		static int SqsSeCheck(char*, float*, int);
		static void CYySePlayCursor();
		static void CYySePlayClick();
		static void CYySePlayBeep();
		static void CYySePlayCancel();
		static void CYySePlayWindowSelect();
		static CYy::CYySepRes** SepCursor;
		static CYy::CYySepRes** SepClick;
		static CYy::CYySepRes** SepBeep;
		static CYy::CYySepRes** SepCancel;
		static CYy::CYySepRes** SepWindowSelect;
		static const char* BGMPrefix;
		static LPTHREAD_START_ROUTINE SoundThreadProc;
		static IDirectSound8* DirectSound8;
		static bool SoundThreadRunning;
		static SoundController* SoundController;
		static const MusicTableEntry MusicTable[];
		static const MusicTable2Entry MusicTable2[];
		static const int SomeAudioTable[128];
		static const int SomeAudioTable2[64];
		static bool sound_command_send;
		static float SomeFloat1;
		static float SomeFloat2;
		bool StartThread(HWND);
		void Clean();
		int SqsSePlay(char*, int, int);
		int UpdateCounter();
		int field_0;
		int field_4;
		int field_8;
		ThreadParam* Param;
	};
}