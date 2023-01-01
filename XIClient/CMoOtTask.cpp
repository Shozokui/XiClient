#include "CMoOtTask.h"
using namespace FFXI::CYy;

const CYyClass CMoOtTask::CMoOtTaskClass = {
	"CMoOtTask", sizeof(CMoOtTask), &CMoTask::CMoTaskClass
};

const CYyClass* CMoOtTask::GetRuntimeClass() {
	return &CMoOtTask::CMoOtTaskClass;
}

void CMoOtTask::VirtOt1() {
	//nullsub
}

void __cdecl otTaskCallback(CMoTask* a1) {
	((CMoOtTask*)a1)->VirtOt1();
}
FFXI::CYy::CMoOtTask::CMoOtTask()
{
	this->field_34.Callback = otTaskCallback;
	this->field_34.field_C = this;
}
