#include "CTkObList.h"
#include "CTkObject.h"
#include "CTkNode.h"
using namespace FFXI::CTk;

CTkObList::CTkObList() {
	this->field_10 = 0;
	this->field_C = nullptr;
	this->field_8 = nullptr;
	this->Tail = nullptr;
	this->Head = nullptr;
	this->field_14 = 0;
	this->field_18 = 0;
}

CTkObList::~CTkObList() {
	CTkNode* Head = this->Head;
	if (Head) {
		CTkNode* v3{ nullptr };
		do {
			v3 = Head->field_0;
			delete Head;
			Head = v3;
		} while (v3);
	}

	Head = this->field_8;
	if (Head) {
		CTkNode* v3{ nullptr };
		do {
			v3 = Head->field_8;
			delete Head;
			Head = v3;
		} while (v3);
	}
}

CTkNode* FFXI::CTk::CTkObList::GetFreeNode()
{
	CTkNode* result{ nullptr };
	if (this->field_18 > 8) {
		result = this->field_C;
		this->field_C = result->field_C;
		result->field_C->field_8 = 0;
		this->field_18 -= 1;
	}
	else {
		result = new CTkNode();
		if (!result) return result;
	}
	this->field_14 += 1;
	result->field_14 = 0;
	result->field_14 = 0;
	result->field_8 = 0;
	result->field_C = 0;
	result->field_0 = 0;
	result->field_4 = 0;
	result->field_10 = 0;
	return result;
}

void FFXI::CTk::CTkObList::AddTail(CTkObject* a2)
{
	CTkNode* FreeNode = this->GetFreeNode();
	if (!this->Head)
		this->Head = FreeNode;
	if (this->Tail) {
		FreeNode->field_4 = this->Tail;
		this->Tail->field_0 = FreeNode;
	}
	this->Tail = FreeNode;
	FreeNode->field_10 = a2;
	this->field_10 += 1;
}

void FFXI::CTk::CTkObList::InsertAfter(CTkNode* a2, CTkObject* a3)
{
	if (!a2)
		this->AddTail(a3);
	else {
		CTkNode* FreeNode = this->GetFreeNode();
		CTkNode* v5 = a2->field_0;
		if (this->Tail == a2)
			this->Tail = FreeNode;
		FreeNode->field_4 = a2;
		FreeNode->field_0 = v5;
		a2->field_0 = FreeNode;
		if (v5)
			v5->field_4 = FreeNode;
		FreeNode->field_10 = a3;
		++this->field_10;
	}
}

void FFXI::CTk::CTkObList::InsertBefore(CTkNode* a2, CTkObject* a3)
{
	if (!a2) {
		this->AddTail(a3);
		return;
	}

	CTkNode* FreeNode = this->GetFreeNode();
	if (this->Head == a2)
		this->Head = FreeNode;
	FreeNode->field_4 = a2->field_4;
	FreeNode->field_0 = a2;
	a2->field_4 = FreeNode;
	if (a2->field_4)
		a2->field_4->field_0 = FreeNode;
	FreeNode->field_10 = a3;
	++this->field_10;

}

CTkNode* FFXI::CTk::CTkObList::GetHeadPosition()
{
	return this->Head;
}

CTkNode* FFXI::CTk::CTkObList::GetTailPosition()
{
	return this->Tail;
}

CTkObject* FFXI::CTk::CTkObList::GetNext(CTkNode** node)
{
	if (!*node) return nullptr;
	CTkNode* v2 = *node;
	while (v2->field_14) {
		v2 = v2->field_0;
		if (v2 == nullptr) return nullptr;
	}
	*node = v2->field_0;
	return v2->field_10;
}

CTkObject* FFXI::CTk::CTkObList::GetPrev(CTkNode** a2)
{
	if (!*a2)
		return nullptr;

	CTkNode* v2 = *a2;
	while (v2->field_14) {
		v2 = v2->field_4;
		if (!v2)
			return nullptr;
	}
	*a2 = v2->field_4;
	return v2->field_10;
}

void FFXI::CTk::CTkObList::RemoveAt(CTkNode* a2)
{
	if (!a2) return;

	if (this->Head == a2)
		this->Head = a2->field_0;
	if (this->Tail == a2)
		this->Tail = a2->field_4;
	a2->field_10 = nullptr;
	if (a2->field_0)
		a2->field_0->field_4 = a2->field_4;
	if (a2->field_4)
		a2->field_4->field_0 = a2->field_0;
	this->field_10 -= 1;
	a2->field_14 = 1;
	this->field_18 += 1;
	this->field_14 -= 1;
	if (this->field_8)
		this->field_8->field_C = a2;
	a2->field_C = nullptr;
	a2->field_8 = this->field_8;
	this->field_8 = a2;
	if (!this->field_C)
		this->field_C = a2;
	a2->field_10 = nullptr;
}

bool FFXI::CTk::CTkObList::RemoveAtIfContains(CTkObject* a2)
{
	CTkNode* Head = this->Head;
	CTkNode* v2 = this->Head;
	CTkObject* Next = this->GetNext(&v2);
	if (!Next)
		return false;

	while (Next != a2) {
		Head = v2;
		Next = this->GetNext(&v2);
		if (!Next)
			return false;
	}
	this->RemoveAt(Head);
	return true;
}

void FFXI::CTk::CTkObList::RemoveAll()
{
	CTkNode* Node = this->Head;
	CTkNode* NodeBackup{ nullptr };
	while (Node) {
		NodeBackup = Node->field_0;
		delete Node;
		Head = NodeBackup;
	}

	Node = this->field_8;
	while (Node) {
		NodeBackup = Node->field_8;
		delete Node;
		Node = NodeBackup;
	}

	this->field_10 = 0;
	this->field_C = 0;
	this->field_8 = 0;
	this->Tail = 0;
	this->Head = 0;
	this->field_14 = 0;
	this->field_18 = 0;
}
