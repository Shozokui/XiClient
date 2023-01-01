#include "_49SubList.h"
#include "_49SubListNode.h"
#include "CTkNode.h"
#include "Globals.h"
#include <string>
using namespace FFXI::CTk;

_49SubList::~_49SubList() {
	CTkNode* Head = this->ObList.GetHeadPosition();
	CTkNode* a2 = Head;
	while (a2) {
		_49SubListNode* Next = (_49SubListNode*)this->ObList.GetNext(&a2);
		this->ObList.RemoveAt(Head);
		if (Next)
			delete Next;
		
		Head = a2;
	}
}

bool FFXI::CTk::_49SubList::DoSomething(void** data)
{
	unsigned char EntryCount{ 0 };
	Globals::CopyAdvance(&EntryCount, data, 1);

	int v4 = 0;
	while (v4 < EntryCount) {
		_49SubListNode* newSubListNode = new _49SubListNode();
		if (!newSubListNode) return false;
		newSubListNode->Fill(data);
		this->ObList.AddTail(newSubListNode);
		++v4;
	}
	
	CTkNode* Head = this->ObList.GetHeadPosition();
	if (!Head) return false;
	_49SubListNode* v9 = (_49SubListNode*)Head->GetTkObject();
	this->field_20 = v9->VertX[0];
	this->field_22 = v9->VertX[0];
	this->field_24 = v9->VertY[0];
	this->field_26 = v9->VertY[0];

	int v11 = 0;
	while (v11 < EntryCount) {
		_49SubListNode* Next = (_49SubListNode*)this->ObList.GetNext(&Head);
		if (!Next) break;

		for (int i = 0; i < 4; ++i) {
			if (Next->VertX[i] > this->field_22)
				this->field_22 = Next->VertX[i];
			if (Next->VertX[i] < this->field_20)
				this->field_20 = Next->VertX[i];
			if (Next->VertY[i] > this->field_26)
				this->field_26 = Next->VertY[i];
			if (Next->VertY[i] < this->field_24)
				this->field_24 = Next->VertY[i];
		}
		++v11;
	}

	return true;
}

void FFXI::CTk::_49SubList::DoSomething2()
{
	int v1 = this->ObList.field_10;
	CTkNode* a2 = this->ObList.GetHeadPosition();
	_49SubListNode* Next { nullptr };
	while (v1) {
		Next = (_49SubListNode*)this->ObList.GetNext(&a2);
		if (Next)
			Next->TexInit();
		--v1;
	}
}

void FFXI::CTk::_49SubList::Draw(int a2, int a3, D3DCOLOR a4, int a5, int a6)
{
	CTkNode* Head = this->ObList.GetHeadPosition();
	if (a6 > 0) {
		int v9 = a6;
		while (v9) {
			this->ObList.GetNext(&Head);
			--v9;
		};
	}

	int DrawCount = this->ObList.field_10 - a6;
	while (DrawCount) {
		_49SubListNode* Next = (_49SubListNode*)this->ObList.GetNext(&Head);
		if (Next)
			Next->Draw(a2, a3, 1.0, 1.0, a4, a5);
		--DrawCount;
	}
}

void FFXI::CTk::_49SubList::DrawRotate(int a2, int a3, FFXI::Math::WMatrix* a4, D3DCOLOR a5, int a6)
{
	CTkNode* head = this->ObList.GetHeadPosition();

	int v12 = 0;
	while (v12 < this->ObList.field_10) {
		_49SubListNode* next = (_49SubListNode*)this->ObList.GetNext(&head);
		if (next)
			next->DrawRotate(a2, a3, a4, a5, a6);
		++v12;
	}
}

void FFXI::CTk::_49SubList::ExtraDraw(int a2, int a3, float a4, float a5, unsigned int a6, char a7, unsigned int a8, int a9)
{
	if (a4 == 1.0 && a5 == 1.0) {
		this->Draw(a2, a3, a6, a8, a9);
		return;
	}

	int v12 = a2;
	int v14 = a3;
	if (a7) {
		long long v11 = (int)this->field_22 - (int)this->field_20;
		int v111 = v11 >> 32;
		long long v113 = v111 ^ v11;
		long long v114 = v113 - v111;

		int v114i = v114;
		double v114id = v114i;

		double v114id2 = v114id * (a4 - 1.0) * 0.5;
		short v114id2s = v114id2;
		v12 -= v114id2s;

		v11 = this->field_26 - this->field_24;
		v111 = v11 >> 32;
		v113 = v111 ^ v11;
		v114 = v113 - v111;

		v114i = v114;
		v114id = v114i;

		v114id2 = v114id * (a5 - 1.0) * 0.5;
		v114id2s = v114id2;
		v14 -= v114id2s;
	}
	CTkNode* Head = this->ObList.GetHeadPosition();
	int v15 = a9;
	while (v15) {
		this->ObList.GetNext(&Head);
		--v15;
	}

	int a7a = this->ObList.field_10 - a9;
	while (a7a) {
		_49SubListNode* Next = (_49SubListNode*)this->ObList.GetNext(&Head);
		if (Next)
			Next->Draw(v12, v14, a4, a5, a6, a8);
		--a7a;
	}
}

void FFXI::CTk::_49SubList::DrawTile(int a2, int a3, int a4, int a5, unsigned int a6, unsigned int a7, float a8, float a9, unsigned int a10, unsigned int a11, char a12)
{
	CTkNode* Head = this->ObList.GetHeadPosition();
	_49SubListNode* Next = (_49SubListNode*) this->ObList.GetNext(&Head);
	if (Next)
		Next->DrawTile(a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
}
