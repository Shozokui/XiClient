#pragma once
namespace FFXI {
	namespace CTk { class CTkMenuPrimitive; }
	namespace Config {
		class FsCallbackListLink {
		public:
			FsCallbackListLink(void(__cdecl*)(int, int, CTk::CTkMenuPrimitive*), CTk::CTkMenuPrimitive*);
			~FsCallbackListLink();
			void addToEnd(void(__cdecl*)(int, int, CTk::CTkMenuPrimitive*), CTk::CTkMenuPrimitive*);
			void runAllCallbacks(int, int);
			FsCallbackListLink* next;
			void(__cdecl* callback)(int, int, CTk::CTkMenuPrimitive*);
			CTk::CTkMenuPrimitive* menu;
		};
	}
}