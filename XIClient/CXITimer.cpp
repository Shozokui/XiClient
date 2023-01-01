#include "CXITimer.h"
#include "Globals.h"
#include <stdlib.h>
#include <iostream>

using namespace FFXI::CYy;

const CYyClass CXITimer::CXITimerClass{
	"CXITimer", sizeof(CXITimer), &CYyObject::CYyObjectClass
};
const CYyClass* CXITimer::GetRuntimeClass() {
	return &CXITimerClass;
}

CXITimer::CXITimer() {
	this->field_4 = 1.0;
	this->field_8 = 1.0;
}

CXITimer::~CXITimer() {
	//nullsub
}

void CXITimer::TimerVirt1() {}

double CXITimer::TimerVirt6() { return this->field_8; }
float CXITimer::TimerVirt7(float p_value) {
	this->field_8 = p_value;
	return p_value;
}
