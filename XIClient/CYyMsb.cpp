#include "CYyMsb.h"
#include "CTkMsbData.h"
#include "_49SubList.h"
#include "_49ListNode.h"
#include "Globals.h"
#include "CTkMsbDataList.h"
#include "TkManager.h"
#include <iostream>
using namespace FFXI::CYy;
using namespace FFXI::CTk;

void InitList(CTkMsbData* list, void* Data) {
	unsigned int Flag{ 0 };
	Globals::CopyAdvance(list->ObjName, &Data, sizeof(list->ObjName));
	Globals::CopyAdvance(&Flag, &Data, 1);
	unsigned int counter = 0;

	while (counter < Flag) {
		_49ListNode* newNode = new _49ListNode();
		if (!newNode) return;
		Globals::CopyAdvance(newNode->ResName, &Data, sizeof(newNode->ResName));
		list->ObList.AddTail(newNode);
		++counter;
	}

	Globals::CopyAdvance(&list->field_3C, &Data, sizeof(list->field_3C));
	list->SubLists = new _49SubList*[list->field_3C + 1];
	if (list->SubLists) {
		counter = 0;
		while (counter < list->field_3C) {
			_49SubList* newSubList = new _49SubList();
			list->SubLists[counter] = newSubList;
			if (!newSubList) return;
			if (!list->SubLists[counter]->DoSomething(&Data)) {
				if (list->SubLists[counter]) {
					delete list->SubLists[counter];
					list->SubLists[counter] = nullptr;
				}
			}
			++counter;
		}
		list->SubLists[counter] = nullptr;
	}
}
//~LOCAL FUNCS
void CYyMsb::Open()
{
	void* DataStart = this + 1;
	CTkMsbData* newList = new CTkMsbData();
	InitList(newList, DataStart);
	TkManager::g_CTkMenuDataList.AddTail(newList);
}

void CYyMsb::Close()
{
	void* DataStart = this + 1;
	TkManager::g_CTkMenuDataList.Remove(&DataStart);
}
