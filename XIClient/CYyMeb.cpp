#include "CYyMeb.h"
#include "CTkMenuData.h"
#include "TkManager.h"
#include "CTkMenuMng.h"

using namespace FFXI::CYy;

void FFXI::CYy::CYyMeb::Open()
{
	void* Data = this + 1;
	
	CTk::CTkMenuData* node = new CTk::CTkMenuData();
	if (!node) return;

	node->Store(&Data);
	CTk::TkManager::g_CTkMenuMng.ResourceMenuList.AddTail(node);
}

void FFXI::CYy::CYyMeb::Close()
{
	void* Data = this + 1;

	CTk::CTkMenuMng* MenuMng = &CTk::TkManager::g_CTkMenuMng;
	CTk::CTkMenuData* node = MenuMng->GetResourceMenu((char*)Data);
	if (node) {
		if (MenuMng->ResourceMenuList.RemoveAtIfContains(node))
			delete node;
	}
}
