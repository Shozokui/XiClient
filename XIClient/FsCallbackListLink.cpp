#include "FsCallbackListLink.h"
#include "CTkMenuPrimitive.h"

using namespace FFXI::Config;

FFXI::Config::FsCallbackListLink::FsCallbackListLink(void(__cdecl* a2)(int, int, CTk::CTkMenuPrimitive*), CTk::CTkMenuPrimitive* a3)
{
	this->next = nullptr;
	this->callback = a2;
	this->menu = a3;
}

FFXI::Config::FsCallbackListLink::~FsCallbackListLink()
{
	if (this->next != nullptr) {
		delete this->next;
		this->next = nullptr;
	}
}

void FFXI::Config::FsCallbackListLink::addToEnd(void(__cdecl* a2)(int, int, CTk::CTkMenuPrimitive*), CTk::CTkMenuPrimitive* a3)
{
	if (this->callback == a2 && this->menu == a3)
		return;

	FsCallbackListLink* entry = this;
	while (entry->next) {
		entry = entry->next;
		if (entry->callback == a2 && entry->menu == a3)
			return;
	}

	entry->next = new FsCallbackListLink(a2, a3);
}

void FFXI::Config::FsCallbackListLink::runAllCallbacks(int a2, int a3)
{
	FsCallbackListLink* entry = this;
	while (entry) {
		entry->callback(a2, a3, entry->menu);
		entry = entry->next;
	}
}
