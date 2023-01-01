#pragma once

namespace FFXI {
	namespace CYy { class CYyMusicLoadTask;  }
	class MusicServer {
	public:
		static int last_request;
		static void clear_last_request();
		static void Play(int);
		static CYy::CYyMusicLoadTask* MusicLoadTask;
	};
}