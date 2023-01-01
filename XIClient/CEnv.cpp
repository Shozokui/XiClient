#include "Globals.h"
#include "CEnv.h"
#include "RegistryConfig.h"
#include <iostream>
#include <intrin.h>
using namespace FFXI::CYy;

const CYyClass CEnv::CEnvClass{
	"CEnv", sizeof(CEnv), &CYyObject::CYyObjectClass
};

const CYyClass* CEnv::GetRuntimeClass()
{
	return &CEnvClass;
}

CEnv::~CEnv() {
	//nullsub
}

CEnv::CEnv() {
	this->field_8 = 0;
	this->field_9 = 0;
	this->field_A = 0;
	this->field_B = 0;
	this->field_C = 0;
	this->field_10 = 1;
	this->field_18 = 0;
	this->field_14 = 0;
	this->Init();
}

void FFXI::CYy::CEnv::Init()
{
	this->CheckCPU();
	this->field_18 = 0;
	this->field_1C = 0;
	this->field_1E = 1;
	this->field_20 = 0;
	this->field_14 = 1;
	this->DoSomething(0.0);
}

void FFXI::CYy::CEnv::CheckCPU()
{
	this->CPUType = FFXI::Constants::Enums::CPUVendorID::Unknown;

	unsigned int v1 = __readeflags();
	__writeeflags(v1 ^ 0x200000);
	unsigned int v2 = __readeflags();
	if (v2 == v1)
		return;

	int data[4] = { 0 };
	char cpuStr[13] = { 0 };
	__cpuid(data, 0);

	*((int*)cpuStr + 0) = data[1];
	*((int*)cpuStr + 1) = data[3];
	*((int*)cpuStr + 2) = data[2];

	if (strcmp(cpuStr, "GenuineTMx86\0") == 0) {
		this->CPUType = FFXI::Constants::Enums::CPUVendorID::Intel;
	}
	else if (strcmp(cpuStr, "AuthenticAMD\0") == 0) {
		this->CPUType = FFXI::Constants::Enums::CPUVendorID::AMD;
	}
	else {
		for (int i = 0; i < 8; ++i) {
			if (cpuStr[i] == 'I') {
				if (memcmp(cpuStr + i, "Intel", 5) == 0) {
					this->CPUType = FFXI::Constants::Enums::CPUVendorID::Intel;
					break;
				}
			}
		}
	}

	__cpuid(data, 1);
	this->field_8 = (data[2] & 0x800000) != 0;
	if (this->CPUType == FFXI::Constants::Enums::CPUVendorID::Intel) {
		this->field_9 = (data[4] & 0x2000000) != 0;
		if ((data[0] & 0xF00) == 0xF00
			|| (data[0] & 0xF00000) == 0xF00000) {
			if ((data[3] & 0x10000000) == 0xF00000) {
				this->field_10 = (data[1] >> 16) & 0xFF;
			}
		}
		if ((data[2] & 1) == 1) {
			this->field_A = 1;
		}

		__cpuid(data, 2);
		if ((unsigned char)data[3] == 37 || (unsigned char)data[3] == 41) {
			this->field_A = 1;
		}
	}
	else if (this->CPUType == FFXI::Constants::Enums::CPUVendorID::AMD) {
		this->field_B = 0;

		__cpuid(data, (int)0x80000000);
		if (data[0] != (int)0x80000000) {
			__cpuid(data, (int)0x80000001);
			if (data[3] < 0) {
				this->field_B = 1;
			}
		}
	}
	else if ((int)this->CPUType == 3) {
		//Don't see this value being set anywhere
		this->field_9 = (data[3] & 0x2000000) != 0;
	}
}

void FFXI::CYy::CEnv::DoSomething(float)
{
	this->field_18 = Config::MainRegistryConfig::dword104458E4;
	if (this->CheckField18()) {
		short v3 = this->field_1C + 1;
		if (v3 >= 4)
			v3 = 0;
		this->field_1C = v3;
	}

}

bool FFXI::CYy::CEnv::CheckField18()
{
	return this->field_18 != 0;
}
