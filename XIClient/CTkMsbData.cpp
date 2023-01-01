#include "CTkMsbData.h"
#include "_49ListNode.h"
#include "_49SubList.h"
using namespace FFXI::CTk;

FFXI::CTk::CTkMsbData::~CTkMsbData()
{
	CTkNode* HeadPosition = this->ObList.GetHeadPosition();
	CTkNode* a2 = HeadPosition;

	while (a2) {
		CTkNode* v4 = HeadPosition;
		_49ListNode* Next = (_49ListNode*)this->ObList.GetNext(&a2);
		this->ObList.RemoveAt(v4);
		if (Next)
			delete Next;
		HeadPosition = a2;
	}
	this->ObList.RemoveAll();

	int v6 = 0;
	while (v6 < this->field_3C) {
		_49SubList* Next = this->SubLists[v6];
		if (Next) {
			delete Next;
			this->SubLists[v6] = nullptr;
		}
		++v6;
	}

	if (this->SubLists) {
		delete[] this->SubLists;
		this->SubLists = nullptr;
	}
}

_49SubList*** FFXI::CTk::CTkMsbData::GetSubLists()
{
	return &this->SubLists;
}

void FFXI::CTk::CTkMsbData::DoSomething()
{
	int v2 = 0;
	_49SubList* v3{ nullptr };
	while (v2 < this->field_3C) {
		v3 = this->SubLists[v2];
		if (v3)
			v3->DoSomething2();
		++v2;
	}
}
