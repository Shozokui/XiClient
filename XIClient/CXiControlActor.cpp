#include "CXiSkeletonActor.h"
#include "TkManager.h"
#include "CTkInputCtrl.h"
#include "CYyDb.h"
#include "Globals.h"
#include "XiZone.h"
using namespace FFXI::CYy;

const CYyClass CXiControlActor::CXiControlActorClass{
	"CXiControlActor", sizeof(CXiControlActor), &CXiActor::CXiActorClass
};

bool CXiControlActor::is_first_person_view{ false };
bool CXiControlActor::is_auto_running{ false };
const CYyClass* FFXI::CYy::CXiControlActor::GetRuntimeClass()
{
	return &CXiControlActor::CXiControlActorClass;
}

char FFXI::CYy::CXiControlActor::OnMove()
{
	char retval = this->CXiAtelActor::OnMove();
	if (retval == 0) {
		this->field_5B4.x = this->field_D4.x;
		this->field_5B4.y = this->field_D4.y;
		this->field_5B4.z = this->field_D4.z;

		D3DXVECTOR4 v44{};
		if ((this->field_88 & 1) == 1) {
			v44.x = this->field_D4.y;
			v44.y = this->field_D4.z;
			v44.z = this->field_D4.w;
		}
		else {
			this->field_E4 = this->field_44;
			if (this->field_103 != 0 || this->field_104 != 0) {
				D3DXVECTOR4* v9 = this->VirtActor101();
				v44.x = v9->y;
				v44.y = v9->z;
				v44.z = v9->w;
			}
			else {
				v44.x = this->field_C4.y;
				v44.y = this->field_C4.z;
				v44.z = this->field_C4.w;
			}
		}

		int status = this->GetGameStatus();

		if (this->AmIControlActor() == true) {
			if (CYyDb::g_pCYyDb->field_8 != 0) {

				switch (status) {
				case 0:
				case 1:
				case 4:
				case 28:
				case 31:
					if (this->field_103 != 0) {
						this->ApproachControl(&v44);
					}
					else if (this->field_104 != 0) {
						this->BackJumpControl(&v44);
					}
					else if (this->field_102 != 0) {
						this->BlowBackControl(&v44);
					}
					else if (this->field_101 != 0) {
						this->ActorMoveControl(&v44);
					}
					else {
						this->UserControl(&v44);
					}
					break;
				case 2:
				case 3:
					break;
				case 7:
				case 33:
				case 44:
				case 47:
				case 48:
				case 63:
				case 64:
				case 65:
				case 66:
				case 67:
				case 68:
				case 69:
				case 70:
				case 71:
				case 72:
				case 73:
				case 74:
				case 75:
				case 76:
				case 77:
				case 78:
				case 79:
				case 80:
				case 81:
				case 82:
				case 83:
					if (this->field_102 != 0) {
						this->BlowBackControl(&v44);
					}
					break;
				default:
					if (this->CheckGameStatus5(status) == true
						|| this->CheckGameStatus85(status) == true) {
						if (this->VirtActor201() == false && this->VirtActor204() == false) {
							this->UserControl(&v44);
						}
					}
					break;
				}
			}
			else if (this->field_103 != 0) {
				this->ApproachControl(&v44);
			}
			else if (this->field_104 != 0) {
				this->BackJumpControl(&v44);
			}
			else if (this->field_102 != 0) {
				this->BlowBackControl(&v44);
			}
			else if (this->field_101 != 0) {
				this->ActorMoveControl(&v44);
			}
			this->TryOnKeyDown(&v44);
		}
		else if (this->field_103 != 0) {
			this->ApproachControl(&v44);
		}
		else if (this->field_104 != 0) {
			this->BackJumpControl(&v44);
		}
		else if (
				this->field_102 == 0
				|| this->IsFishingRod() == true
				|| this->VirtActor209() == true
				|| this->VirtActor201() == true
				|| this->VirtActor204() == true
				|| this->VirtActor212() == true
			) {
			if (this->field_101 != 0) {
				this->ActorMoveControl(&v44);
			}
		}
		else {
			this->BlowBackControl(&v44);
		}

		D3DXVECTOR3 v45{};
		v45.x = v44.x - this->field_D4.x;
		v45.y = v44.y - this->field_D4.y;
		v45.x = v44.z - this->field_D4.z;
		float magsq = v45.x * v45.x + v45.y * v45.y + v45.z * v45.z;
		float mag = sqrt(magsq);
		this->field_F8 = mag > 0.02;
		if (this->field_F8 != 0
			|| status != 0 && status != 33 && status != 47 && this->TestSomethingIfAtel(status) == false) {
			this->field_5AE = 1800;
		}

		if (this->field_5AE >= 0) {
			this->field_5AE -= (int)CYyDb::g_pCYyDb->CheckTick();
		}
		if (this->field_5AE < 0) {
			this->field_5AE = 0;
		}

		this->SetPos(&v44);
		this->field_44 = this->field_E4;
		Globals::PiClamp(&this->field_44);

		int v38 = this->VirtActor130() == 0;
		int v39 = this->VirtActor88();

		D3DLIGHT8 v64{};
		XiZone::zone->GetAreaLightByFourCC(&v64, v38, v39);
		this->VirtActor127(&v64);
	}

	this->field_5B4.x = this->field_D4.x - this->field_5B4.x;
	this->field_5B4.y = this->field_D4.y - this->field_5B4.y;
	this->field_5B4.z = this->field_D4.z - this->field_5B4.z;

	return retval;
}

FFXI::CYy::CXiControlActor::CXiControlActor()
{
	this->field_F4 = 0;
	this->field_D4.w = 1.0;
	this->field_F8 = 0;
	this->field_F9 = 1;
	this->field_FB = 0;
	this->field_103 = 0;
	this->field_104 = 0;
	this->field_102 = 0;
	this->field_FC = 0;
	this->field_100 = 0;
	this->field_594 = 0;
	this->field_598 = 0;
	this->field_174 = 0;
	this->field_178 = 0;
	this->field_17C = 0;
	this->field_160 = { 0.0, -1.0, 0.0, 1.0 };

	if (CTk::TkManager::g_pCTkInputCtrl) {
		//sub //TODO
	}

	this->field_5A0.DoSomething(nullptr);

	this->field_5AC = 0;
	this->field_5B0 = 0;
	this->field_108 = 0;
	this->field_10C = 0;
	this->field_110 = 0;
	this->field_114 = 0;
	this->field_101 = 0;
	this->field_118 = 0;
	this->field_11C = 0;
	this->field_5B1 = 0;

	this->field_120 = { 0.0, 0.0, 0.0, 1.0 };
	this->field_130 = { 0.0, 0.0, 0.0, 1.0 };
	this->field_140 = { 0.125, 0.0, 0.0, 0.0 };
	this->field_150 = { 0.0, 0.0, 0.0, 1.0 };

	for (int i = 0; i < sizeof(this->field_180) / sizeof(this->field_180[0]); ++i) {
		memset(this->field_180 + i, 0, sizeof(this->field_180[0]));
		memset(this->field_388 + i, 0, sizeof(this->field_388[0]));
	}

	if (CXiActor::tempAttachStruct.GetSearchActor() == this) {
		//this struct doesn't seem to be used
	}

}

void FFXI::CYy::CXiControlActor::SetPos(D3DXVECTOR4* a2)
{
	this->field_D4 = *a2;
}

void FFXI::CYy::CXiControlActor::ApproachControl(D3DXVECTOR4* a2)
{
	exit(0x100A8640);
	float v20 = CYyDb::g_pCYyDb->CheckTick();
	this->field_108 -= v20;

	if (this->field_108 > 0.0) {
		D3DXVECTOR3 v22{};
		this->GetApproachPoint(&v22);
		D3DXVECTOR3 v21{};
		v21.x = v22.x - a2->x;
		v21.y = v22.y - a2->y;
		v21.z = v22.z - a2->z;
		float magsq = Globals::Vec3Inner(&v21, &v21);
		float mag = sqrt(magsq);
		float attackReach = ((CXiSkeletonActor*)this)->GetAttackReach();
		float diff = mag - attackReach;
		if (diff >= 0.0099999998) {

		}
		else {
			this->field_108 = 0.0;
			this->field_103 = 0;
		}
	}
	else {

	}
}

void FFXI::CYy::CXiControlActor::BackJumpControl(D3DXVECTOR4*)
{
	exit(0x001C4AE0);
}

void FFXI::CYy::CXiControlActor::ActorMoveControl(D3DXVECTOR4*)
{
	exit(0x001C5BC0);
}

void FFXI::CYy::CXiControlActor::BlowBackControl(D3DXVECTOR4*)
{
	exit(0x001C57A0);
}

void FFXI::CYy::CXiControlActor::TryOnKeyDown(D3DXVECTOR4* a2)
{
	if ((this->field_88 & 1) == 0) {
		if (this->VirtActor186() == 0) {
			this->OnKeyDown(a2);
		}
	}
}

void FFXI::CYy::CXiControlActor::OnKeyDown(D3DXVECTOR4*)
{
	//sub //TODO
}

void FFXI::CYy::CXiControlActor::UserControl(D3DXVECTOR4* a2)
{
	if (this->AtelBuffer != nullptr) {
		//sub //TODO
	}
	CXiActor* a = CXiActor::tempAttachStruct.GetSearchActor();
	if (a != nullptr) {
		if (a->AtelBuffer == nullptr) {
			((CXiControlActor*)a)->TryOnKeyDown(a2);
		}
	}
}

void FFXI::CYy::CXiControlActor::GetApproachPoint(D3DXVECTOR3*)
{
	exit(0x100A8B60);
}

D3DXVECTOR4* FFXI::CYy::CXiControlActor::VirtActor101()
{
	return &this->field_D4;
}

D3DXVECTOR4* FFXI::CYy::CXiControlActor::VirtActor102()
{
	return &this->field_E4;
}

D3DXVECTOR4* FFXI::CYy::CXiControlActor::VirtActor122()
{
	return &this->field_160;
}

int FFXI::CYy::CXiControlActor::VirtActor124()
{
	return this->field_174;
}

void FFXI::CYy::CXiControlActor::VirtActor127(D3DLIGHT8* a2)
{
	Globals::CopyLight(&this->field_388[0], a2);
}

void FFXI::CYy::CXiControlActor::VirtActor144(CXiActor* a2)
{
	exit(0x100A85C0);
}

bool FFXI::CYy::CXiControlActor::AmIControlActor()
{
	return CXiActor::tempAttachStruct.GetSearchActor() == this;
}

int FFXI::CYy::CXiControlActor::VirtActor186()
{
	return this->field_FC;
}

char FFXI::CYy::CXiControlActor::VirtActor194()
{
	return this->field_F9;
}

char FFXI::CYy::CXiControlActor::VirtActor198()
{
	return this->field_FB;
}
