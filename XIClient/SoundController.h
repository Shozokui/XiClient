#pragma once
#include <wtypes.h>

namespace FFXI {
	class SoundBuff1;
	class SoundBuff2;
	class SoundController {
	public:
		struct CachedSoundEffect {
			char* Data;
			int ID;
			int LoadOrder;
		};
		SoundController();
		~SoundController();
		static void SomeClean(int, int);
		static int GetBuff1Flag(int, int, int);
		static int GetBuff2Flag(int, int);
		static void UpdateBin1();
		static void UpdateBin2();
		static void UpdateQueue();
		static CachedSoundEffect* GetCachedSoundEffect(int);
		static CachedSoundEffect* CreateCachedSoundEffect(int);
		static void UpdateCacheLoadOrders();
		static void NotSure(int, int);
		static void NotSure2(int);
		static void PlaySeWav(char*, int, int, int);
		static SoundBuff1* GetSB1(int, int, int);
		const static int CommandQueueSize;
		const static int SoundBuff1BinSize;
		const static int SoundBuff2BinSize;
		const static int AudioPosBackupSize;
		const static int SoundEffectCacheSize;

		static unsigned int* CommandQueue;
		static unsigned int* CommandQueueWriteCursor;
		static unsigned int* CommandQueueReadCursor;

		static char* MusicDirs;
		static char* SoundEffectDirs;

		static int SoundOOFFlag;
		static int SomeValue;

		//Controls start point of the audio data.
		//It is a fraction with valid range 0-1.
		//0 Represents the beginning of the audio data and 1 represent the end (0% / 100%)
		static float AudioStartPos;
		static SoundBuff1* SoundBuff1Bin;
		static SoundBuff2* SoundBuff2Bin;
		static int* AudioSpotBackups;
		static CachedSoundEffect* SoundEffectCache;
		static CRITICAL_SECTION cs;
		int field_0;
		int field_4;
		int field_8;
	};
}