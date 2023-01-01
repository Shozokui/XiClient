#include "CXiCollisionActor.h"
#include "XiAtelBuff.h"
#include "RidStruct.h"
#include "CYyDb.h"
#include "CTsZoneMap.h"
#include "Globals.h"
using namespace FFXI::CYy;

const CYyClass CXiCollisionActor::CXiCollisionActorClass{
	"CXiCollisionActor", sizeof(CXiCollisionActor), &CXiControlActor::CXiControlActorClass
};

const CYyClass* FFXI::CYy::CXiCollisionActor::GetRuntimeClass()
{
	return &CXiCollisionActor::CXiCollisionActorClass;
}

char FFXI::CYy::CXiCollisionActor::OnMove()
{
	//sub //TODO
	//lots to do here
	char control_result = this->CXiControlActor::OnMove();
	if (control_result != 0) {
		return control_result;
	}

	bool flag_one{ false };
	bool flag_two{ true };
	if (this->AtelBuffer != nullptr) {
		if ((this->AtelBuffer->field_120 & 0x80) != 0
			|| (this->AtelBuffer->field_120 & 0x100) != 0) {
			flag_one = true;
		}

		if ((this->AtelBuffer->field_120 & 0x100000) != 0
			|| (this->AtelBuffer->field_120 & 0x200000) != 0
			|| (this->AtelBuffer->field_128 & 0x4000) != 0) {
			flag_two = false;
		}
	}

	D3DXVECTOR3 v53{};
	v53.x = this->field_5C4.x;
	v53.y = this->field_5C4.y;
	v53.z = this->field_5C4.z;

	D3DXVECTOR3 v49{};
	v49.x = this->field_D4.x;
	v49.y = this->field_D4.y;
	v49.z = this->field_D4.z;

	bool flag_three{ true };

	if (this->VirtActor201() == true
		|| this->VirtActor204() == true
		|| this->IsFishingRod() == true
		|| this->VirtActor209() == true
		|| this->VirtActor212() == true)
	{
		flag_three = false;
	}

	if (this->VirtActor92() == true) {
		this->field_F4 = 0;
	}

	D3DXVECTOR3 v52 = v49 - v53;
	D3DXVECTOR3 v61 = v53;

	bool flag_four{ false };
	bool flag_five{ false };
	bool do_35{ false };
	if (flag_three == true) {
		float magsq = v52.x * v52.x + v52.y * v52.y + v52.z * v52.z;
		float mag = sqrt(magsq);
		if (this->field_5E0 == 0
			|| mag > 0.0000001 
			|| this->VirtActor242() == 1) {
			if (this->AmIControlActor() == false && this->field_102 == 0) {
				if (this->field_5E1 != 0
					|| this->field_BC <= 10.0
					|| this->VirtActor242() != 0
					|| flag_one == true) {
					this->VirtActor243(0);
					//this->field_5E0 = CYyDb::g_pTsZoneMap->func();
					if (this->field_5E0 == 0) {
						flag_four = true;
					}
					flag_five = true;
					do_35 = true;
				}
				else {
					this->field_5E0 = 1;
				}
			}
			else {
				this->VirtActor243(0);
				D3DXVECTOR3 v51{};
				//this->field_5E0 = CYyDb::g_pTsZoneMap->func2(&v53, &v49, &v51) != 0;
				if (Globals::CheckFloat(v51.x) == true) {
					if (Globals::CheckFloat(v51.y) == true) {
						if (Globals::CheckFloat(v51.z) == true) {
							v49 = v51;
							do_35 = true;
						}
					}
				}
			}
		}
	}

	if (do_35 == true) {
		D3DXVECTOR3 v60{};
		if (this->AmIControlActor() == true) {

		}
		else if (flag_five == false) {
			//if (CYyDb::g_pTsZoneMap->func3(&v49, &v60, nullptr) == false) {
				flag_four == true;
			//}
		}
		if (flag_four == false) {

		}
	}
	else {
		//Label 60
		this->field_5F0 += 1;
		if (this->field_5F0 > 120) {

		}
	}

	//label 70
	if (flag_two == true) {
		this->field_5C4.x = v49.x;
		this->field_5C4.y = v49.y;
		this->field_5C4.z = v49.z;
	}
	else {
		this->field_5C4.x = this->field_D4.x;
		this->field_5C4.y = this->field_D4.y;
		this->field_5C4.z = this->field_D4.z;
	}

	v49.x = this->field_5C4.x;
	v49.y = this->field_5C4.y;
	v49.z = this->field_5C4.z;

	//sub //TODO
	RidStruct* rs = nullptr; //sub here
	if (rs != nullptr) {
		//subs here
	}
	else {
		this->field_5E2 = 0;
		if (flag_two == true) {
			if (flag_four == true) {
				this->field_5C4.x = v61.x;
				this->field_5C4.y = v61.y;
				this->field_5C4.z = v61.z;
				this->field_F4 = 0;
			}
		}
	}

	if (this->field_103 == 0 && this->field_104 == 0) {
		this->field_34 = this->field_5C4;
		this->field_C4 = this->field_5C4;
		this->SetPos(&this->field_5C4);
	}

	return 0;
}

FFXI::CYy::CXiCollisionActor::CXiCollisionActor()
{
	this->field_5E4 = 0;
	this->field_5E0 = 0;
	this->field_5E1 = 0;
	this->field_5E2 = 0;
	this->field_5E8 = rand() % 120;

	this->field_160 = { 0.0, -1.0, 0.0, 1.0 };

	this->field_174 = 0;
	this->field_178 = this->field_5C4.y;
	this->field_17C = 0;
	this->field_5EC = 1;
	this->field_5F4 = 0;
	this->field_5F0 = rand() % 120;
}

int FFXI::CYy::CXiCollisionActor::VirtActor88()
{
	return this->field_5E4;
}

char FFXI::CYy::CXiCollisionActor::VirtActor92()
{
	return this->field_5E0;
}

char FFXI::CYy::CXiCollisionActor::VirtActor93()
{
	return this->field_5E2;
}

D3DXVECTOR4* FFXI::CYy::CXiCollisionActor::VirtActor101()
{
	return &this->field_5C4;
}

char FFXI::CYy::CXiCollisionActor::VirtActor242()
{
	return this->field_5EC;
}

void FFXI::CYy::CXiCollisionActor::VirtActor243(char a2)
{
	this->field_5EC = a2;
}
