#include "CXITimerLow.h"
#include "Globals.h"
#include <iostream>
using namespace FFXI::CYy;

const CYyClass CXITimerLow::CXITimerLowClass{
	"CXITimerLow", sizeof(CXITimerLow), &CXITimer::CXITimerClass
};

const CYyClass* CXITimerLow::GetRuntimeClass(){
	return &CXITimerLowClass;
}

CXITimerLow::CXITimerLow() {
	this->field_C = timeGetTime();
	this->field_14 = 0;
	this->field_18 = 0;
	this->field_1C = 0;
	this->TimerVirt2();
}

CXITimerLow::~CXITimerLow() {
	//nullsub
}

CXITimerLow* CXITimerLow::GetNew() {
	char* mem = StorageLists::instance->Get(sizeof(CXITimerLow), Constants::Enums::MEM_MODE::Ex);
	if (mem) {
		CXITimerLow* newTimer = new (mem) CXITimerLow();
		newTimer->TimerVirt1();
		return newTimer;
	}

	return nullptr;
}

int CXITimerLow::TimerVirt2() { 
	int Time; // eax
	int v3; // ecx
	int v4; // edx
	int result; // eax

	Time = timeGetTime();
	v3 = Time - this->field_C - this->field_1C;
	v4 = this->field_14;
	this->field_C = Time;
	result = 0;
	this->field_14 = v3 + v4;
	this->field_18 = 0;
	this->field_1C = 0;
	return result;
}
double CXITimerLow::TimerVirt3() { 
	DWORD v2{ 0 };
	v2 = timeGetTime() - this->field_1C - this->field_C;
	if (v2)
		this->field_4 = 1000.0 / (float)v2;
	else
		this->field_4 = 30.0;
	return this->field_4;
}
double CXITimerLow::TimerVirt4() {
	return (double)(timeGetTime() - this->field_1C - this->field_C);
}
double CXITimerLow::TimerVirt5() { 
	return this->field_14;
}