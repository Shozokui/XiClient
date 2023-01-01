#include "CYySoundElem.h"
#include "CYyGenerator.h"
#include "CYyDb.h"
#include "CYyCamMng2.h"
#include "CYySepRes.h"
#include "CMoSphRes.h"
#include "SoundMng.h"
#include "Globals.h"
#include "SoundController.h"
#include "BaseProcessor.h"
using namespace FFXI::CYy;

const CYyClass FFXI::CYy::CYySoundElem::CYySoundElemClass{
	"CYySoundElem", sizeof(CYySoundElem), &CMoElem::CMoElemClass
};

CYySoundElem* CYySoundElem::head{ nullptr };
CYySepRes* CYySoundElem::zone_sound{ nullptr };
int CYySoundElem::zone_sound_id{ 0 };
float CYySoundElem::system_volume{ 1.0 };
float CYySoundElem::effect_volume{ 1.0 };
float CYySoundElem::zone_volume{ 1.0 };
float CYySoundElem::back_sound_rate{ 0.64999998 };
float CYySoundElem::sound_near{ 3.0 };
float CYySoundElem::sound_far{ 30.0};
float CYySoundElem::sound_width{ 512 };

bool CYySoundElem::sound_standby{ false };
bool CYySoundElem::check_flag{ false };

int CYySoundElem::play_num{ 0 };
int CYySoundElem::se_req_num{ 0 };
int CYySoundElem::entry_num{ 0 };
int CYySoundElem::report_buffer[FFXI::Constants::Values::INDEX_SOUND_REPORT_MAX] = { 0 };
CYySepRes** CYySoundElem::req_buffer[FFXI::Constants::Values::INDEX_SOUND_REQUEST_MAX] = { nullptr };

void FFXI::CYy::CYySoundElem::SysMove()
{
	if (CYy::CYySoundElem::se_req_num == 0
		&& SoundMng::sound_command_send == 0
		&& CYySoundElem::head == nullptr) {
		return;
	}
	CYySoundElem::sound_standby = true;
	CYySoundElem::play_num = 0;
	CYySoundElem* elem = CYySoundElem::head;
	unsigned char count{0};

	if (FFXI::SoundMng::g_pSoundMng != nullptr) {
		do {
			if (elem == nullptr) {
				break;
			}
			if (elem->field_1E0 > 0) {
				count = CYySoundElem::play_num;
				if (SoundController::GetBuff1Flag(0, elem->field_1E0, -1) != 0) {
					CYySoundElem::report_buffer[CYySoundElem::play_num] = elem->field_1E0;
					count += 1;
					CYySoundElem::play_num = count;
				}
			}
			elem = elem->previous;
		} while (count < 64);
	}
	
	CYySoundElem::entry_num = count;

	if (FFXI::SoundMng::g_pSoundMng != nullptr) {
		elem = CYySoundElem::head;
		int i = count;
		while (i < 64) {
			if (elem == nullptr) {
				break;
			}
			if (elem->res != nullptr) {
				CYySepRes* sep = (CYySepRes*)*elem->res;
				CYySoundElem::report_buffer[i] = sep->field_38;
				i += 1;
				CYySoundElem::entry_num = i;
			}
			elem = elem->previous;
		}
	}

	elem = CYySoundElem::head;
	while (elem != nullptr) {
		elem->OnMove2();
		elem = elem->previous;
	}

	for (int j = 0; j < CYy::CYySoundElem::se_req_num; ++j) {
		CYySepRes** psep = CYySoundElem::req_buffer[j];
		if (psep != nullptr) {
			if (*psep != nullptr) {
				CYySoundElem::SendData(psep);
			}
		}
	}

	CYy::CYySoundElem::se_req_num = 0;
}

void FFXI::CYy::CYySoundElem::SetZoneSound(CYySepRes* a2)
{
	if (a2 != nullptr) {
		if (a2 != CYySoundElem::zone_sound) {
			CYySoundElem::zone_sound_id = a2->Play(64, (long long)(CYySoundElem::system_volume * 127.0));
			CYySoundElem::zone_sound = a2;
		}
	}
	else if (CYySoundElem::zone_sound != nullptr) {
		CYySoundElem::zone_sound->Stop(CYySoundElem::zone_sound_id);
		CYySoundElem::zone_sound = nullptr;
	}
}

void FFXI::CYy::CYySoundElem::AddEntry(CYySepRes** a1)
{
	if (a1 == nullptr || *a1 == nullptr) {
		return;
	}

	int top = CYySoundElem::entry_num + CYySoundElem::play_num;
	if ((top + 1) < 64) {
		CYySoundElem::report_buffer[top] = (*a1)->field_38;
		CYySoundElem::entry_num += 1;
	}
}

bool FFXI::CYy::CYySoundElem::SendData(CYySepRes** a1)
{
	if (a1 == nullptr || *a1 == nullptr) {
		return false;
	}

	if (CYySoundElem::EntryExist(a1) == true) {
		return true;
	}
	if (CYySoundElem::sound_standby == false) {
		return false;
	}

	CYySoundElem::AddEntry(a1);
	return true;
}

bool FFXI::CYy::CYySoundElem::EntryExist(CYySepRes** a1)
{
	if (a1 == nullptr || *a1 == nullptr) {
		return false;
	}

	if (CYySoundElem::entry_num <= 0) {
		return false;
	}

	int* i = CYySoundElem::report_buffer + CYySoundElem::play_num;
	int a = 0;
	do {
		a += 1;
		if (a >= CYySoundElem::entry_num) {
			return false;
		}
		i += 1;
	} while (*i != (*a1)->field_38);

	return true;
}

bool FFXI::CYy::CYySoundElem::PlayExist(int a1)
{
	if (a1 == 0) {
		return false;
	}

	if (CYySoundElem::play_num <= 0) {
		return false;
	}

	for (int i = 0; i < CYySoundElem::play_num; ++i) {
		if (CYySoundElem::report_buffer[i] == a1) {
			return true;
		}
	}

	return false;
}

const CYyClass* FFXI::CYy::CYySoundElem::GetRuntimeClass()
{
	return &CYySoundElem::CYySoundElemClass;
}

char FFXI::CYy::CYySoundElem::OnMove()
{
	float tick = CYyDb::g_pCYyDb->CheckTick();
	if (this->Life - tick < 1.0) {
		this->Life = tick + 1.0;
	}
	if (this->field_FC != nullptr) {
		if (this->ExecTag() == true) {
			return 1;
		}
	}
	this->field_138 = 0.0;
	if (this->field_198 != nullptr) {
		D3DXVECTOR3 v2{};
		this->field_198->path.GetDirection(&v2, &FFXI::CYyDb::g_pCYyDb->CameraManager->field_60);
		this->field_54 = v2 + FFXI::CYyDb::g_pCYyDb->CameraManager->field_60;
	}

	return 0;
}

bool FFXI::CYy::CYySoundElem::VirtElem1(FFXI::Constants::Enums::ElemType a2)
{
	return a2 == FFXI::Constants::Enums::ElemType::Sound;
}

bool FFXI::CYy::CYySoundElem::IsNever()
{
	if (this->field_FC != nullptr) {
		if (*this->field_FC != nullptr) {
			if ((*this->field_FC)->IsNever() == true) {
				return true;
			}
		}
	}
	if (this->res == nullptr) {
		return false;
	}

	CYySepRes* sep = (CYySepRes*)*this->res;

	if (sep == nullptr) {
		return false;
	}

	return (sep->field_3C & 0x80000000) != 0;
}

FFXI::CYy::CYySoundElem::CYySoundElem()
{
	this->field_138 = 0.0;
	this->field_1E0 = 0;
	memset(this->field_1C0, 0, sizeof(this->field_1C0));
	this->field_1BC = 0;
	this->field_198 = 0;
	this->LinkToGenerator(nullptr);
	this->field_19C = 0;
	this->field_108 = 0;
	this->field_1E8 = 0;
	this->field_1E9 = 0;
	this->field_1E4 = 3.0;
	this->previous = CYySoundElem::head;
	CYySoundElem::head = this;
	this->field_176 = sizeof(CYySoundElem);
}

FFXI::CYy::CYySoundElem::~CYySoundElem()
{
	CYySoundElem* se = CYySoundElem::head;
	CYySoundElem* pre = nullptr;
	while (se != nullptr) {
		if (se == this) {
			break;
		}
		pre = se;
		se = se->previous;
	}

	if (pre != nullptr) {
		pre->previous = this->previous;
	}
	else {
		CYySoundElem::head = this->previous;
	}

	if (this->res != nullptr) {
		CYySepRes* sep = (CYySepRes*) *this->res;
		if (sep != nullptr) {
			if (this->field_FC != nullptr) {
				if (*this->field_FC != nullptr) {
					if (this->field_1E0 != 0) {
						SoundMng::sound_command_send = 1;
						if (SoundMng::SoundThreadRunning == true) {
							if (FFXI::SoundMng::g_pSoundMng != nullptr) {
								FFXI::SoundMng::g_pSoundMng->QueueNewSoundCommand(FFXI::Constants::Enums::SoundCommand::SEFadeOut, this->field_1E0, 0, this->field_114, 0, 0, 0, 0);
							}
						}
						if (sep->field_38 == 0x8BF || sep->field_38 == 0x80F) {
							CYySoundElem::check_flag = false;
						}
					}
				}
			}

			sep->SomeClean3();
		}
	}
}

void FFXI::CYy::CYySoundElem::OnMove2()
{
	if (this->OnPlayUpdate() == false) {
		if (this->PlayExist() == true) {
			if (CYySoundElem::SendData((CYySepRes**)this->res) == false) {
				this->Stop();
			}
		}
	}
}

bool FFXI::CYy::CYySoundElem::OnPlayUpdate()
{
	if (CYySoundElem::sound_standby == true) {
		if (this->field_1E4 > 0.0) {
			this->field_1E4 -= 1.0;
		}
	}

	if (SoundMng::SoundThreadRunning == true) {
		D3DXVECTOR3 v1{};
		if (this->field_19C != 0) {
			this->field_54 = this->field_1A0;
		}
		FFXI::Math::WMatrix* smtx = this->GetSomeMatrix();
		if (smtx != nullptr && this->field_FC != nullptr && *this->field_FC != nullptr) {
			CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor15(&v1, smtx, &this->field_54);
		}
		else {
			v1 = this->field_54;
		}
		if (this->res != nullptr && *this->res != nullptr) {
			float v22[4];
			memcpy(v22, this->field_1D0, sizeof(this->field_1D0));
			int v20 = this->field_1BE;
			if (CYySepRes::Calc3D(
				v22, 
				&v20, 
				&v1, 
				this->s_near, 
				this->s_far, 
				this->s_width, 
				this->field_1E8, 
				this->field_1E9) != 0) {
				int v12 = (double)v20 * CYySoundElem::effect_volume;
				if (this->field_1E0 == 0) {
					this->Play(v22, v12);
				}
				if (this->field_1E0 < 0) {
					if (this->field_FC != nullptr) {
						(*this->field_FC)->ElemDie(this);
					}
					delete this;
					return true;
				}
				if (this->PlayExist() == true) {
					if (memcmp(this->field_1C0, v22, 12) != 0) {
						memcpy(this->field_1C0, v22, 16);
						if (SoundMng::SoundThreadRunning == true) {
							SoundMng::SqsSePanDirect(this->field_1E0, (int)v22[0], 0);
						}
					}
					if (this->field_1BC != v12) {
						this->field_1BC = v12;
						if (SoundMng::SoundThreadRunning == true) {
							SoundMng::SqsSeVolumeDirect(this->field_1E0, v12, 0);
						}
					}
					return false;
				}
			}
			if (this->IsNever() == true) {
				this->Stop();
				return false;
			}
		}
	}

	if (this->field_FC != nullptr) {
		(*this->field_FC)->ElemDie(this);
	}
	delete this;
	return true;
}

bool FFXI::CYy::CYySoundElem::PlayExist()
{
	if (this->field_1E4 <= 0.0) {
		return CYySoundElem::PlayExist(this->field_1E0);
	}
	return true;
}

void FFXI::CYy::CYySoundElem::Play(float* a2, int a3)
{
	memcpy(this->field_1C0, a2, sizeof(this->field_1C0));
	this->field_1BC = (double)a3 * CYySoundElem::effect_volume;
	if (this->res != nullptr) {
		CYySepRes* sep = (CYySepRes*)*this->res;
		if (sep != nullptr) {
			this->field_1E0 = sep->Play(a2, this->field_1BC);
		}
	}
	this->field_1E4 = 3.0;
}

void FFXI::CYy::CYySoundElem::Stop()
{
	if (this->field_1E0 > 0) {
		if (this->res != nullptr) {
			CYySepRes* sep = (CYySepRes*)*this->res;
			if (sep != nullptr) {
				sep->Stop(this->field_1E0);
			}
		}
		this->field_1E0 = 0;
	}
}
