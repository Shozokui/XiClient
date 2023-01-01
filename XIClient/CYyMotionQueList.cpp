#include "CYyMotionQueList.h"
#include "CYyMotionQue.h"
#include "Globals.h"
#include "StorageLists.h"
#include "CMoMo2.h"

using namespace FFXI::CYy;

const CYyClass CYyMotionQueList::CYyMotionQueListClass{
	"CYyMotionQueList", sizeof(CYyMotionQueList), &CYyObject::CYyObjectClass
};

const CYyClass* FFXI::CYy::CYyMotionQueList::GetRuntimeClass()
{
	return &CYyMotionQueList::CYyMotionQueListClass;
}

FFXI::CYy::CYyMotionQueList::CYyMotionQueList()
{
	this->Head = nullptr;
	this->field_8 = 1.0;
}

FFXI::CYy::CYyMotionQueList::~CYyMotionQueList()
{
	this->DeleteAll();
}

void FFXI::CYy::CYyMotionQueList::AppendSync(CMoMo2** a2, float a3, float a4, int a5, float a6, float a7, float a8, float a9)
{
	CYyMotionQue* tail = this->GetTail();
	if (a2 == nullptr) {
		//The motion resource handle is NULL
		return;
	}

	char* mem = StorageLists::instance->Get(sizeof(CYyMotionQue), FFXI::Constants::Enums::MEM_MODE::Ex);
	if (mem != nullptr) {
		CYyMotionQue* mque = new (mem) CYyMotionQue();
		if (tail != nullptr) {
			tail->Previous = mque;
		}
		else {
			this->Head = mque;
		}

		if (a6 == 0.0)
			a6 = 1.0;

		double v14 = 1.0 / a6;
		mque->field_14 = a6;
		mque->field_C = 0;
		mque->field_10 = 0.0;
		mque->field_38 = a9;
		mque->Frame = a8;
		mque->field_1C = a3;
		mque->field_8 = 1;
		mque->field_18 = v14;
		mque->SetSpeed(a4);
		mque->field_2C = (char)a5;
		mque->field_30 = (*a2)->Header.FourCC;
		mque->SetMod(a2);
		CMoResource::DoTheThing(*a2);

		if (a7 < 0.0)
			a7 = 1.0;

		mque->field_20 = a7;
		if (tail != nullptr) {
			CMoMo2** p_tail = (CMoMo2**)tail->GetMod();
			CMoMo2** p_que = (CMoMo2**)mque->GetMod();

			if (p_tail != nullptr && p_que != nullptr) {
				CMoMo2* tailmo2 = *p_tail;
				CMoMo2* quemo2 = *p_que;
				float offset = 0.0;
				float scale = 0.0;

				float v2 = (double)*(short*)(tailmo2->Data + 4) / *(float*)(tailmo2->Data + 6);
				float v3 = (double)*(short*)(quemo2->Data + 4) / *(float*)(quemo2->Data + 6);

				if (v2 > 0.001) {
					v2 = 1.0 / v2;
					scale = tail->GetFrame() * v2;
					offset = v2 * v3 * (*(float*)(quemo2->Data + 6) * 0.5) * a4 * a6;
				}

				double v31 = scale * v3 + offset;

				if (v3 <= 0.0) {
					v31 = 0.0;
				}
				else {
					while (v31 >= v3) {
						v31 -= v3;
					}
				}
				mque->Frame = *(float*)(quemo2->Data + 6) * v31;
			}

			tail->Kill(a6);
		}

		int nbque = this->GetNbQue();
		CYyMotionQue* head = this->Head;
		while (nbque > 3) {
			head->Kill(1.0);
			head = (CYyMotionQue*)head->Previous;
			nbque -= 1;
		}

		if (head != nullptr)
			head->field_10 = 1.0;
	}
	else {
		//Failed to allocate motion que memory
	}
}

void FFXI::CYy::CYyMotionQueList::Append(CMoMo2** a2, float a3, float a4, int a5, float a6, float a7, float a8, float a9)
{
	this->AppendSync(a2, a3, a4, a5, a6, a7, a8, a9);
	CYyMotionQue* tail = this->GetTail();
	if (tail != nullptr)
		tail->Frame = a8;
}

void FFXI::CYy::CYyMotionQueList::Remove(CYyMotionQue* a2)
{
	if (this->Head == a2) {
		this->Head = (CYyMotionQue*)a2->Previous;
		delete a2;
	}
	else {
		CYyMotionQue* que = this->GetParent(a2);
		if (que != nullptr) {
			que->Previous = a2->Previous;
			delete a2;
		}
		else {
			//Motion requests may be accumulated. Please contact Yamamoto.
		}
	}
}

void FFXI::CYy::CYyMotionQueList::DeleteAll()
{
	CYyMotionQue* head = this->Head;
	while (head != nullptr) {
		CYyMotionQue* prev = (CYyMotionQue*)head->Previous;
		this->Remove(head);
		head = prev;
	}
	this->Head = nullptr;
}

int FFXI::CYy::CYyMotionQueList::GetNbQue()
{
	int count = 0;

	CYyMotionQue* head = this->Head;
	while (head) {
		count += 1;
		head = (CYyMotionQue*)head->Previous;
	}
	return count;
}

void FFXI::CYy::CYyMotionQueList::UpdateFloats(int a2, MotionManStaticStruct* a3)
{
	this->field_C = 0.0;
	CYyMotionQue* v4 = nullptr;
	CYyMotionQue* head = this->Head;
	while (head) {
		CYyQue* prev = head->Previous;
		if (prev == nullptr || prev->field_10 != 1.0) {
			head->CalcMotion(a2, this->field_10, a3, v4);
		}
		if (a2 != 0) {
			this->field_C = (1.0 - head->field_10) * this->field_C + head->field_1C * head->field_10;
		}
		v4 = head;
		head = (CYyMotionQue*)prev;
	}
}

bool FFXI::CYy::CYyMotionQueList::IsExistZombiQue()
{
	CYyMotionQue* head = this->Head;
	while (head != nullptr) {
		if (head->IsZombi() == true)
			return true;

		head = (CYyMotionQue*)head->Previous;
	}
	return false;
}

CYyMotionQue* FFXI::CYy::CYyMotionQueList::GetParent(CYyMotionQue* a2)
{
	CYyMotionQue* result = this->Head;

	while (result != nullptr) {
		if (result->Previous == a2)
			return result;
		result = (CYyMotionQue*)result->Previous;
	}

	return nullptr;
}

CYyMotionQue* FFXI::CYy::CYyMotionQueList::GetTail()
{
	CYyMotionQue* tail = this->Head;
	while (tail)
		tail = (CYyMotionQue*)tail->Previous;

	return tail;
}
