#include "CXiSkeletonActor.h"
#include "CXiSkeletonActorRes.h"
#include "CYyMotionQue.h"
#include "CYyModelDt.h"
#include "XiAtelBuff.h"
#include "Globals.h"
#include "CDx.h"
#include "FsConfig.h"
#include "Strings.h"
#include "DatIndices.h"
#include "StorageLists.h"
#include "CYyDb.h"
#include "CYyCamMng2.h"
#include "CMoTaskMng.h"
#include "CMoResourceMng.h"
#include "CYyResourceFile.h"
#include "CMoSk2.h"
#include "CMoMo2.h"
#include "CMoOs2.h"
#include "CYyModelBase.h"
#include "CMoCib.h"
#include "RaceModelTables.h"
#include "EquipReferenceRead.h"
#include "MotionReferenceRead.h"
#include "CYyModel.h"
#include "ResourceList.h"
#include "XiZone.h"
#include "WMatrix.h"
#include "KzFQuat.h"
#include "PlaceHolders.h"
#include "CYyResfList.h"
#include "CYyTexMng.h"

using namespace FFXI::CYy;

const CYyClass CXiSkeletonActor::CXiSkeletonActorClass{
	"CXiSkeletonActor", sizeof(CXiSkeletonActor), &CXiCollisionActor::CXiCollisionActorClass
};

D3DLIGHT8* CXiSkeletonActor::g_light{ nullptr };
D3DLIGHT8 CXiSkeletonActor::g_light_arr[4]{};
float CXiSkeletonActor::skeleton_subsitute[18] = { 0.0 };
D3DCOLOR CXiSkeletonActor::ActorAmbientLight{};

char is_motion_append{ 0 };

const CYyClass* FFXI::CYy::CXiSkeletonActor::GetRuntimeClass()
{
	return &CXiSkeletonActor::CXiSkeletonActorClass;
}

char FFXI::CYy::CXiSkeletonActor::OnMove()
{
	//sub //TODO
	this->Model.field_30 &= 0x10u;
	this->field_B2 &= 0xFFFE;
	this->field_60C.x = 0.0;
	this->field_60C.y = 0.0;
	this->field_60C.z = 0.0;
	this->field_62C.x = 0.0;
	this->field_62C.y = 0.0;
	this->field_62C.z = 0.0;
	if (this->InOwnActorPointers() == false) {
		if (this->CheckAtel13CBit20() == true) {
			if (this->field_8BC < 0.0) {
				this->field_8BC = 32.0;
				this->field_8B6 = 1;
			}
			this->SetAtel13CBit20(false);
		}

		if (this->field_8BC >= 0.0) {
			this->field_8BC -= CYyDb::g_pCYyDb->CheckTick();
		}
	}

	D3DXVECTOR4* v101 = this->VirtActor101();
	D3DXVECTOR4* v102 = this->VirtActor102();
	D3DXVECTOR4 v287 = *v101;
	D3DXVECTOR4 v290 = *v102;

	char collisionResult = this->CXiCollisionActor::OnMove();
	if (collisionResult == 0) {
		this->ReadMotionPackPolling();

		D3DXVECTOR4 v277{};
		if (this->field_103 != 0
			|| this->field_104 != 0
			|| this->VirtActor93() != 0
			|| this->field_102 != 0
			|| this->field_101 != 0) {
			v277 = this->field_5C4 - *this->VirtActor101();
			v277.w = 0.0;
		}
		else if ((this->CheckGameStatus5(255) == true
			|| this->CheckGameStatus85(255) == true) 
			&& this->field_768[0] == nullptr) {
			v277 = { 0.0, 0.0, 0.0, 1.0 };
		}
		else {
			D3DXVECTOR4 a1 = *this->VirtActor101();
			D3DXVECTOR4 a2 = this->field_5C4;
			float v16 = CYyDb::g_pCYyDb->CheckTick();
			while (v16 > 0.0) {
				v277 = a2 - a1;
				v277.x *= 0.125;
				v277.y *= 0.125;
				v277.z *= 0.125;
				a1 += v277;
				v16 -= 1.0;
			}
			v277 = a1 - *this->VirtActor101();
			v277.w = 0.0;
		}
		D3DXVECTOR4 a2{};
		//label_23
		bool v28 = false;
		if (this->IsDirectionLock() == false
			&& this->field_102 == 0
			&& this->field_7A4 == -1) {
			if (this->field_86C != 0) {
				if (this->field_870 > 0.0) {
					this->field_61C.y += this->field_874;
					if (this->field_61C.y > FFXI::Constants::Values::ANGLE_PI) {
						this->field_61C.y -= FFXI::Constants::Values::ANGLE_2PI;
					}
					else if (this->field_61C.y < FFXI::Constants::Values::ANGLE_MINUS_PI) {
						this->field_61C.y += FFXI::Constants::Values::ANGLE_2PI;
					}

					double v37 = this->field_874;
					//sub //TODO make sure this is correct
					if (v37 < 0.0)
						v37 *= -1;
					double v40 = this->field_870 - v37;
					if (v40 < 0.0) {
						if (this->field_874 > 0.0) {
							v40 *= -1;
						}
						this->field_61C.y += v40;
						if (this->field_61C.y > FFXI::Constants::Values::ANGLE_PI) {
							this->field_61C.y -= FFXI::Constants::Values::ANGLE_2PI;
						}
						else if (this->field_61C.y < FFXI::Constants::Values::ANGLE_MINUS_PI) {
							this->field_61C.y += FFXI::Constants::Values::ANGLE_2PI;
						}
						this->field_870 = 0.0;
					}
					else {
						this->field_870 = v40;
					}
				}
			}
			else if (this->AmIControlActor() == true
				&& this->VirtActor198() != 0
				&& (this->CheckGameStatus5(255) == true
				|| this->CheckGameStatus85(255) == true	)
				|| this->VirtActor194() == 0) {
				a2.y = this->field_E4.x;
				a2.z = this->field_E4.y;
				a2.w = this->field_E4.z;
				v277.x = 0.125;
				if (Placeholder::IsEvent(255) == true) {
					int something = this->VirtActor69();
					if (something != 0)
						v277.x = something * 0.00125;
				}
				double v49 = CYyDb::g_pCYyDb->CheckTick();
				D3DXVECTOR4 v278{};
				while (v49 > 0.0) {
					v278.x = a2.y - this->field_61C.x;
					v278.y = a2.z - this->field_61C.y;
					v278.z = a2.w - this->field_61C.z;
					Globals::PiClamp(&v278);
					v278 *= v277.x;
					v278.w = 0;
					this->field_61C += v278;
					v49 -= 1.0;
				}

				Globals::PiClamp(&this->field_61C);
			}
			else {
				D3DXVECTOR4 v286 = this->field_5B4;
				D3DXVECTOR3 v287 = { v277.y, v277.z, v277.w };
				Globals::Vec3Normalize((D3DXVECTOR3*) &v286);
				Globals::Vec3Normalize(&v287);
				v286.y = 0.0;
				v287.y = 0.0;
				long double v87{};
				double v3{};
				if (this->AmIControlActor() == true
					&& Globals::Vec3Inner((D3DXVECTOR3*) &v286, &v287) < 0.0) {
					a2.y = this->field_E4.x;
					a2.z = this->field_E4.y;
					a2.w = this->field_E4.z;
					v87 = v286.z;
					v3 = v286.x;
				}
				else {
					double v88;
					if (v277.z >= 0.0) {
						v3 = v277.y * v277.y;
						v88 = v277.w * v277.w + v3;
					}
					else {
						v88 = v277.y * v277.y + v277.z * v277.z + v277.w * v277.w;
					}
					v277.x = sqrt(v88);
					a2.y = this->field_E4.x;
					a2.z = this->field_E4.y;
					a2.w = this->field_E4.z;
					if (this->AtelBuffer == nullptr
						|| this->AtelBuffer->GetSomeActor() == nullptr) {
						if (v277.x < 0.0099999998) {
							double w = v277.w;
							if (w < 0.0)
								w = -w;
							if (w <= 0.0099999998) {
								double v102 = v277.y;
								if (v102 < 0.0)
									v102 = -v102;
								if (v102 > 0.0099999998) {
									v87 = v277.w;
									v3 = v277.y;
									v28 = true;
									a2.z = -atan2(v87, v3);
								}
							}
							else {
								v87 = v277.w;
								v3 = v277.y;
								v28 = true;
								a2.z = -atan2(v87, v3);
							}
						}
					}

					//label 95
					v277.x = 0.125;
					if (Placeholder::IsEvent(255) == true) {
						int something = this->VirtActor69();
						if (something != 0)
							v277.x *= something;
					}
					double v108 = CYyDb::g_pCYyDb->CheckTick();
					D3DXVECTOR4 v278{};
					while (v108 > 0.0) {
						v278.x = a2.y - this->field_61C.x;
						v278.y = a2.z - this->field_61C.y;
						v278.z = a2.w - this->field_61C.z;
						Globals::PiClamp(&v278);
						v278 *= v277.x;
						v278.w = 0.0;
						this->field_61C += v278;
						v108 -= 1.0;
					}

					Globals::PiClamp(&this->field_61C);
					if (v28 == true && CXiControlActor::is_first_person_view == false) {
						this->field_44 = this->field_61C;
						Globals::PiClamp(&this->field_44);
					}
				}
			}
		}

		//Label 139
		if (this->field_7A4 != -1) {
			//sub //TODO this block
			exit(0x100C4FF1);
			CXiActor* act = this->field_7A8.GetSearchActor();
			if (this == act || act == nullptr) {
				this->field_7A4 = -1;
			}
			else {
				CXiSkeletonActor* skac = (CXiSkeletonActor*)act;
				if (this->field_7A4 < 48
					|| this->field_7A4 >= 61) {
					D3DXVECTOR4 a1{};
					skac->VirtActor103(this->field_7A4, &a1);
				}
				else {
					//sub //TODO
				}
			}
		}

		double v180 = v277.y * v277.y + v277.z * v277.z + v277.w * v277.w;
		v180 = sqrt(v180);
		if (v180 < 0.0099999998) {
			D3DXVECTOR4* v181 = this->VirtActor101();
			v181->x += v277.y;
			v181->y += v277.z;
			v181->z += v277.w;
			v277.y = 0.0;
			v277.z = 0.0;
			v277.w = 0.0;
		}

		D3DXVECTOR4* v182 = this->VirtActor101();
		v182->x += v277.y;
		v182->y += v277.z;
		v182->z += v277.w;

		if (this->field_B4 != nullptr
			&& CMoResourceMng::CheckResourceThing((CMoResource***) &this->field_B4) == true) {

			if (this->field_80 != 0) {
				if (this->field_103 != 0
					|| this->field_104 != 0
					|| this->field_101 != 0
					|| this->VirtActor209() == true
					|| this->VirtActor219() == true
					|| this->VirtActor223() == true) {
					this->DefaultMove((D3DXVECTOR3*)& v277.y);
				}
				else if (this->field_102 == 0) {
					if (this->VirtActor201() == false
						&& this->VirtActor204() == false
						&& this->IsFishingRod() == false
						&& this->VirtActor209() == false
						&& this->VirtActor212() == false
						&& this->VirtActor215() == false
						&& this->VirtActor219() == false
						&& this->VirtActor223() == false) {
						if (this->CheckGameStatus5(255) == true) {
							exit(0x100C5335);
						}
						else if (this->CheckGameStatus85(255) == true) {
							exit(0x100C5359);
						}
						else {
							int status = this->GetGameStatus();
							if (status == 6
								|| status == 38
								|| status == 39
								|| status == 40
								|| status == 41
								|| status == 42
								|| status == 43
								|| status == 50
								|| status == 56
								|| status == 57
								|| status == 58
								|| status == 59
								|| status == 60
								|| status == 61
								|| status == 62
								|| status == 51
								|| status == 52
								|| status == 53) {
								exit(0x100C548A);
							}
							else {
								if (this->TestSomethingIfAtel(255) == true)
									exit(0x100C547C);
							}
						}
					}
				}
			}
			else if (this->VirtActor67() != 0) {
				if (this->field_102 != 0) {
					exit(0x100C554C);
				}
				else if (this->field_101 != 0
					|| this->VirtActor209() == true
					|| this->VirtActor219() == true) {
					this->DefaultMove((D3DXVECTOR3*)&v277.y);
				}
			}
			else if (this->field_102 == 0) {
				if ( this->field_101 == 0
					&& this->VirtActor201() == false
					&& this->VirtActor204() == false
					&& this->IsFishingRod() == false
					&& this->VirtActor209() == false
					&& this->VirtActor212() == false
					&& this->VirtActor215() == false
					&& this->VirtActor219() == false
					&& this->VirtActor223() == false
					&& this->VirtActor75() == false) {
					this->DefaultMove((D3DXVECTOR3*)&v277.y);
					//sub //sub
				}

			}
			

			//i think label 222 here
		}

		//Label 228 hopefully
		//Label 301 hopefully
	}

	this->field_9D8 = v287;
	this->field_9D8.w = 1.0;

	return collisionResult;
}

bool FFXI::CYy::CXiSkeletonActor::VirtSkeletonActor247()
{
	return false;
}

void __cdecl ResCallback() {
	throw "NOT IMPLEMENTED";
	exit(0xDEADBEEF);
}

const unsigned short SomeEquipTable[8] = {
	0x0, 0x3F, 0x75, 0x8F, 0x1D7, 0x1FF, 0x280, 0x2BF
};

void LoadRaceActor(CXiSkeletonActor* actor, int a2) {
	int race = actor->GetRace();
	if (race >= 9 && race < 29 || race >= 37) {
		//race number is wrong
		race = 1;
	}

	CYyResourceFile** numfile{ nullptr };
	int index = FFXI::Constants::DatIndices::base_skeleton_no_tab[race];
	FFXI::CYyDb::g_pCYyDb->pCMoResourceMng->LoadNumFile(&numfile, index);
	if (CMoResourceMng::CheckResourceThing((CMoResource***)&numfile)) {
		actor->SetFile(numfile, actor->field_9EC);
	}
	else {
		exit(0x100D1C14);
		//sub //TODO
	}

	if (race < sizeof(RaceModelTable) / sizeof(RaceModelTable[0])) {
		const RaceModelTableEntry* racetable = RaceModelTable[race];

		for (int category = 0; category < 9; ++category) {
			if (race >= 0x1D && category > 5)
				break;

			int equipnum = actor->GetEquipNum(category);
			int tableindex = equipnum >> 12;
			if (tableindex != category) {
				tableindex = category;
				equipnum = (category << 12) | (equipnum & 0xFFF);
			}

			//racetable is incomplete
			const RaceModelTableEntry* equiptable = racetable + tableindex;
			if (equiptable->field_0) {
				int total = equiptable->field_24 + equiptable->field_4 + equiptable->field_C
					+ equiptable->field_14 + equiptable->field_1C + equiptable->field_2C;
				if ((equipnum & 0xFFF) >= total) {
					//wrong GRP number
					equipnum = tableindex << 12;
				}

				int equipnum12 = equipnum & 0xFFF;
				if (tableindex == 7) {
					actor->field_8B5 = 1;
					for (int v84 = 0; v84 < 8; v84 += 2) {
						if (equipnum12 < SomeEquipTable[v84] || equipnum12 > SomeEquipTable[v84 + 1]) {
							actor->field_8B5 = 0;
							break;
						}
					}
				}

				if (racetable) {
					int offset = 0;
					int sum = 0;
					if (equipnum12 < equiptable->field_4) {
						offset = equiptable->field_0;
					}
					else if (sum += equiptable->field_4, equipnum12 < sum + equiptable->field_C) {
						offset = equiptable->field_8 - sum;
					}
					else if (sum += equiptable->field_C, equipnum12 < sum + equiptable->field_14) {
						offset = equiptable->field_10 - sum;
					}
					else if (sum += equiptable->field_14, equipnum12 < sum + equiptable->field_1C) {
						offset = equiptable->field_18 - sum;
					}
					else if (sum += equiptable->field_1C, equipnum12 < sum + equiptable->field_24) {
						offset = equiptable->field_20 - sum;
					}
					else if (sum += equiptable->field_24, equipnum12 < sum + equiptable->field_2C) {
						offset = equiptable->field_28 - sum;
					}
					else
						continue;

					int DATIndex = equipnum12 + offset;
					CYyResourceFile** equipfile = FFXI::CYyDb::g_pCYyDb->pCMoResourceMng->GetOrLoadDatByIndex(DATIndex);
					if (CMoResourceMng::CheckResourceThing((CMoResource***)&equipfile) == true) {
						actor->SetEquipModel(equipfile, equipnum, actor->field_9EC, true);
					}
					else {
						FFXI::File::EquipReferenceRead* refread = new FFXI::File::EquipReferenceRead();
						if (refread) {
							refread->ActorIndex = actor->field_9EC;
							refread->field_14 = 0;
							refread->field_4.DoSomething(actor);
							refread->field_10 = 0;
							refread->EquipModelID = equipnum;
							refread->DATIndex = DATIndex;
							CYyResourceFile::ReferenceRead(&equipfile, FFXI::File::EquipReferenceRead::ReadCallback, refread, actor, nullptr);
						}
					}
				}
			}
		}
	}

	if (race < 9) {
		if (a2 >= 0) {
			actor->field_8A0 = a2;
			actor->field_8A4 = 4;
		}
		else {
			actor->Negativea2();
		}
	}
}

FFXI::CYy::CXiSkeletonActor::CXiSkeletonActor(int a2)
{
	this->field_734 = 0;
	this->field_750 = 0;
	this->field_744 = 0.1;
	this->field_740 = 0;
	this->field_73C = 0;
	this->field_74C = 0;
	this->field_730 = 72;
	this->field_738 = 0;

	//sub //TODO

	this->Init();
	this->field_8C4 = a2;

	this->field_5FC = *(D3DXVECTOR4*)&CYyDb::g_pCYyDb->CameraManager->field_50;
	this->field_5FC.w = 1.0;

	this->field_9D8 = this->field_5FC;
	this->Model.field_4 = this->field_5FC;

	//inputctrl stuff //SUB //TODO

	int ActorType = this->GetType();
	if (this->GetGameStatus() == 1) {
		if (ActorType == 0 || ActorType == 6)
			this->field_83C[0] = 1;
	}

	if (this->GetMonsterFlag() == 1) {
		if (ActorType == 2 || ActorType == 7)
			this->field_83C[0] = 1;
	}

	this->TaskMngShift4(4);
	//ZoneStartOK //sub //TODO
}

void FFXI::CYy::CXiSkeletonActor::SetUp(int a2)
{
	bool v3{ false };
	if (this->AtelBuffer) {
		if ((this->AtelBuffer->field_120 & 0x80) != 0
			|| (this->AtelBuffer->field_120 & 0x100) != 0) {
			v3 = true;
		}
	}

	if (this->AmIControlActor() || v3 == true) {
		this->SetModels(a2);
	}
	else {
		char* mem = StorageLists::instance->Get(sizeof(CXiSkeletonActorRes), FFXI::Constants::Enums::MEM_MODE::Ex);
		if (mem) {
			CMoTaskMng::DeleteThisTask = true;
			this->field_A08 = new (mem) CXiSkeletonActorRes(this, ResCallback);
			this->field_A08->field_44 = a2;
		}
		else {
			this->SetModels(a2);
		}
	}
}

void FFXI::CYy::CXiSkeletonActor::Init()
{
	this->field_5F8 = 0;
	this->field_764 = 0;
	memset(this->field_768, 0, sizeof(this->field_768));
	this->field_798 = 0;
	this->SetMotionLock(1);
	this->field_840 &= ~0x800000u;
	this->field_838 = 0;

	this->SetConstrain(0, 0);
	this->SetConstrain(0, 1);
	this->VirtActor191(0, 0);
	this->VirtActor191(0, 1);

	this->field_7F8 = 0x20202020;
	this->field_7FC = 0;
	this->field_800 = 0;
	this->field_804 = 0x20202020;
	this->field_80C = 0x20202020;
	this->field_810 = 0;
	this->field_808 = 0;
	this->field_814 = 0;
	this->field_818 = 0;

	this->field_840 &= 0xFC;
	this->field_848 = { 20.0, 0.0, 0.0, -1.0 };
	this->field_82C = { 0.0, -1.0, 0.0 };
	this->field_828 = 0;

	this->field_61C = { 0.0, 0.0, 0.0, 1.0 };
	this->field_5FC = { 0.0, 0.0, 0.0, 1.0 };
	this->field_44 = this->field_5FC;
	Globals::PiClamp(&this->field_44);

	this->field_9D8 = { 0.0, 0.0, 0.0, 1.0 };
	this->field_60C = { 0.0, 0.0, 0.0, 1.0 };
	this->field_62C = { 0.0, 0.0, 0.0, 1.0 };

	this->field_840 &= 0xFF8FFFFF;

	this->field_758 = -1.0;
	this->field_75C = -1.0;
	this->field_760 = -1.0;
	this->field_65C = 0;
	this->field_754 = 1.0;
	this->field_660 = 0x80808080;
	this->field_664 = 1.0;
	this->field_59C = 0;
	this->field_668 = 128;
	this->field_66C = 128;
	this->field_670 = 0;
	this->field_8C8.field_C = 0;
	this->field_8E8 = 1;
	this->field_8EC = 0;
	this->field_7A0 = 0;
	this->field_7E8 = 0;
	this->field_7EC = 0;
	this->field_7F0 = 0;
	this->field_7F4 = 0;
	this->field_8B8 = 120.0;
	this->field_9E8 = 0;
	this->field_81C = 0;
	this->field_820 = 0;
	this->field_8B5 = 0;

	this->SetSomeFlag(0);
	this->field_824 = 0;

	strcpy_s((char*)this->field_7C8, sizeof(this->field_7C8), FFXI::Constants::Strings::SkeletonAnimations);
	this->field_7C8[4] = 0;
	this->field_7DC = 1.0;
	this->field_7E0 = 1.0;
	this->field_8C4 = 0;
	this->field_7C4 = 0;
	this->field_7E4 = nullptr;
	this->field_8A0 = 0;
	this->field_8A4 = 0;
	this->VirtActor32(8);
	this->VirtActor34(2);
	this->VirtActor36(2);
	this->field_8C0 = 0;
	this->VirtActor38(128);
	this->VirtActor40(128);
	this->field_8B0 = 0;
	this->field_8B4 = 1;
	this->field_8AA = 0;
	this->field_8AB = 0;
	this->SetModelWep(0);
	this->field_8AC &= 0xF8;
	this->field_8BC = 0;
	this->field_8B6 = 0;
	this->field_86C = 0;
	this->field_870 = 0;
	this->field_874 = 0;
	this->veve_texinfo = 0;
	this->null_maxresource = 0;
	this->ohno_texinfo = 0;
	this->some_cubemap = 0;
	this->field_864 = 0;
	this->field_840 = this->field_840 & 0xF9F00003 | 0x7FF00;
	this->field_844 = 0;
	this->field_88 &= 0xFFFFF9FF;
	this->field_B2 = 1;
	this->field_B0 = 1;
	this->field_7A4 = -1;

	this->field_7A8.DoSomething(0);
	this->field_7B4 = { 0.0, 0.0, 0.0, 1.0 };

	this->VirtActor11();
	this->field_9EC = CXiActor::skeletonActorIndex++;

	this->field_9F4 = 1;
	this->field_9F8 = 1;
	this->field_A04 = 1;
	this->field_A00 = 0;
	this->field_A08 = nullptr;

	//sub //TODO some globals
}

void FFXI::CYy::CXiSkeletonActor::SetModelWep(int a2)
{
	CYyModelBase* base = *this->Model.GetBase();
	if (base == nullptr) return;

	//throw "not implemented";
	exit(0x100D41C0);
}

void FFXI::CYy::CXiSkeletonActor::SetEquipModel(CYyResourceFile** a2, int p_equipModelID, int p_actorIndex, bool a5)
{
	int v5 = (p_equipModelID >> 12) & 0xF;
	if (a5 == false) {
		this->field_8B0 |= 1 << v5;
		return;
	}

	if (a2 == nullptr) return;
	if (p_actorIndex != this->field_9EC) return;

	int actortype = this->GetType();
	if (actortype == 6) {
		if (v5 == 9) {
			if (this->GetRace() == 0) {
				this->VirtActor154(a2);
			}
			else {
				this->field_864 = a2;
				CMoResource::DoTheThing(*a2);
			}
		}
	}
	else if (actortype == 7) {
		if (v5 == 9) {
			if (this->GetEquipNum(0) == 0) {
				this->VirtActor154(a2);
			}
			else {
				this->field_864 = a2;
				CMoResource::DoTheThing(*a2);
			}
		}
	}

	CYyModelBase* base = *this->Model.GetBase();
	if (base == nullptr) {
		base = this->Model.CreateBase();
		if (base == nullptr)
			return;
	}

	this->AddOS2s((CMoResource**)a2, base);

	CMoCib** pcib{ nullptr };
	CYyDb::g_pCYyDb->pCMoResourceMng->FindNextUnder((CMoResource***)&pcib, *a2, FFXI::Constants::Enums::ResourceType::Cib, 0);
	if (pcib && *pcib) {
		CMoCib* cib = *pcib;
		switch (v5) {
		case 2:
			this->CibCollect.waist_type = cib->info.waist_type;
			break;
		case 5:
			this->CibCollect.material_foot = cib->info.material_foot;
			this->CibCollect.size_foot = cib->info.size_foot;
			break;
		case 6:
			this->CibCollect.amot_wep0 = cib->info.amot_wep0;
			this->CibCollect.inout_wep0 = cib->info.inout_wep0;
			this->CibCollect.constrain_no = cib->info.constrain_no;
			this->CibCollect.atksch_wep = cib->info.atksch_wep;
			this->CibCollect.field_13 = cib->info.legs4;
			break;
		case 7:
			this->CibCollect.inout_wep1 = cib->info.inout_wep0;
			this->CibCollect.field_10 = cib->info.constrain_no;
			this->CibCollect.field_11 = cib->info.atksch_wep;
			if (this->field_8B5 != 0) {
				this->CibCollect.is_shield = 0;
				this->CibCollect.field_12 = cib->info.amot_wep0;
			}
			else {
				this->CibCollect.field_12 = 0;
				this->CibCollect.is_shield = cib->info.is_shield;
			}
			break;
		case 8:
			this->CibCollect.field_E = cib->info.field_E;
			break;
		default:
			break;
		}
	}

	this->field_8B0 |= 1 << v5;
}

bool FFXI::CYy::CXiSkeletonActor::SetModels(int a2)
{
	bool v4{ false };
	if (this->AtelBuffer) {
		if ((this->AtelBuffer->field_120 & 0x80) != 0
			|| (this->AtelBuffer->field_120 & 0x100) != 0)
			v4 = true;
	}

	if (this->VirtActor203()) {
		//throw "not implemented";

		int race = this->GetRace();
		if ( (race >= 9 && race < 29) || race >= 37)
			race = 32;
		
		exit(1232);
	}
	
	if (this->VirtActor201()) {
		//throw "not implemented";

		exit(1233);
	}
	
	if (this->VirtActor204()) {
		//throw "not implemented";

		exit(1234);
	}
	
	if (this->IsFishingRod()) {
		//throw "not implemented";

		exit(1235);
	}
	
	if (this->VirtActor209()) {
		exit(1236);
	}
	
	if (this->VirtActor223() || this->VirtActor219()) {
		exit(1237);
	}
	
	if (this->VirtActor215()) {
		exit(1238);
	}

	if (this->VirtActor212()) {
		exit(1239);
	}

	int Type = this->GetType();
	switch (Type) {
	case 0:
	case 1:
		LoadRaceActor(this, a2);
		return true;
	default:
		exit(1240);
	}
}

void FFXI::CYy::CXiSkeletonActor::Set5FC(D3DXVECTOR4* a2)
{
	if (this->AtelBuffer) {
		this->AtelBuffer->field_24.x = a2->x;
		this->AtelBuffer->field_24.y = a2->y;
		this->AtelBuffer->field_24.z = a2->z;
		this->AtelBuffer->CopyAllSPos();
	}
	this->field_5FC = *a2;
	this->field_5FC.w = 1.0;
}

void FFXI::CYy::CXiSkeletonActor::Set61C(D3DXVECTOR4* a2)
{
	if (this->AtelBuffer) {
		this->AtelBuffer->field_34.x = a2->x;
		this->AtelBuffer->field_34.y = a2->y;
		this->AtelBuffer->field_34.z = a2->z;
		this->AtelBuffer->CopyAllSPos();
	}

	if (this->IsDirectionLock() == false) {
		this->field_61C = *a2;
		Globals::PiClamp(&this->field_61C);
	}
}

void FFXI::CYy::CXiSkeletonActor::SetSomeFlag(char a2)
{
	this->field_840 ^= (this->field_840 ^ ((a2 != 0) << 23)) & 0x800000;
}

void FFXI::CYy::CXiSkeletonActor::SetFile(CYyResourceFile** a2, int a3)
{
	if (a3 != this->field_9EC) return;
	if (a2 == nullptr) return;

	this->CXiActor::VirtActor154(a2);
	this->InitFromFile(a2);
}

void FFXI::CYy::CXiSkeletonActor::InitFromFile(CYyResourceFile** a2)
{
	CMoSk2** skeletonresource{ nullptr };
	CMoCib** cibresource{ nullptr };
	CYyResourceFile* file{ nullptr };
	if (a2)
		file = *a2;
	CYyDb::g_pCYyDb->pCMoResourceMng->FindNextUnder((CMoResource***)&skeletonresource, file, FFXI::Constants::Enums::ResourceType::Sk2, 0);
	if (skeletonresource == nullptr) return;

	CYyModelBase* modelbase = *this->Model.GetBase();
	if (modelbase == nullptr) {
		modelbase = this->Model.CreateBase();
		if (modelbase == nullptr)
			return;
	}

	modelbase->SetSkeleton(skeletonresource);
	CMoResource::DoTheThing(*skeletonresource);

	CYyDb::g_pCYyDb->pCMoResourceMng->FindNextUnder((CMoResource***)&cibresource, *skeletonresource, FFXI::Constants::Enums::ResourceType::Cib, 0);
	if (cibresource && *cibresource) {
		this->CibCollect.scale[0] = (*cibresource)->info.scale[0];
		this->CibCollect.scale[1] = (*cibresource)->info.scale[1];
		this->CibCollect.scale[2] = (*cibresource)->info.scale[2];
		this->CibCollect.scale[3] = (*cibresource)->info.scale[3];
	}

	if (this->VirtActor201() == true || this->VirtActor204() == true) {
		if (this->field_768) {
			//throw "not implemented";
			exit(0x100CDA7E);
		}
	} else if (this->IsFishingRod() == true) {
		if (this->field_768) {
			exit(0x100CD8CF);
		}
	}
	else if (this->VirtActor219() == false) {
		if (this->VirtActor223()) {
			this->field_754 = 0.0;
		}
		else if (this->VirtActor209() == true) {
			exit(0x100CD990);
		}
		else if (this->VirtActor212() == true) {
			exit(0x100CDA61);
		}
		else if (this->field_768[1] != nullptr) {
			exit(0x100CDA27);
		}
	}
	else if (this->field_768[10] != nullptr) {
		exit(0x100CD919);
	}
}

void FFXI::CYy::CXiSkeletonActor::AddOS2s(CMoResource** a2, CYyModelBase* a3)
{
	CMoResource* res{ nullptr };
	if (a2)
		res = *a2;
	//res can be nullptr as per client

	ResourceList reslist{};
	reslist.PrepareFromResource(res, FFXI::Constants::Enums::ResourceType::Os2, 0, -1);
	while (reslist.field_14) {
		CMoResource** prpl = reslist.field_14->PreHeader.PointerToThisInRPL;
		a3->LinkOs2((CMoOs2**)prpl);
		CMoResource::DoTheThing(*prpl);
		reslist.GetNext(false);
	}
	if ((res->Header.Type_Size & 0x7F) == FFXI::Constants::Enums::ResourceType::Rmp)
		;// this->StartGenerators();
}

void FFXI::CYy::CXiSkeletonActor::Negativea2()
{
	int status = this->GetGameStatus();
	int type = this->GetType();
	switch (status) {
	case 1:
		if (type >= 0 && (type <= 1 || type == 6))
			this->field_8A4 = 2;
		break;
	case 2:
		if (type >= 0 && (type <= 1 || type == 6))
			this->field_8A4 = 1;
		this->field_7C8[4] = '    ';
		break;
	case 3:
		if (type >= 0 && (type <= 1 || type == 6)) {
			if (this->Unknown7E4Func(1) == nullptr)
				this->field_8A4 = 2;
		}
		this->field_7C8[4] = '    ';
		break;
	case 8:
	case 9:
	case 26:
	case 27:
	case 29:
	case 30:
	case 34:
	case 35:
	case 36:
	case 37:
	case 45:
	case 46:
		return;
	case 49:
		break;
	default:
		if (type >= 0 && (type <= 1 || type == 6))
			this->field_8A4 = 1;
	}
}

void* FFXI::CYy::CXiSkeletonActor::Unknown7E4Func(int a2)
{
	if (this->field_7E4 == nullptr)
		return nullptr;

	//sub //TODO
	exit(0x100D25AE);
}

void FFXI::CYy::CXiSkeletonActor::Draw()
{
	D3DXVECTOR3 a1{};
	D3DXVECTOR4 v99{}, v101{};
	bool FlagOne{ false }, FlagTwo{ false }, FlagThree{ false }, FlagFour{ false };
	if (this->AtelBuffer != nullptr) {
		FlagOne = (this->AtelBuffer->field_12C >> 17) & 1;
		FlagTwo = this->AtelBuffer->field_130 & 1;

		if ((this->AtelBuffer->field_120 & 0x80) != 0
			|| (this->AtelBuffer->field_120 & 0x100) != 0)
			FlagThree = true;

		if ((this->AtelBuffer->field_12C & 0x8000000) != 0) {
			if (this->CheckSomeFlag1() == true 
				|| this->CheckSomeFlag2() == true)
				FlagFour = true;
		}
	}

	CYyModelBase* modelbase = *this->Model.GetBase();
	if (modelbase == nullptr) {
		this->field_B0 = 1;
		this->field_B2 |= 1;
		this->SomeCameraUpdate();
		return;
	}

	CXiSkeletonActor::somebool = true;
	CMoOs2::DrawBasicFlagOne = false;
	CMoOs2::DrawBasicFlagTwo = false;
	//sub //TODO
	FFXI::CYy::CDx::instance->SetTransform(D3DTS_WORLD, &Globals::g_D3DTransformMatrix);
	Globals::g_TransformBackup = Globals::g_D3DTransformMatrix;

	this->Tick();
	double v11{ 1.0 };
	if (this->field_664 < 0.80000001) {
		v11 = this->field_664 * 1.25;
	}
	//sub //TODO
	this->field_59C = 1.0;	//REMOVE THIS. temp to enter logic below

	v11 *= this->field_59C;
	if (v11 > 1.0)
		v11 = 1.0;

	long long v12 = (long long)(128.0 * v11);
	unsigned int v97 = ((unsigned int)v12 << 24) | this->field_660 & 0xFFFFFF;
	if (v11 >= 0.000099999997) {
		CYyModel::g_some_short = this->VirtActor238();
		int something = this->VirtActor124();
		if (XiZone::zone->ZoneID == FFXI::Constants::Enums::Zone::AlTaieu
			&& (something == 8 || something == 9)) {
			Globals::g_some_actordraw_short = 3;
		}
		else if (this->VirtActor6() == 0) {
			Globals::g_some_actordraw_short = 3;
		}
		else if (Config::MainRegistryConfig::dword104458F4 == 1) {
			Globals::g_some_actordraw_short = 3;
		}
		else {
			D3DXVECTOR4* act = this->VirtActor101();
			D3DXVECTOR3* cam = &CYyDb::g_pCYyDb->CameraManager->Position;
			a1.x = act->x - cam->x;
			a1.y = act->y - cam->y;
			a1.z = act->z - cam->z;
			if (a1.y < -2.0) {
				Globals::g_some_actordraw_short = 3;
			}
			else if (Config::MainRegistryConfig::dword104458F4 == 2 && Globals::g_some_actordraw_float < 40.0) {
				Globals::g_some_actordraw_short = 1;
				Globals::g_some_actordraw_float = 0.0;
			}
			else {
				Globals::g_some_actordraw_float = this->field_BC;
				if (this->AmIControlActor() == false) {
					if (this->Model.field_B4 <= 1) {
						Globals::g_some_actordraw_short = 3;
					}
					else if (this->InOwnActorPointers() == true) {
						if (this->VirtActor201() == true
							|| this->VirtActor204() == true) {
							if (this->field_768[0] != 0
								&& this->AmIControlActor() == true) {
								Globals::g_some_actordraw_short = 1;
								Globals::g_some_actordraw_float = 0.0;
							}
							else {
								Globals::g_some_actordraw_short = this->field_17C != 0 ? 2 : 0;
							}
						}
						else if (this->IsFishingRod() == true) {
							if (this->field_768[0] == 0) {
								Globals::g_some_actordraw_short = 3;
							}
							else if (this->AmIControlActor() == true) {
								Globals::g_some_actordraw_short = 1;
								Globals::g_some_actordraw_float = 0.0;
							}
							else {
								Globals::g_some_actordraw_short = 3;
							}
						}
						else if (this->VirtActor219() == true) {
							Globals::g_some_actordraw_short = 3;
						}
						else if (this->VirtActor209() == true) {
							if (this->field_768[0] == 0) {
								Globals::g_some_actordraw_short = 3;
							}
							else if (this->AmIControlActor() == true) {
								Globals::g_some_actordraw_short = 1;
								Globals::g_some_actordraw_float = 0.0;
							}
							else {
								Globals::g_some_actordraw_short = 3;
							}
						}
						else if (this->VirtActor215() == true
							|| this->VirtActor212() == true) {
							Globals::g_some_actordraw_short = 3;
						}
						else if (this->VirtActor223() == true) {
							Globals::g_some_actordraw_short = 3;
						}
						else {
							Globals::g_some_actordraw_short = this->field_17C != 0 ? 2 : 0;
						}
					}
					else {
						if (this->field_768[0] == 0) {
							Globals::g_some_actordraw_short = this->field_17C != 0 ? 2 : 0;
						}
						else if (this->VirtActor201() == true) {
							Globals::g_some_actordraw_short = 3;
						}
						else if (this->VirtActor204() == true) {
							Globals::g_some_actordraw_short = 3;
						}
						else {
							Globals::g_some_actordraw_short = this->field_17C != 0 ? 2 : 0;
						}
					}
				}
				else {
					if (this->field_768[0] == 0) {
						Globals::g_some_actordraw_short = 1;
						Globals::g_some_actordraw_float = 0.0;
					}
					else if (this->VirtActor201() == true) {
						Globals::g_some_actordraw_short = 3;
					}
					else if (this->VirtActor204() == true) {
						Globals::g_some_actordraw_short = 3;
					}
					else {
						Globals::g_some_actordraw_short = 1;
						Globals::g_some_actordraw_float = 0.0;
					}
				}
			}
		}

		static D3DXVECTOR3 static_vec3_1{};
		static D3DXVECTOR3 static_vec3_2{};
		static D3DXVECTOR3 static_vec3_3{};
		static D3DXVECTOR3 static_vec3_4{};

		if (this->VirtSkeletonActor247() == true) {
			a1 = static_vec3_4;
		}
		else if (CXiSkeletonActor::g_light->Type == D3DLIGHT_POINT) {
			D3DXVECTOR4* act = this->VirtActor101();
			a1.x = CXiSkeletonActor::g_light->Position.x - act->x;
			a1.y = CXiSkeletonActor::g_light->Position.y - act->y;
			a1.z = CXiSkeletonActor::g_light->Position.z - act->z;
		}
		else {
			a1.x = -CXiSkeletonActor::g_light->Direction.x;
			a1.y = -CXiSkeletonActor::g_light->Direction.y;
			a1.z = -CXiSkeletonActor::g_light->Direction.z;
		}

		this->Model.SubStruct1.Init(&a1);
		D3DXVECTOR4* act101 = this->VirtActor101();
		static_vec3_2 = { act101->x, act101->y, act101->z };
		static_vec3_1 = static_vec3_2 - this->Model.SubStruct1.field_14;
		D3DXMatrixLookAtRH(&CYyModel::view_transform, &static_vec3_1, &static_vec3_2, &static_vec3_3);
		D3DXVECTOR3 v107{}, v104{};
		CYyModel::view_transform.Vec3TransformNormal(&v107, &this->field_918);
		v99 = { v107.x, v107.y, v107.z, 0 };
		v101 = v99;
		for (int i = 0; i < sizeof(this->field_928) / sizeof(this->field_928[0]); ++i) {
			CYyModel::view_transform.Vec3TransformNormal(&v104, (D3DXVECTOR3*)(this->field_928 + i));
			if (v99.x >= v104.x)
				v99.x = v104.x;
			if (v99.y >= v104.y)
				v99.y = v104.y;

			if (v101.x <= v104.x)
				v101.x = v104.x;
			if (v101.y <= v104.y)
				v101.y = v104.y;
		}

		double v36 = v101.x - v99.x;
		if (v36 < 0.0)
			v36 = -v36;

		double v37 = v101.y - v99.y;
		if (v37 < 0.0)
			v37 = -v37;

		if (v36 > v37)
			v37 = v36;

		if (Globals::g_some_actordraw_short != 1) {
			v37 *= 0.55555558;
		}

		if (v37 < 0.0099999998)
			v37 = 0.0099999998;

		ModelSubStruct1::g_mss1_float = v37;
		static_vec3_1 = this->Model.SubStruct1.field_14 * v37 * -12.0;
		static_vec3_1 += static_vec3_2;
		D3DXMatrixLookAtRH(&CYyModel::view_transform, &static_vec3_1, &static_vec3_2, &static_vec3_3);

		char v24{ 0 };
		if (this->AmIControlActor() == true)
			goto LABEL_155;
		if (FlagOne == true || FlagTwo == true)
			goto LABEL_98;
		if (this->field_9F8 == 0 && FlagThree == false
			&& FFXI::Config::RegistryConfig::g_pOptions->Other.field_48 == 0) {
			if (CYyDb::g_pCYyDb->field_34 % 4 != this->field_9F0) {
				if (this->field_9F4 != 0)
					goto LABEL_98;
				goto LABEL_104;
			}
		}
		if (this->VirtActor204() == true)
			goto LABEL_98;

		this->field_9F8 = 0;
		if (this->field_900 < 0.0)
			goto LABEL_98;

		v99.w = this->field_90C;
		v99.x = this->field_8F8;
		v99.y = this->field_8FC;
		v99.z = this->field_908;
		{
			double v51 = this->field_910 - this->field_900;
			v51 *= 0.25;
			v51 += this->field_900;

			this->field_9F4 = this->field_8F0.Update(&v99, v51, 
				(float)CYyDb::g_pCYyDb->GetBackgroundXRes(), 
				(float)CYyDb::g_pCYyDb->GetBackgroundYRes(),
				Globals::g_pSomeTexture1,
				Globals::g_pSomeTexture2,
				1, 1, 0);
		}
		if (this->field_9F4 != 0) {
		LABEL_98:
			v24 = this->field_A04 == 0 && FlagThree == false;
			if (this->VirtActor201() == true) {
				if (this->field_768[0] != 0) {
					if (this->AmIControlActor() == true)
						v24 = 0;
				}
			}

			if (this->VirtActor204() == true)
				goto LABEL_155;
			if (this->CheckGameStatus85(this->GetGameStatus()) == true|| v24 == 0)
				goto LABEL_155;
			switch (Config::MainRegistryConfig::dword10445A28) {
			case 1:
				//FALLTHROUGH
			case 3:
				if (CYyDb::g_pCYyDb->field_34 % 4 != this->field_9FC)
					goto LABEL_155;
				break;
			case 2:
				if ((((unsigned char)this->field_9FC ^ (unsigned char)(CYyDb::g_pCYyDb->field_34 % 4)) & 1) == 0)
					goto LABEL_155;
				break;
			default:
				goto LABEL_155;
			}
			if (this->field_A00 != 0) {
				CXiActor::somebool = false;
				this->field_A00 = 0;
				LABEL_156:
				this->BaseDoSomething();
				this->SetDrawBasicFlags();
				float v63 = this->VirtActor58();
				float v66 = v63;
				float v67 = v63;
				if (v63 < 0.0) {
					if (this->field_760 >= 0.0) {
						v63 = this->field_760;
					}
					else {
						v63 = this->field_754;
					}
					if (this->field_75C >= 0.0) {
						v66 = this->field_75C;
					}
					else {
						v66 = this->field_754;
					}
					if (this->field_758 >= 0.0) {
						v67 = this->field_758;
					}
					else {
						v67 = this->field_754;
					}
				}

				D3DXVECTOR4 scalevec{};
				scalevec.x = v67;
				scalevec.y = v66;
				scalevec.z = v63;
				scalevec.z = 1.0;
				this->field_998.CreateScaling(&scalevec);
				if (this->field_5F8 != nullptr) {
					this->field_998 = *this->field_5F8;
				}
				else {
					bool rotateflag = this->CheckSomeFlag3();
					if (rotateflag == false) {
						int gamestatus = this->GetGameStatus();
						int legs4 = this->CibCollect.legs4;
						if (legs4 <= 0 || legs4 == 3) {
							if (gamestatus != 2 && gamestatus != 3)
								rotateflag = true;
						}
					}

					if (rotateflag == true) {
						D3DXVECTOR4* pr = this->VirtActor102();
						D3DXVECTOR4 r = *pr + this->field_62C;
						if (this->SomeEventCheck() == true) {
							this->field_998.RotateX(r.z);
							this->field_998.RotateY(r.y);
							this->field_998.RotateZ(r.x);
						}
						else {
							this->field_998.RotateX(r.x);
							this->field_998.RotateY(r.y);
							this->field_998.RotateZ(r.z);
						}
					}
					else {
						this->SomeMatrixMath(&this->field_998, 0.1875);
					}
					D3DXVECTOR4* a = this->VirtActor101();
					D3DXVECTOR4 b = *a + this->field_60C;
					b.w = 1.0;
					this->field_998.AddTranslation4(&b);
				}

				D3DXVECTOR4 v99 = this->field_848;
				v99.w = modelbase->field_B0.w;
				if (FlagThree == true
					|| this->AmIControlActor() == true) {
					D3DXVECTOR4* v83 = this->VirtActor102();
					D3DXVECTOR4* v77 = this->VirtActor101();
					this->Model.Draw(this, &this->field_998, &scalevec, v77, v83, &v99, v97);
					CXiActor::maybeActorDrawCount += 1;
					this->field_B0 = 0;
					this->SomeCameraUpdate();
					return;
				}
				CXiActor::maybeActorDrawCount += 1;
				if (CXiActor::maybeActorDrawCount >= CXiActor::config60
					&& FlagFour == false) {
					CXiSkeletonActor* v74 = (CXiSkeletonActor*)this->field_768[0];
					if (v74 != nullptr
						&& v74->Model.field_30 == 4) {
						D3DXVECTOR4* v83 = this->VirtActor102();
						D3DXVECTOR4* v77 = this->VirtActor101();
						this->Model.Draw(this, &this->field_998, &scalevec, v77, v83, &v99, v97);
						this->field_B0 = 0;
						this->SomeCameraUpdate();
						return;
					}
					v74 = (CXiSkeletonActor*)this->field_768[1];
					if (v74 == nullptr || v74->Model.field_30 != 4) {
						if (this->Model.field_B4 > 1) {
							this->field_B0 = 0;
							this->SomeCameraUpdate();
							return;
						}
					}
				}
				D3DXVECTOR4* v83 = this->VirtActor102();
				D3DXVECTOR4* v77 = this->VirtActor101();
				this->Model.Draw(this, &this->field_998, &scalevec, v77, v83, &v99, v97);
				this->field_B0 = 0;
				this->SomeCameraUpdate();
				return;
			}

		LABEL_155:
			this->field_A04 = 0;
			CXiActor::somebool = true;
			this->field_A00 = 1;
			goto LABEL_156;
		}
		else {
			LABEL_104:
			this->field_B0 = 1;
			this->field_B2 |= 1;
		}
	}
	
	LABEL_105:
	if (this->field_5F8 != nullptr) {
		this->field_998 = *this->field_5F8;
	}
	else {
		float zval{}, yval{}, xval{};
		float somefloat = this->VirtActor58();
		if (somefloat < 0.0) {
			if (this->field_760 >= 0.0) {
				zval = this->field_760;
			}
			else {
				zval = this->field_754;
			}

			if (this->field_75C >= 0.0) {
				yval = this->field_75C;
			}
			else {
				yval = this->field_754;
			}

			if (this->field_758 >= 0.0) {
				xval = this->field_758;
			}
			else {
				xval = this->field_754;
			}
		}
		else {
			zval = somefloat;
			yval = somefloat;
			xval = somefloat;
		}		

		D3DXVECTOR4 v105 = { xval, yval, zval, 1.0 };
		this->field_998.CreateScaling(&v105);

		bool rotateflag = this->CheckSomeFlag3();
		if (rotateflag == false) {
			int gamestatus = this->GetGameStatus();
			int legs4 = this->CibCollect.legs4;
			if (legs4 <= 0 || legs4 == 3) {
				if (gamestatus != 2 && gamestatus != 3)
					rotateflag = true;
			}
		}

		if (rotateflag == true) {
			D3DXVECTOR4* pr = this->VirtActor102();
			D3DXVECTOR4 r = *pr + this->field_62C;
			this->field_998.RotateX(r.x);
			this->field_998.RotateY(r.y);
			this->field_998.RotateZ(r.z);
		}
		else {
			this->SomeMatrixMath(&this->field_998, 0.1875);
		}
		D3DXVECTOR4* a = this->VirtActor101();
		D3DXVECTOR4 b = *a + this->field_60C;
		b.w = 1.0;
		this->field_998.AddTranslation4(&b);
	}

	if (modelbase->field_B0.w == 0.0
		|| modelbase->field_B0.w == -1.0) {
		this->Model.DoingSomething(this, &this->field_998, this->field_BC, nullptr);
	}
	else {
		D3DXVECTOR4 v99 = this->field_848;
		v99.w = modelbase->field_B0.w;
		this->Model.DoingSomething(this, &this->field_998, this->field_BC, &v99);
	}

	this->SomeCameraUpdate();
}

void FFXI::CYy::CXiSkeletonActor::SomeCameraUpdate()
{
	if (this->VirtActor75() == false)
		return;

	//sub //TODO
	exit(0x100CAEF8);
}

void FFXI::CYy::CXiSkeletonActor::BaseDoSomething()
{
	CYyModelBase* base = *this->Model.GetBase();
	if (base == nullptr)
		return;

	if (this->CheckAtel120Bit5() == true) {
		this->field_848.w = -1.0;
	}
	else {
		CXiActor* v3{ nullptr };
		if ((this->field_840 & 1) != 1) {
			if (this->AmIControlActor() == true)
				;// sub;//inputfunc
			else
				v3 = this->Get154Actor();

			if (v3 != nullptr && v3 != this) {
				int v3type = v3->GetType();
				if (v3type == 0
					|| v3type == 1
					|| v3type == 2
					|| v3type == 6
					|| v3type == 7
					|| v3type == 8) {
					int thisstatus = this->GetGameStatus();
					if (thisstatus == 0
						|| thisstatus == 47
						|| this->TestSomethingIfAtel(thisstatus)
						|| thisstatus == 48
						|| this->CheckGameStatus5(thisstatus)
						|| this->CheckGameStatus85(thisstatus)) {
						D3DXVECTOR4 v26{};
						v3->VirtActor103(3, &v26);
						if (v3->field_B2 != 0)
							v26.y -= 1.2;
						this->field_848.x = v26.x;
						this->field_848.y = v26.y;
						this->field_848.z = v26.z;
						this->field_848.w = 0.0;
					}
				}
			}
			this->field_848.w = -1.0;
		}
		else if ((this->field_840 & 2) != 0) {
			this->field_848.w = -1.0;
		}
		else {
			this->field_848.w = 1.0;
		}
	}
}

void FFXI::CYy::CXiSkeletonActor::SetDrawBasicFlags()
{
	if ((this->field_840 & 0x80000) == 0)
	{
		CYyResourceFile* file{ nullptr };
		if (CYyDb::g_pCYyDb->pCMoResourceMng->Unknown3 != nullptr) {
			file = *CYyDb::g_pCYyDb->pCMoResourceMng->Unknown3;
		}

		this->some_cubemap = CYyDb::pCYyTexMng->FindD3sTexUnder("cubemap spec    ", file);
		this->field_840 |= 0x80000;
	}
	//set static
	if ((this->field_840 & 8) == 0)
	{
		this->ActorFindResource(&this->veve_texinfo, FFXI::Constants::Enums::ResourceType::Texinfo, 'veve');
		this->field_840 |= 8u;
	}
	//set static
	if ((this->field_840 & 0x10) == 0)
	{
		this->ActorFindResource(&this->null_maxresource, FFXI::Constants::Enums::ResourceType::Texinfo, 0);
		this->field_840 |= 0x10u;
		this->Model.SubStruct5.field_4 = this->null_maxresource;
		if (this->AtelBuffer != nullptr)
		{
			if ((this->AtelBuffer->field_120 & 0x80) != 0 || (this->AtelBuffer->field_120 & 0x100) != 0)
				this->field_8EC = 0x40800000;
		}
	}
	if (this->null_maxresource != nullptr && *this->null_maxresource != nullptr && (this->field_840 & 0x20) == 0)
	{
		this->ActorFindResource(&this->ohno_texinfo, FFXI::Constants::Enums::ResourceType::Texinfo, 'onho');
		this->field_840 |= 0x20u;
	}
}

void FFXI::CYy::CXiSkeletonActor::Tick()
{
	double v2 = CYyDb::g_pCYyDb->CheckTick() * 0.40000001;
	if (v2 >= 1.0)
		v2 = 1.0;

	D3DLIGHT8* v88[4] = { nullptr };
	float v89[4] = { 10000.0 };
	int v7 = 0;
	for (int i = 0; i < sizeof(this->field_180) / sizeof(this->field_180[0]); ++i) {
		if (v7 >= 4)
			break;

		D3DLIGHT8* light1 = this->field_180 + i;
		D3DLIGHT8* light2 = this->field_388 + i;

		light1->Type = light2->Type;
		light1->Diffuse = light2->Diffuse;
		light1->Position = light2->Position;
		light1->Range = light2->Range;
		light1->Attenuation2 = light2->Attenuation2;
		D3DXVECTOR4* v10 = this->VirtActor101();
		D3DXVECTOR4 v91{}, v92{};
		v92.x = light1->Position.x - v10->x;
		v92.y = light1->Position.y - v10->y;
		v92.z = light1->Position.z - v10->z;

		v91.x = v92.x * v92.x;
		v91.y = v92.y * v92.y;
		v91.z = v92.z * v92.z;

		long double v11 = v91.z + v91.y + v91.x;
		long double v13 = sqrt(v11);
		if (v13 <= light1->Range) {
			long double v17 = v11 * light1->Attenuation2;
			if (v17 <= 0.000099999997)
				v17 = 0.000099999997;
			long double v19 = 1.0 / v17;
			int j{};
			for (j = 0; j < 4; ++j) {
				if (v88[j] == nullptr)
					break;
				if (v19 > v89[j])
					break;
			}
			if (j != 4) {
				for (int k = 2; k >= j; --k) {
					v88[k + 1] = v88[k];
					v89[k + 1] = v89[k];
				}
				v7 += 1;
				v88[j] = light1;
				v89[j] = v19;
			}
		}
	}
	if (v7 > 1) {
		memset(v89 + 1, 0, 12);
		memset(v88 + 1, 0, 12);
		v7 = 1;
	}

	for (int i = 0; i < sizeof(this->field_180) / sizeof(this->field_180[0]); ++i) {
		if (v7 >= 4)
			break;

		D3DLIGHT8* light1 = this->field_180 + i;
		D3DLIGHT8* light2 = this->field_388 + i;

		light1->Type = light2->Type;
		if (light2->Type == D3DLIGHT_DIRECTIONAL) {
			if (this->field_B0 == 0 || this->field_B2 != 0) {
				light1->Diffuse.r += (light2->Diffuse.r - light1->Diffuse.r) * v2;
				light1->Diffuse.g += (light2->Diffuse.g - light1->Diffuse.g) * v2;
				light1->Diffuse.b += (light2->Diffuse.b - light1->Diffuse.b) * v2;
				light1->Diffuse.a += (light2->Diffuse.a - light1->Diffuse.a) * v2;
			
				light1->Direction.x += (light2->Direction.x - light1->Direction.x) * v2;
				light1->Direction.y += (light2->Direction.y - light1->Direction.y) * v2;
				light1->Direction.z += (light2->Direction.z - light1->Direction.z) * v2;
			}
			else {
				light1->Diffuse = light2->Diffuse;
				light1->Direction = light2->Direction;
			}
			int j{};
			for (j = 0; j < 4; ++j) {
				if (v88[j] == nullptr)
					break;
				if (v89[j] < 0.001)
					break;
			}
			if (j != 4) {
				for (int k = 2; k >= j; --k) {
					v88[k + 1] = v88[k];
					v89[k + 1] = v89[k];
				}
				v7 += 1;
				v88[j] = light1;
				v89[j] = 0.001;
			}
		}
	}

	for (int i = 0; i < 4; ++i) {
		D3DLIGHT8* light = CXiSkeletonActor::g_light_arr + i;
		if (v88[i] != nullptr) {
			*v88[i] = *light;
			if (light->Type == D3DLIGHT_POINT) {
				light->Type = D3DLIGHT_DIRECTIONAL;
				D3DXVECTOR4* ac = this->VirtActor101();
				light->Direction.x = ac->x - light->Position.x;
				light->Direction.y = ac->y - light->Position.y;
				light->Direction.z = ac->z - light->Position.z;
				Globals::Vec3Normalize((D3DXVECTOR3*)&light->Direction);
				light->Diffuse.r *= v89[i];
				light->Diffuse.g *= v89[i];
				light->Diffuse.b *= v89[i];
			}
		}
		else {
			light->Type = (D3DLIGHTTYPE)NULL;
		}
	}

	int v34{};
	for (v34 = 0; v34 < 4; v34++) {
		if (this->field_180 + v34 == v88[0])
			break;
	}
	if (v34 == 4)
		v34 = 0;
	CXiSkeletonActor::g_light = this->field_388 + v34;
	int vf130 = this->VirtActor130();
	bool vf130zero = vf130 == 0;
	int f88 = this->VirtActor88();
	CMoResource** v37 = this->Model.SubStruct5.GetResource();
	if (v37 != nullptr) {
		//sub //TODO
		exit(0x100C998A);
	}
	else {
		XiZone::zone->GetAmbientByFourCC(&ActorAmbientLight, vf130zero, f88);
	}

	D3DCOLOR fogColor{};
	float fogStart{}, fogEnd{};
	XiZone::zone->GetFogByFourCC(&fogColor, &fogEnd, &fogStart, vf130zero, f88);
	Placeholder::g_pTkInputCtrl;
	CXiActor* v42 = nullptr; //sub //TODO //inputctrl func
	if (v42 == nullptr) {
		v42 = CXiActor::TWOtempAttachStruct.GetActor();
	}
	if (v42 == this
		|| (this->field_88 & 0x200) != 0) {
		if ((this->field_840 & 4) == 0) {
			this->field_844 = 40.0;
			this->field_840 |= 0x6000000;
		}
		if ((this->field_840 & 0x6000000) != 0
			|| (this->field_88 & 0x200) != 0) {
			double v50{};
			if (this->field_844 <= 20.0)
				v50 = this->field_844;
			else
				v50 = 40.0 - this->field_844;

			//Adjust ambient light
			float v84 = v50 * 0.050000001;
			float scale = 1.0 - v84;
			float add = v84 * 220.0;
			D3DXVECTOR3 vecambient{};
			vecambient.x = (unsigned char)ActorAmbientLight;
			vecambient.x *= scale;
			vecambient.x += add;

			vecambient.y = (unsigned char)(ActorAmbientLight >> 8);
			vecambient.y *= scale;
			vecambient.y += add;

			vecambient.z = (unsigned char)(ActorAmbientLight >> 16);
			vecambient.z *= scale;
			vecambient.z += add;

			//Lower bound check
			if (vecambient.x < 0.0)
				vecambient.x = 0;
			if (vecambient.y < 0.0)
				vecambient.y = 0;
			if (vecambient.z < 0.0)
				vecambient.z = 0;

			//Upper bound check
			if (vecambient.x > 255.0)
				vecambient.x = 255.0;
			if (vecambient.y > 255.0)
				vecambient.y = 255.0;
			if (vecambient.z > 255.0)
				vecambient.z = 255.0;

			ActorAmbientLight = (int)vecambient.z;
			ActorAmbientLight <<= 8;
			ActorAmbientLight |= (int)vecambient.y;
			ActorAmbientLight <<= 8;
			ActorAmbientLight |= (int)vecambient.x;

			//Adjust Fog Color
			D3DXVECTOR3 fog{};
			fog.x = (unsigned char)fogColor;
			fog.x *= v84;

			fog.y = (unsigned char)(fogColor >> 8);
			fog.y *= v84;

			fog.z = (unsigned char)(fogColor >> 16);
			fog.z *= v84;

			//Lower bound check
			if (fog.x < 0.0)
				fog.x = 0;
			if (fog.y < 0.0)
				fog.y = 0;
			if (fog.z < 0.0)
				fog.z = 0;

			//Upper bound check
			if (fog.x > 255.0)
				fog.x = 255.0;
			if (fog.y > 255.0)
				fog.y = 255.0;
			if (fog.z > 255.0)
				fog.z = 255.0;

			fogColor = (int)fog.z;
			fogColor <<= 8;
			fogColor |= (int)fog.y;
			fogColor <<= 8;
			fogColor |= (int)fog.x;

			this->field_844 -= CYyDb::g_pCYyDb->CheckTick();
			if (this->field_844 < 0.0) {
				this->field_844 = 40.0;
				int v76 = this->field_840;
				this->field_840 = v76 ^ (v76 ^ ((v76 & 0xFE000000) - 1)) & 0x6000000;
				if ((this->field_840 & 0x6000000) == 0) {
					this->field_88 &= 0xFFFFFDFF;
				}
			}
		}
		this->field_840 |= 4;
	}
	else {
		this->field_840 &= 0xFFFFFFFB;
	}

	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGCOLOR, fogColor);
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)(&fogStart)));
	FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)(&fogEnd)));

	CMoResource** something = this->Model.SubStruct5.GetResource();
	if (something == nullptr)
		return;

	exit(0x83380);
	//is there something else here?
}

void FFXI::CYy::CXiSkeletonActor::SomeMatrixMath(FFXI::Math::WMatrix* a2, float a3) {
	FFXI::Math::WMatrix v33{};
	D3DXVECTOR4 v28 = { 0.0, 0.0, 1.0, 1.0 };
	D3DXVECTOR4 v31 = { 0.0, -1.0, 0.0, 1.0 };
	D3DXVECTOR4* v5 = this->VirtActor122();
	D3DXVECTOR4 v34{};
	D3DXVECTOR3 v27{}, v30{}, v32{}, v36{};
	double magsq = v31.x * v5->x + v31.y * v5->y + v31.z * v5->z;
	if (magsq >= 0.86602539) {
		v34.x = v5->x;
		v34.y = v5->y;
		v34.z = v5->z;
	}
	else {
		FFXI::Math::KzFQuat v35{};
		v35.DoTheThing((D3DXVECTOR3*)&v31, (D3DXVECTOR3*)v5, 0.52359879);
		v35.GetMatrix(&v33);
		v33.Vec3TransformDrop4((D3DXVECTOR3*)&v34, (D3DXVECTOR3*)&v31);
		Globals::Vec3Normalize((D3DXVECTOR3*)&v34);
	}
	float v25 = 1.0 - this->field_828;
	v30.x = v34.x;
	v30.y = v34.y;
	v30.z = v34.z;
	
	v30.x = v30.x * this->field_828 + v25 * v31.x;
	v30.y = v30.y * this->field_828 + v25 * v31.y;
	v30.z = v30.z * this->field_828 + v25 * v31.z;
	Globals::Vec3Normalize(&v30);

	float v26 = 1.0 - a3;
	v27 = this->field_82C;
	v27.x = v27.x * v26 + v30.x * a3;
	v27.y = v27.y * v26 + v30.y * a3;
	v27.z = v27.z * v26 + v30.z * a3;
	Globals::Vec3Normalize(&v27);
	this->field_82C = v27;
	v27.x *= -1;
	v27.y *= -1;
	v27.z *= -1;

	D3DXVECTOR4* v8 = this->VirtActor102();
	v33.Identity();
	v33.RotateX(v8->x);
	v33.RotateY(v8->y);
	v33.RotateZ(v8->z);

	v33.Vec3TransformDrop4Self((D3DXVECTOR3*) &v28);
	Globals::Vec3Normalize((D3DXVECTOR3*) &v28);
	Globals::Vec3Outer(&v32, &v27, (D3DXVECTOR3*)&v28);
	Globals::Vec3Normalize(&v32);
	Globals::Vec3Outer((D3DXVECTOR3*)&v28, &v32, &v27);
	Globals::Vec3Normalize((D3DXVECTOR3*)&v28);

	v33._11 = v32.x;
	v33._12 = v32.y;
	v33._13 = v32.z;
	v33._14 = 0.0;

	v33._21 = v27.x;
	v33._22 = v27.y;
	v33._23 = v27.z;
	v33._24 = 0.0;

	v33._31 = v28.x;
	v33._32 = v28.y;
	v33._33 = v28.z;
	v33._34 = 0.0;

	v33._41 = 0.0;
	v33._42 = 0.0;
	v33._43 = 1.0;
	v33._44 = 0.0;
	a2->MatrixMultiply(&v33);

	Globals::Vec3Outer(&v32, (D3DXVECTOR3*) & v31, (D3DXVECTOR3*)&v34);
	Globals::Vec3Normalize(&v32);
	Globals::Vec3Outer(&v36, (D3DXVECTOR3*)&v34, &v32);
	Globals::Vec3Normalize(&v36);

	D3DXVECTOR3 v29{};
	float* va232 = this->VirtActor232(false);
	v29.x = va232[4] - va232[5];
	v29.y = 0.0;
	v29.z = va232[2] - va232[3];

	double v17 = v29.z / v29.x;
	if (v17 < 1.4) {
		if (v17 > 0.60000002) {
			this->field_828 = 1.0;
			return;
		}
	}

	if (v17 >= 1.0) {
		v29 = { 1.0, 0.0, 0.0 };
	}
	else {
		v29 = { 0.0, 0.0, 1.0 };
	}

	a2->Vec3TransformDrop4Self(&v29);
	this->field_828 = Globals::Vec3Inner(&v36, &v29);
	if (this->field_828 < 0)
		this->field_828 *= -1;

}

void FFXI::CYy::CXiSkeletonActor::ReadMotionPackPolling()
{
	int actortype = this->GetType();
	if (actortype == 0 || actortype == 1 || actortype == 6) {
		if ((this->field_8B0 & 0x1FF) != 0x1FF)
			return;

		switch (this->field_8A4) {
		case 1:
			this->ReadStdMotionRes();
			this->field_8A4 = 0;
			break;
		case 2:
			//sub //TODO
			this->field_8A4 = 0;
			break;
		case 3:
			//sub //TODO
			this->field_8A4 = 0;
			break;
		case 4:
			//sub //TODO
			this->field_8A4 = 0;
			break;
		case 5:
			//sub //TODO
			this->field_8A4 = 0;
			break;
		default:
			break;
		}
	}

	if (this->field_8A4 == 3) {
		if (actortype == 2 || actortype == 7) {
			exit(0x100CF1A3);
			this->field_8A4 = 0;
		}
	}
	
	if (this->IsReadCompleteResList() == true) {
		if (this->field_8C4 != 0) {
			exit(0x100CF1A4);
		}
	}
}

void FFXI::CYy::CXiSkeletonActor::ReadStdMotionRes()
{
	if (this->GetType() == 2)
		return;

	int race = this->GetRace();
	if (race >= 9)
		race = 1;

	unsigned int waistDatIndex = this->GetWaistDatIndex(race, 0, 0);
	if (waistDatIndex > 0) {
		if (this->FindResfList(waistDatIndex) == nullptr) {
			CYyResourceFile** wres = CYyDb::pCMoResourceMng->GetOrLoadDatByIndex(waistDatIndex);
			if (wres != nullptr) {
				this->AppendNewResp(wres, 2, waistDatIndex, 5);
				if (CMoResourceMng::CheckResourceThing((CMoResource***) &wres) == true) {
					this->AddMotRes((CMoResource**)wres, this->field_9EC);
				}
				else {
					File::MotionReferenceRead* motread = new File::MotionReferenceRead();
					if (motread != nullptr) {
						if (this == nullptr) {
							motread->ActorIndex = CXiSkeletonActor::skeletonActorIndex;
						}
						else {
							motread->ActorIndex = this->field_9EC;
						}
						motread->field_4.DoSomething(this);
						motread->field_10 = 2;
					}
					CYyResourceFile::ReferenceRead(&wres, File::MotionReferenceRead::ReadCallback, motread, this, nullptr);
				}
			}
		}
	}

	unsigned int upperBodyDatIndex = this->GetUpperBodyDatIndex(race, 0);
	if (upperBodyDatIndex >= 0) {
		if (this->FindResfList(upperBodyDatIndex) == nullptr) {
			CYyResourceFile** ubres = CYyDb::pCMoResourceMng->GetOrLoadDatByIndex(upperBodyDatIndex);
			if (ubres != nullptr) {
				this->AppendNewResp(ubres, 1, upperBodyDatIndex, 0);
				if (CMoResourceMng::CheckResourceThing((CMoResource***)&ubres) == true) {
					this->AddMotRes((CMoResource**)ubres, this->field_9EC);
				}
				else {
					File::MotionReferenceRead* motread = new File::MotionReferenceRead();
					if (motread != nullptr) {
						if (this == nullptr) {
							motread->ActorIndex = CXiSkeletonActor::skeletonActorIndex;
						}
						else {
							motread->ActorIndex = this->field_9EC;
						}
						motread->field_4.DoSomething(this);
						motread->field_10 = 1;
					}
					CYyResourceFile::ReferenceRead(&ubres, File::MotionReferenceRead::ReadCallback, motread, this, nullptr);
				}
			}
		}
	}

	if (this->CheckGameStatus5(255) == true
		|| this->CheckGameStatus85(255) == true) {
		//sub //TODO
		exit(0x100CF712);
	}
}

float FFXI::CYy::CXiSkeletonActor::GetAttackReach()
{
	D3DXVECTOR4 v2{};
	this->GetElemLocal(25, &v2);
	return sqrt(Globals::Vec3Inner(&v2, &v2));
}

unsigned int FFXI::CYy::CXiSkeletonActor::GetWaistDatIndex(int a2, int a3, int a4)
{
	int race = a2;
	if (race >= 9)
		race = 1;

	int waist_type = this->CibCollect.waist_type;
	if (waist_type < 1)
		waist_type = 1;

	if (a3 == 1) {
		int offset = this->CibCollect.amot_wep0;
		if (a4 == 2) {
			offset = this->CibCollect.field_12;
		}

		if (offset < 0) {
			return -1;
		}

		int index = waist_type - 1 + 2 * race;
		if (a4 == 1) {
			return FFXI::Constants::DatIndices::mot_waist_type1[index] + offset;
		}
		else if (a4 == 2) {
			return FFXI::Constants::DatIndices::mot_waist_type2[index] + offset;
		}
		else {
			return FFXI::Constants::DatIndices::mot_waist_typeX[index] + offset;
		}
	}
	else if (waist_type <= 1) {
		if (a3 != 0) {
			return 0;
		}
	}

	return FFXI::Constants::DatIndices::base_skeleton_no_tab[race] + waist_type + 2;
}

unsigned int FFXI::CYy::CXiSkeletonActor::GetUpperBodyDatIndex(int a2, int a3)
{
	int race = a2;
	if (race >= 9)
		race = 1;

	if (this->GetGameStatus() != 1) {
		return FFXI::Constants::DatIndices::base_skeleton_no_tab[race] + this->CibCollect.is_shield + 1;
	}

	int offset = this->CibCollect.amot_wep0;
	if (a3 == 2 ) {
		offset = this->CibCollect.field_12;
	}

	if (offset < 0)
		return -1;

	if (a3 == 1) {
		return FFXI::Constants::DatIndices::mot_upperbody_type1[race] + offset;
	}
	else if (a3 == 2) {
		return FFXI::Constants::DatIndices::mot_upperbody_type2[race] + offset;
	}
	else {
		return FFXI::Constants::DatIndices::mot_upperbody_typeX[race] + offset;
	}
}

bool FFXI::CYy::CXiSkeletonActor::IsReadCompleteResList()
{
	if (this->field_8A4 != 0)
		return false;

	CYyResfList* resf = (CYyResfList*)this->field_7E4->GetNextFreeSpot();
	while (resf != nullptr) {
		if (resf->field_1C == nullptr
			|| CYy::CMoResourceMng::CheckResourceThing((CYy::CMoResource***) & resf->field_1C) == false) {
			return false;
		}
		resf = (CYyResfList*)resf->field_8;
	}

	CYyResourceFile** v5{};
	this->VirtActor109(&v5);
	if (v5 == nullptr)
		return false;

	if (CYy::CMoResourceMng::CheckResourceThing((CYy::CMoResource***) &v5) == false)
		return false;

	this->VirtSkeletonActor246(&v5);
	if (v5 != nullptr) {
		if (CYy::CMoResourceMng::CheckResourceThing((CYy::CMoResource***) &v5) == false)
			return false;
	}
	else if (this->GetType() == 7) {
		if (this->GetEquipNum(0) != 0)
			return false;
	}
	return true;
}

void FFXI::CYy::CXiSkeletonActor::AddMotRes(CMoResource** a2, int a3)
{
	if (this->field_9EC != a3)
		return;

	if (((*a2)->Header.Type_Size & 0x7F) == FFXI::Constants::Enums::ResourceType::Rmp) {
		;// this->StartGeneraters();
		//sub //TODO
	}
	
	CYyResfList* resf = this->field_7E4;
	while (resf != nullptr) {
		if (resf->field_1C != nullptr) {
			if (CYy::CMoResourceMng::CheckResourceThing((CYy::CMoResource***) &resf->field_1C) == true) {
				if (resf->field_20 == false) {
					resf->field_20 = true;
					CYy::CMoResource::DoTheThing(*resf->field_1C);
				}
			}
		}
		resf = (CYyResfList*)resf->field_4;
	}
}

void FFXI::CYy::CXiSkeletonActor::SelectMotionResId(float a2, int* a3, float* a4)
{
	*a4 = 1.0;
	*a3 = this->field_7C8[0];
	switch (this->GetGameStatus()) {
	case 1:
		*a3 = this->field_7C8[3];
		break;
	case 2:
	case 3:
		if (CYyDb::g_pCYyDb->field_9 != 0
			&& this->IsMotionLock() == false) {
			*a3 = this->field_7C8[4];
			return;
		}
		*a3 = ' roc';
		return;
	case 6:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 50:
	case 51:
	case 52:
	case 53:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
	case 61:
	case 62:
		if (CYyDb::g_pCYyDb->field_9 != 0) {
			*a3 = this->field_7C8[4];
		}
		else {
			*a3 = ' 1hf';
		}
		break;
	case 7:
	case 33:
		if (CYyDb::g_pCYyDb->field_9 != 0
			&& this->IsMotionLock() == false) {
			*a3 = this->field_7C8[4];
			return;
		}
		*a3 = ' 1xr';
		return;
	case 47:
		if (CYyDb::g_pCYyDb->field_9 != 0
			&& this->IsMotionLock() == false) {
			*a3 = this->field_7C8[4];
			return;
		}
		*a3 = ' 1is';
		return;
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
		if (CYyDb::g_pCYyDb->field_9 != 0
			&& this->IsMotionLock() == false) {
			*a3 = this->field_7C8[4];
			return;
		}
		*a3 = ' 1ic';
		return;
	default:
		break;
	}
	
	double v7 = a2;
	if (v7 > 0.33333334) {
		*a4 = a2 * 3.0;
		if (*a4 <= 1.5) {
			*a3 = this->field_7C8[1];
		}
		else {
			*a4 = a2;
			*a3 = this->field_7C8[2];
		}
	}
	else if (v7 > 0.000099999997) {
		*a4 = a2 * 3.0;
		*a3 = this->field_7C8[1];
	}

	if (this->VirtActor198() != 0
		|| this->VirtActor194() == 0) {
		if (this->GetType() == 2) {
			if (this->field_598 >= 2 && this->field_598 <= 4)
				*a3 = this->field_7C8[1];
		}
		else if (this->CheckGameStatus5(255) == true
			|| this->CheckGameStatus85(255) == true) {
			if (this->field_598 == 2
				|| this->field_598 == 4) {
				*a3 = ' klw';
			}
			else if (this->field_598 == 3) {
				*a3 = ' bvm';
				}
		}
		else if (this->field_598 == 2) {
			*a3 = ' rvm';
		}
		else if (this->field_598 == 3) {
			*a3 = ' bvm';
		}
		else if (this->field_598 == 4) {
			*a3 = ' lvm';
		}
	}

	if (this->field_8BC >= 0.0) {
		*a3 = this->GetGameStatus() != 1 ? ' pmj' : '*pmj';
		*a4 = 1.0;
	}
	*a4 *= this->field_7E0;
	if (this->GetType() == 2 || this->GetType() == 7) {
		if (this->VirtActor94() == 0.0) {
			*a4 = 1.0;
		}
	}
}

CYyResfList* FFXI::CYy::CXiSkeletonActor::FindResfList(int a2)
{
	CYyResfList* list = this->field_7E4;
	while (list != nullptr) {
		if (list->field_14 == a2)
			break;
		list = (CYyResfList*)list->field_4;
	}
	return list;
}

void FFXI::CYy::CXiSkeletonActor::AppendNewResp(CYyResourceFile** a2, int a3, int a4, int a5)
{
	char* mem = StorageLists::instance->Get(sizeof(CYyResfList), FFXI::Constants::Enums::MEM_MODE::Ex);
	if (mem == nullptr)
		return;

	CYyResfList* newlist = new (mem) CYyResfList();
	if (newlist == nullptr)
		return;

	newlist->field_10 = a3;
	newlist->field_14 = a4;
	newlist->field_1C = a2;
	newlist->field_C = a5;

	newlist->Append((CYyNode**) & this->field_7E4);
}

void FFXI::CYy::CXiSkeletonActor::DefaultMove(D3DXVECTOR3* a2)
{
	bool v3 = this->InOwnActorPointers();
	float MoveVectorRatio = this->GetMoveVectorRatio(a2);
	int v35{};
	float v34{};
	this->SelectMotionResId(MoveVectorRatio, &v35, &v34);
	int v35backup = v35;
	if (this->field_768[0] != nullptr) {
		if (this->CheckGameStatus85(255) == true) {
			int something = ((CXiSkeletonActor*)this->field_768[0])->CibCollect.GetSomething(this->GetRace());
			v35 ^= (v35 ^ (something + '0'));
		}
	}

	if (this->field_80 != 0)
		return;

	if (this->IsMotionLock() == false
		&& this->field_7C8[4] == v35) {
		if (this->field_7DC != v34) {
			CYyModelBase* base = *this->Model.GetBase();
			if (base != nullptr) {
				for (int i = 0; i < sizeof(base->field_24.motions) / sizeof(base->field_24.motions[0]); ++i) {
					CYyMotionQue* tail = base->field_24.motions[i].GetTail();
					if (tail != nullptr) {
						tail->SetSpeed(v34);
					}
				}
			}

			this->field_7DC = v34;

			CXiSkeletonActor* first = (CXiSkeletonActor*)this->field_768[0];
			if (first != nullptr) {
				if (first->VirtActor201() == true
					|| first->IsFishingRod() == true
					|| first->VirtActor209() == true) {
					CYyModelBase* base = *this->Model.GetBase();
					CYyModelBase* base2 = *first->Model.GetBase();
					if (base != nullptr
						&& base2 != nullptr) {
						for (int i = 0; i < sizeof(base->field_24.motions) / sizeof(base->field_24.motions[0]); ++i) {
							CYyMotionQue* tail = base->field_24.motions[i].GetTail();
							CYyMotionQue* tail2 = base2->field_24.motions[i].GetTail();
							if (tail != nullptr
								&& tail2 != nullptr) {
								float somefloat = tail->GetFrame();
								tail2->Frame = somefloat;
								tail2->SetSpeed(v34);
							}
						}
					}
				}
				else if (this->VirtActor204() == true) {
					double somedouble = first->CibCollect.GetSomeDouble();
					CYyModelBase* base = *first->Model.GetBase();
					if (base != nullptr) {
						for (int i = 0; i < sizeof(base->field_24.motions) / sizeof(base->field_24.motions[0]); ++i) {
							CYyMotionQue* tail = base->field_24.motions[i].GetTail();
							if (tail != nullptr) {
								tail->SetSpeed(v34 * somedouble);
							}
						}
					}
				}
			}
		}
	}
	else {
		float v41 = 16.0;
		bool v24 = false;
		if (this->IsMotionLock() == true) {
			v41 = this->GetRetIdleFrameTailQue();
			v24 = true;
		}

		if ((v35 & 0xFFFFFF) == 'pmj') {
			v24 = false;
		}

		if (v3 == false && this->DefSchedularCall() == true) {
			return;
		}

		if (this->IsReadCompleteResList() == false)
			return;

		if ((this->field_840 & 0x800000) == 0x800000)
			return;

		if (this->SetSystemMotion(v35, 1.0, v34, 0, v41, 1.0, 0, v24, 0, 0, 0) == true) {
			if (v35 == ' 1di') {
				if (this->SetSystemMotion(' ldi', 1.0, v34, 0, v41, 1.0, 0, v24, 0, 0, 0) == false) {
					this->field_7C8[4] = v35;
					this->field_7DC = v34;
				}
			}
			else if (v35 == ' 1fd') {
				if (this->SetSystemMotion('0tfd', 1.0, v34, 0, v41, 1.0, 0, v24, 0, 0, 0) == false) {
					this->field_7C8[4] = v35;
					this->field_7DC = v34;
				}
			}
		}
		else {
			this->field_7C8[4] = v35;
			this->field_7DC = v34;
			if (v3 == false) {
				bool v33{ false }, v42{ false };
				if (this->AtelBuffer != nullptr) {
					if ((this->AtelBuffer->field_12C & 0x400000) != 0)
						v33 = true;
					if ((this->AtelBuffer->field_12C & 0x400000) != 0)
						v42 = true;
				}

				if (this->CheckGameStatus85(255) == false) {
					if (this->TestSomethingIfAtel(255)) {
						this->HideAllWeapons(0);
					}
					else {
						int type = this->GetType();
						if (type == 0
							|| type == 6
							|| (type == 2 || type == 7 || type == 1) && this->GetMonsterFlag() == true
							|| v42 == true && type == 1
							|| (this->field_840 & 0x400000) != 0
							|| v33 == true) {
							this->ShowWeapon(0, 0);
							this->ShowWeapon(1, 0);
							this->HideWeapon(2, 0);
						}
					}
				}
			}
		}

		CXiSkeletonActor* first = (CXiSkeletonActor*)this->field_768[0];
		if (first != nullptr) {
			if (first->VirtActor201() == true
				|| first->VirtActor204() == true
				|| first->IsFishingRod() == true) {
				float v43 = v34;
				if (first->VirtActor204() == true)
					v43 *= first->CibCollect.GetSomeDouble();

				if (first->SetSystemMotion(v35backup, 1.0, v43, 0, v24, 1.0, 0, v24, 0, 0, 0) == true)
					this->field_7C8[4] = '    ';
			}
		}
	}

}

float FFXI::CYy::CXiSkeletonActor::GetMoveVectorRatio(D3DXVECTOR3* a2)
{
	int actorstatus = this->GetGameStatus();
	double v10{};
	if (this->AmIControlActor() == false
		|| CXiControlActor::is_auto_running == true
		|| this->GetMoveMode() == 1
		|| this->field_101 != 0
		|| actorstatus == 4) {
		float v13 = a2->x * a2->x + a2->z * a2->z;
		if (this->VirtActor93() == true) {
			v13 += a2->y * a2->y;
		}
		v13 = sqrt(v13);
		float v32 = 0.0;
		if (v13 >= 0.000099999997) {
			double v16 = CYyDb::g_pCYyDb->CheckTick();
			v16 *= this->VirtActor96();
			if (this->CheckGameStatus5(actorstatus) == true
				|| this->CheckGameStatus85(actorstatus) == true) {
				v16 *= 0.026666667;
			}
			else {
				v16 *= 0.016666668;
			}

			double v17{ 0.0 };
			if (v16 >= 0.000001) {
				double v18 = 0.0;
				if (v18 > 0.0)
					v17 = v13 / v16;
			}

			v10 = this->field_824 * 0.75 + v17 * 0.25;
		}
	}
	else {
		v10 = this->field_594;
		if (v10 <= 0.0 && this->field_F8 != 0) {
			v10 = 0.33333334;
		}

		double v3 = this->VirtActor94();
		if (v3 > 0.0) {
			v10 = v3 / this->VirtActor96();
		}

		if (this->CheckGameStatus5(actorstatus) == true
			|| this->CheckGameStatus85(actorstatus) == true)
			v10 *= 1.25;
	}

	if (this->VirtActor73() == true) {
		if (v10 > 1.0) {
			v10 = 1.0;
		}
	}
	else {
		if (v10 > 3.0) {
			v10 = 3.0;
		}
	}
	this->field_824 = v10;
	return v10;
}

double FFXI::CYy::CXiSkeletonActor::GetRetIdleFrameTailQue()
{
	CYyModelBase* base = *this->Model.GetBase();
	if (base == nullptr)
		return 1.0;

	CYyMotionQue* tailzero = base->field_24.motions[0].GetTail();
CYyMotionQue* tailone = base->field_24.motions[1].GetTail();

if (tailzero != nullptr) {
	if (tailone == nullptr) {
		return tailzero->field_20;
	}

	if (tailzero->field_20 <= tailone->field_20) {
		return tailone->field_20;
	}
}
else if (tailone != nullptr) {
	return tailone->field_20;
}

return 4.0;
}

void FFXI::CYy::CXiSkeletonActor::ShowWeapon(int a2, int a3)
{
	CYyModelBase* base = *this->Model.GetBase();
	while (base != nullptr) {
		int index = 0;
		CYyModelDt* dt = *base->GetModelDt();
		while (dt != nullptr) {
			if (this->Model.GetOs2ResId(index) == (((a2 + 48) << 24) | 'pew')) {
				this->Model.IsHideOs2(index, 0, a3);
			}
			index += 1;
			dt = dt->field_4;
		}
		base = base->Previous;
	}

	bool v10{ false };
	if (this->AtelBuffer != nullptr) {
		v10 = (this->AtelBuffer->field_128 & 0x40) != 0;
	}

	if (this->GetGameStatus() != 1 || v10 == true) {
		this->MaybeHideWepScheduler(a2);
	}
	else {
		this->MaybeShowWepScheduler(a2);
	}
}

void FFXI::CYy::CXiSkeletonActor::ShowAllWeapons(int a2)
{
	for (int i = 0; i < 9; ++i) {
		this->ShowWeapon(i, a2);
	}
}

void FFXI::CYy::CXiSkeletonActor::HideWeapon(int a2, int a3)
{
	CYyModelBase* base = *this->Model.GetBase();
	while (base != nullptr) {
		int index = 0;
		CYyModelDt* dt = *base->GetModelDt();
		while (dt != nullptr) {
			if (this->Model.GetOs2ResId(index) == (((a2 + 48) << 24) | 'pew')) {
				this->Model.IsHideOs2(index, 1, a3);
			}
			index += 1;
			dt = dt->field_4;
		}
		base = base->Previous;
	}

	this->MaybeHideWepScheduler(a2);
}

void FFXI::CYy::CXiSkeletonActor::HideAllWeapons(int a2)
{
	for (int i = 0; i < 9; ++i) {
		this->HideWeapon(i, a2);
	}
}

bool FFXI::CYy::CXiSkeletonActor::SetSystemMotion(int a2, float a3, float a4, int a5, float a6, float a7, int a8, int a9, int a10, int a11, CYyResourceFile** a12)
{
	is_motion_append = 0;

	CYyModelBase* base = *this->Model.GetBase();
	int v35{ 0 };
	if (base != nullptr) {
		for (int i = 0; i < 3; ++i) {
			if (base->field_24.motions[i].IsExistZombiQue() == true) {
				v35 |= 1 << i;
			}
		}
	}

	bool v18{ false }, v34{ false };
	int actortype = this->GetType();
	if (actortype == 6 || actortype == 7)
		v34 = true;

	if ( (char)this->field_88 < 0
		|| v34 == true && (a2 & 0xFF000000) == '?\0\0\0') {
		if (a12 != nullptr) {
			v18 = this->SetMotion(a12, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
		}
		else {
			CYyResourceFile** pfile{ nullptr };
			this->VirtSkeletonActor246(&pfile);
			if (pfile != nullptr) {
				if (CMoResourceMng::CheckResourceThing((CMoResource***) &pfile) == true) {
					v18 = this->SetMotion(pfile, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
				}
			}
		}
	}

	bool v36{ false };
	if (v18 == false) {
		CYyResfList* nextfree = (CYyResfList*)this->field_7E4->GetNextFreeSpot();
		while (nextfree != nullptr) {
			if (CMoResourceMng::CheckResourceThing((CMoResource***)&nextfree->field_1C) == false) {
				v36 = true;
				break;
			}
			this->SetMotion(nextfree->field_1C, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
			nextfree = (CYyResfList*)nextfree->field_8;
		}

		if (v36 == false) {
			if (this->AtelBuffer != nullptr) {
				switch (this->GetType()) {
				case 0:
				case 1:
				case 6:
				case 8:
					if (this->GetRace() < 29
						&& this->field_7E4 == 0
						&& this->VirtActor201() == false
						&& this->VirtActor204() == false
						&& this->IsFishingRod() == false
						&& this->VirtActor209() == false
						&& this->VirtActor212() == false
						&& this->VirtActor215() == false
						&& this->VirtActor219() == false
						&& this->VirtActor223() == false)
					{
						break;
					}
					//FALLTHROUGH
				case 2:
				case 7: {
					CYyResourceFile** pfile{ nullptr };
					this->VirtSkeletonActor246(&pfile);
					if (pfile == nullptr
						|| CMoResourceMng::CheckResourceThing((CMoResource***)&pfile) == false
						|| this->SetMotion(pfile, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) == false) {
						this->VirtActor109(&pfile);
						if (pfile != nullptr) {
							if (CMoResourceMng::CheckResourceThing((CMoResource***)&pfile) == true) {
								this->SetMotion(pfile, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
							}
						}
					}
				}
					  break;
				default:
					break;
				}
			}
			else {
				CYyResourceFile** pfile{ nullptr };
				this->VirtActor109(&pfile);
				if (pfile != nullptr) {
					if (CMoResourceMng::CheckResourceThing((CMoResource***)&pfile) == true) {
						this->SetMotion(pfile, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
					}
				}
			}
		}
	}

	if (v36 == true)
		return true;

	base = *this->Model.GetBase();
	if (base != nullptr) {
		if ((is_motion_append & 1) != 0) {
			int v30 = v35 ^ is_motion_append;
			for (int i = 0; i < 3; ++i) {
				int a = 1 << i;
				if ((a & v30) != 0 && (a & v35) != 0) {
					base->field_24.motions[i].DeleteAll();
				}
			}
		}
	}

	return false;
}

bool FFXI::CYy::CXiSkeletonActor::SetMotion(CYyResourceFile** a2, int a3, float a4, float a5, int a6, float a7, float a8, int a9, int a10, int a11, int a12)
{
	ResourceList v38{};
	CMoResource** v15 = (CMoResource**)a2;
	int actortype = this->GetType();
	bool returnValue{ false };
	while (true) {
		v38.PrepareFromResource(*v15, FFXI::Constants::Enums::ResourceType::Mo2, a3 & 0xFFFFFF, 0xFFFFFF);
		while (v38.field_14 != nullptr) {
			CMoMo2** rpl = (CMoMo2**)v38.field_14->PreHeader.PointerToThisInRPL;
			CMoMo2* mo2 = *rpl;

			int v12 = a9;
			if (v12 < 5) {
				char c1[2] = { (mo2->Header.FourCC >> 24) & 0xFF, 0 };
				v12 = atoi(c1);
			}

			if (v12 < 7) {
				if (v12 != 2
					|| this->CibCollect.waist_type != 0
					|| actortype != 0 && actortype != 1 && actortype != 6) {
					if (a3 != '*pmj' || v12 != 1) {
						if (((1 << v12) & is_motion_append) == 0) {
							CYyModelBase* base = *this->Model.GetBase();
							if (base != nullptr) {
								if (a10 != 0) {
									base->field_24.AppendSync(this, rpl, a4, a5, a6, a7, a8, v12, a11, a12);
								}
								else {
									base->field_24.Append(this, rpl, a4, a5, a6, a7, a8, v12, a11, a12);
								}
							}
							is_motion_append |= 1 << v12;
							returnValue = true;
						}
					}
				}
			}
			else {
				//Wrong layer designation
			}
			v38.GetNext(false);
		}
		CMoResource* head = (*a2)->GetHead();
		if (head == nullptr || head == *a2) {
			break;
		}
		v15 = head->PreHeader.PointerToThisInRPL;
		CMoResource::SomeCounter -= 1;
	}

	float v42 = a5 * 0.40000001;
	if (a3 == '*pmj') {
		exit(0x100CB7F2);
	}

	return returnValue;
}

void FFXI::CYy::CXiSkeletonActor::MaybeHideWepScheduler(int a2)
{
	if (((1 << a2) & (this->field_840 >> 8) & 0x3FF) != 0) {
		return;
	}

	unsigned int v3{};
	if (a2 == 0) {
		v3 = '20w!';
	}
	else if (a2 == 1) {
		v3 = '21w!';
	}
	else
		return;

	if (this->StartScheduler(v3, nullptr, nullptr) != 0)
		this->field_840 |= ((1 << a2) & 0x3FF) << 8;
}

void FFXI::CYy::CXiSkeletonActor::MaybeShowWepScheduler(int a2)
{
	if (((1 << a2) & (this->field_840 >> 8) & 0x3FF) == 0) {
		return;
	}

	unsigned int v3{};
	if (a2 == 0) {
		v3 = '10w!';
	}
	else if (a2 == 1) {
		v3 = '11w!';
	}
	else
		return;

	if (FFXI::Config::FsConfig::GetConfig(FFXI::Constants::Enums::FsConfigSubjects::Subject190) != 1) {
		if (this->StartScheduler(v3, nullptr, nullptr) == true) {
			v3 = ((~(1 << a2) << 8) | 0xFFFC00FF) & this->field_840;
			this->field_840 = v3;
		}
	}
	if (this->StartScheduler(v3, 0, 0) != 0)
		this->field_840 |= ((1 << a2) & 0x3FF) << 8;
}

int FFXI::CYy::CXiSkeletonActor::VirtActor6()
{
	return this->field_66C;
}

void FFXI::CYy::CXiSkeletonActor::VirtActor11()
{
	this->field_734 = 0;
}

float FFXI::CYy::CXiSkeletonActor::GetWidthScale()
{
	CYyModelBase* base = *this->Model.GetBase();
	if (base == nullptr) {
		return 1.0;
	}

	float* fptr = base->Skeleton.GetSomeFloatPtr();
	if (fptr == nullptr) {
		return 1.0;
	}

	float v5 = fptr[2] - fptr[3];
	float v6 = this->VirtActor58();
	if (v6 >= 0.0) {
		return v6 * v5 * 0.58823526;
	}

	if (this->field_760 >= 0.0) {
		return this->field_760 * v5 * 0.58823526;
	}

	return this->field_754 * v5 * 0.58823526;
}

float FFXI::CYy::CXiSkeletonActor::GetHeightScale()
{
	CYyModelBase* base = *this->Model.GetBase();
	if (base == nullptr) {
		return 1.0;
	}

	float* fptr = base->Skeleton.GetSomeFloatPtr();
	if (fptr == nullptr) {
		return 1.0;
	}

	float v5 = fptr[1] - fptr[0];
	float v6 = this->VirtActor58();
	if (v6 >= 0.0) {
		return v6 * v5 * 0.52631581;
	}

	if (this->field_75C >= 0.0) {
		return this->field_75C * v5 * 0.52631581;
	}

	return this->field_754 * v5 * 0.52631581;
}

float FFXI::CYy::CXiSkeletonActor::GetDepthScale()
{
	CYyModelBase* base = *this->Model.GetBase();
	if (base == nullptr) {
		return 1.0;
	}

	float* fptr = base->Skeleton.GetSomeFloatPtr();
	if (fptr == nullptr) {
		return 1.0;
	}

	float v5 = fptr[4] - fptr[5];
	float v6 = this->VirtActor58();
	if (v6 >= 0.0) {
		return v6 * v5 * 0.52631581;
	}

	if (this->field_758 >= 0.0) {
		return this->field_758 * v5 * 0.52631581;
	}

	return this->field_754 * v5 * 0.52631581;
}

void FFXI::CYy::CXiSkeletonActor::VirtActor32(char a2)
{
	this->field_8A5 = a2;
}

void FFXI::CYy::CXiSkeletonActor::VirtActor34(char a2)
{
	this->field_8A6 = a2;
}

void FFXI::CYy::CXiSkeletonActor::VirtActor36(char a2)
{
	this->field_8A7 = a2;
}

void FFXI::CYy::CXiSkeletonActor::VirtActor38(char a2)
{
	this->field_8A8 = a2;
}

void FFXI::CYy::CXiSkeletonActor::VirtActor40(char a2)
{
	this->field_8A9 = a2;
}

bool FFXI::CYy::CXiSkeletonActor::VirtActor75()
{
	return this->field_8AC & 0x02;
}

void FFXI::CYy::CXiSkeletonActor::VirtActor89()
{
	this->field_9F8 = 1;
}

void FFXI::CYy::CXiSkeletonActor::VirtActor90()
{
	this->field_A04 = 1;
}

D3DXVECTOR4* FFXI::CYy::CXiSkeletonActor::VirtActor101()
{
	return &this->field_5FC;
}

D3DXVECTOR4* FFXI::CYy::CXiSkeletonActor::VirtActor102()
{
	return &this->field_61C;
}

void FFXI::CYy::CXiSkeletonActor::VirtActor103(int a2, D3DXVECTOR4* a3)
{
	*a3 = this->Model.field_4;
	D3DXVECTOR4 v16{};
	v16.x = this->Model.field_24.x;
	v16.y = this->Model.field_24.z;
	v16.z = this->Model.field_24.y;
	v16.w = 1.0;

	this->Model.GetSomething(0, a2, a3);
	float a2a{ 0.0 };
	int v7 = this->Model.GetSomeBaseIndex(a2, 0);
	if (this->VirtActor204() == true
		|| this->CheckGameStatus85(255) == false
		|| v7 != 0) {
		if (this->VirtActor201() == false
			&& this->CheckGameStatus5(255) == true
			&& v7 == 0) {
					a2a = -1.3;
		}
		else if (this->VirtActor209() == false) {
			if (this->TestSomethingIfAtel(255) == true && v7 == 0)
				a2a = -0.5;
		}
		else if (this->field_768[0] == 0) {
			a2a = -1.3;
		}
		else {
			int race = this->GetRace();
			int something = this->CibCollect.GetSomething(race);
			if (something == 0
				|| something == 6)
				a2a = -1.3;
			else
				a2a = -0.5;
		}
	}
	
	double v13 = this->VirtActor58();
	if (v13 < 0.0) {
		if (this->field_75C >= 0.0) {
			v13 = this->field_75C;
		}
		else {
			v13 = this->field_754;
		}
	}

	a3->y += v13 * a2a;
}

void FFXI::CYy::CXiSkeletonActor::GetElemLocal(unsigned int a2, D3DXVECTOR4* a3)
{
	*a3 = this->Model.field_4;
	D3DXVECTOR4 v12{};
	v12.x = this->Model.field_24.x;
	v12.y = this->Model.field_24.z;
	v12.z = this->Model.field_24.y;
	v12.w = 1.0;

	bool a = this->Model.GetSomething(0, a2, a3);
	a3->x -= this->Model.field_4.x;
	a3->y -= this->Model.field_4.y;
	a3->z -= this->Model.field_4.z;

	float a1a = 0.0;
	int sbi = this->Model.GetSomeBaseIndex(a2, 0);
	int status = this->GetGameStatus();
	if ((this->VirtActor201() == true || this->CheckGameStatus5(status) == false) 
	&& (this->VirtActor204() == true || this->CheckGameStatus85(status) == false)
	|| sbi != 0) {
			if (this->VirtActor209() == false) {
				if (this->TestSomethingIfAtel(status) == true) {
					if (sbi == 0) {
						a1a = -0.5;
					}
			}
		}
	}
	else {
		a1a = -1.3;
	}
	
	float scale{ 0.0 };
	float v58 = this->VirtActor58();
	if (v58 >= 0.0) {
		scale = v58;
	}
	else if (this->field_75C >= 0.0) {
		scale = this->field_75C;
	}
	else {
		scale = this->field_754;
	}

	a3->y += scale * a1a;
}

void FFXI::CYy::CXiSkeletonActor::ActorFindResource(CMoResource***, int, int)
{
	//sub //TODO
	exit(0x182343);
	//CYyModelDt** ModelDt{ nullptr }, ** v10{ nullptr };
	//CYyModelBase** p_base = this->Model.GetBase();
	//if (p_base != nullptr && *p_base != nullptr) {
	//	CYyModelBase* v8 = *p_base;
	//	CYyModelDt* v11{ nullptr };
	//	CYyModelBase** p_Previous{ nullptr };
	//	CMoOs2** a1{ nullptr };
	//	CMoOs2* v13{ nullptr };
	//	while (1)
	//	{
	//		ModelDt = v8->GetModelDt();
	//		v10 = ModelDt;
	//		if (ModelDt)
	//		{
	//			v11 = *ModelDt;
	//			if (*v10)
	//				break;
	//		}
	//	LABEL_19:
	//		p_Previous = &(*p_Previous)->Previous;
	//		v8 = *p_Previous;
	//		if (!*p_Previous)
	//		{
	//			goto LABEL_21;
	//		}
	//	}

	//	while (1)
	//	{
	//		a1 = v11->field_8;
	//		if (a1)
	//		{
	//			v13 = *a1;
	//			if (v13)
	//				break;
	//		}
	//	LABEL_18:
	//		v10 = &(*v10)->field_4;
	//		v11 = *v10;
	//		if (!*v10)
	//			goto LABEL_19;
	//	}
	//	CMoResource** RPLHead = v13->Header.RPLHead;
	//	ResourceList v21{};
	//	while (1)
	//	{
	//		a1 = RPLHead;
	//		v15 = RPLHead ? (YmResource*)*RPLHead : 0;
	//		v21.PrepareFromResource(v15, a3, v6, -1);
	//		if (v21.field_14)
	//			break;
	//		
	//		Head = YmResource::GetHead(&(*a1)->baseclass_0);
	//		if (!Head)
	//			goto LABEL_18;
	//		v17 = a1 ? *a1 : 0;
	//		if (Head == (YmResourceFile*)v17)
	//			goto LABEL_18;
	//		RPLHead = (YmScheduler**)Head->PreHeader.PointerToThisInRPL;
	//		--YmResource::SomeCounter;
	//	}
	//	result = a2;
	//	*a2 = v21.field_14->PreHeader.PointerToThisInRPL;
	//}
	//else {
	//LABEL_21:
	//	a1 = *(YmScheduler***)((int(__thiscall*)(CXiSkeletonActor*, int*))v4->VTable->GetModelFile)(v4, &a4);
	//	if (a1 && CheckResourceThing__CMoResourceMng((YmResource***)&a1))
	//	{
	//		YmResource::AnotherResourceSearcher(&(*a1)->baseclass_0, a2, a3, v6);
	//		return a2;
	//	}
	//	else
	//	{
	//		result = a2;
	//		*a2 = 0;
	//	}
	//}
}

void FFXI::CYy::CXiSkeletonActor::SetMotionLock(bool a2)
{
	this->MotionLock = a2;
}

bool FFXI::CYy::CXiSkeletonActor::IsMotionLock()
{
	return this->MotionLock;;
}

void FFXI::CYy::CXiSkeletonActor::OnDraw()
{
	if (this->field_8C8.field_C == 0) {
		this->Draw();
	}
	else {
		if (this->field_8E8 == 1) {
			this->Draw();
		}
		this->field_8E8 = 0;
	}
}

void FFXI::CYy::CXiSkeletonActor::VirtActor154(CYyResourceFile**)
{
	//sub //TODO
	exit(0x103255B0);
}

bool FFXI::CYy::CXiSkeletonActor::SetAction(int a2, CXiActor* a3, void* a4)
{
	//sub //TODO
	exit(0x100CCED0);
	return false;
}

bool FFXI::CYy::CXiSkeletonActor::IsDirectionLock()
{
	return this->field_838;
}

void FFXI::CYy::CXiSkeletonActor::SetConstrain(char a2, int a3)
{
	this->field_83C[a3] = a2 == 1;
}

short FFXI::CYy::CXiSkeletonActor::IsConstrain()
{
	return *(short*)this->field_83C;
}

void FFXI::CYy::CXiSkeletonActor::VirtActor191(char a2, int a3)
{
	this->field_83C[a3 + 2] = a2 == 1;
}

char FFXI::CYy::CXiSkeletonActor::VirtActor192(int a2)
{
	return this->field_83C[a2 + 2];
}

float* FFXI::CYy::CXiSkeletonActor::VirtActor232(bool a2) {

	CYyModelBase* base = *this->Model.GetBase();
	if (base != nullptr) {
		float* sfp = base->Skeleton.GetSomeFloatPtr();
		if (sfp != nullptr) {
			int index = 0;
			switch (this->GetGameStatus()) {
			case 0:
			case 4:
			case 7:
			case 28:
			case 31:
				index = 0;
				break;
			case 1:
			case 6:
			case 38:
			case 39:
			case 40:
			case 41:
			case 42:
			case 43:
			case 50:
			case 51:
			case 52:
			case 53:
			case 56:
			case 57:
			case 58:
			case 59:
			case 60:
			case 61:
			case 62:
				index = 1;
				break;
			case 2:
			case 3:
				index = 2;
				break;
			default:
				if (this->CheckGameStatus5(255) == true
					|| this->CheckGameStatus85(255) == true) {
					index = 1;
				}
				break;
			}
			return sfp + 6 * index;
		}
	}

	if (a2 == true)
		return nullptr;

	return CXiSkeletonActor::skeleton_subsitute;
}

KzCibCollect* FFXI::CYy::CXiSkeletonActor::VirtActor236()
{
	return &this->CibCollect;
}

short FFXI::CYy::CXiSkeletonActor::VirtActor238()
{
	return this->field_670;
}

void FFXI::CYy::CXiSkeletonActor::VirtSkeletonActor246(CYyResourceFile*** a2)
{
	*a2 = this->field_864;
}
