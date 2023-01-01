#pragma once

const static void* pGlobalNowZone{ nullptr };
const static void* pZoneSys{ nullptr };

static inline bool CheckSomething() {
	if (pGlobalNowZone || pZoneSys)
		throw "NOT IMPLEMENTED";

	return false;
}