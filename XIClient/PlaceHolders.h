#pragma once

namespace Placeholder {
	static void StartAutoOffline() {}
	static void PauseAutoOffline() {}

	static inline char gcZoneSubMapChangeSet(int a1, int a2) {
		return 0;
	}
	const static void* pGlobalNowZone{ nullptr };
	const static void* pZoneSys{ nullptr };
	const static void* g_pTkInputCtrl{ nullptr };
	const static bool GetControlActor{ false };
	const static bool enable_change_focus{ false };
	const static bool is_first_person_view{ false };
	const static void* g_pStDancer{ nullptr };
	static inline bool CheckSomething() {
		if (pGlobalNowZone || pZoneSys)
			throw "NOT IMPLEMENTED";

		return false;
	}
	static inline bool IsEvent(int) {
		return false;
	}
}