#include "CTkMsbDataList.h"
#include "_49SubList.h"
#include "CTkMsbData.h"
#include "CTkNode.h"
#include <string.h>
#include "Globals.h"
#include "CTkMenuMng.h"
#include "CTkMenuData.h"
#include "CTkMenuFrame.h"
#include "TkManager.h"
#include "CTkResourceID.h"
using namespace FFXI::CTk;

FFXI::CTk::CTkMsbDataList::~CTkMsbDataList()
{
	this->field_1C = 0;
	this->field_20 = 0;
	this->field_24 = -1;
	this->field_28 = -1;
	this->field_2C = 0;
	this->field_30 = 0;
	this->field_34 = 0;
	this->field_38 = -1;
	this->field_3C = -1;
	this->field_40 = 0;

	CTkNode* HeadPosition = this->CTkObList::GetHeadPosition();
	CTkNode* a2 = HeadPosition;

	while (a2) {
		CTkNode* v5 = HeadPosition;
		CTkMsbData* Next = (CTkMsbData*)this->CTkObList::GetNext(&a2);
		this->CTkObList::RemoveAt(v5);
		if (Next)
			delete Next;
		HeadPosition = a2;
	}
	this->CTkObList::RemoveAll();
}

void FFXI::CTk::CTkMsbDataList::Init()
{
	this->field_1C = 0;
	this->field_20 = 0;
	this->field_24 = -1;
	this->field_28 = -1;
	this->field_2C = 0;
	this->field_30 = 0;
	this->field_34 = 0;
	this->field_38 = -1;
	this->field_3C = -1;
	this->field_40 = 0;
	this->CTkObList::RemoveAll();
}

_49SubList*** FFXI::CTk::CTkMsbDataList::GetSubLists(const char* resname)
{
	int v3;
	int v4;
	CTkMsbData* Next;
	CTkNode* a2;

	v3 = this->CTkObList::field_10;
	v4 = 0;
	a2 = this->CTkObList::GetHeadPosition();
	if (v3 <= 0)
		return 0;
	while (1)
	{
		Next = (CTkMsbData*)this->CTkObList::GetNext(&a2); 
		if (!_strnicmp(Next->ObjName, resname, sizeof(Next->ObjName)))
			break;
		if (++v4 >= v3)
			return 0;
	}
	return Next->GetSubLists();
}

void FFXI::CTk::CTkMsbDataList::Remove(void** vdata)
{
	char** data = (char**)(vdata);
	CTkMsbData* v4 = this->ContainsName(*data);
	while (v4 != nullptr) {
		if (this->CTkObList::RemoveAtIfContains(v4) == true) {
			CTkNode* head = TkManager::g_CTkMenuMng.ResourceMenuList.GetHeadPosition();
			while (head != nullptr) {
				CTkMenuData* next = (CTkMenuData*)TkManager::g_CTkMenuMng.ResourceMenuList.GetNext(&head);
				if (next != nullptr) {
					CTkMenuFrame* frame = next->GetFrame();
					if (frame != nullptr) {
						if (frame->Buffer != nullptr) {
							unsigned int count = frame->Buffer[20];
							for (unsigned int i = 0; i < count; ++i) {
								if (frame->ResourceIDs != nullptr) {
									if (strncmp(frame->ResourceIDs[i].field_4, *data, sizeof(frame->ResourceIDs[i].field_4)) == 0) {
										next->GetFrame()->ResourceIDs[i].field_18 = 0;
									}
								}
							}
						}
					}
				}
			}
			if (v4 != nullptr) {
				delete v4;
			}
		}
		v4 = this->ContainsName(*data);
	}
}

CTkMsbData* FFXI::CTk::CTkMsbDataList::ContainsName(char* name)
{
	if (this->CTkObList::field_10 <= 0) return nullptr;

	CTkMsbData* Next{ nullptr };
	CTkNode* a2 = this->CTkObList::GetHeadPosition();
	int v4 = 0;
	while (true) {
		Next = (CTkMsbData*)this->CTkObList::GetNext(&a2);
		if (!_strnicmp(name, Next->ObjName, sizeof(Next->ObjName)))
			break;
		if (++v4 >= this->CTkObList::field_10)
			return nullptr;
	}
	return Next;
	
}