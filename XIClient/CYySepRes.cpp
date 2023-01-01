#include "CYySepRes.h"
#include "CYyResourceFile.h"
#include "SoundMng.h"
#include "Globals.h"
#include "CYyCamMng2.h"
#include "CYySoundElem.h"
#include "CYyDb.h"
using namespace FFXI::CYy;

const CYyClass CYySepRes::CYySepResClass{
	"CYySepRes", sizeof(CYySepRes), &CMoResource::CMoResourceClass
};

int FFXI::CYy::CYySepRes::Calc3D(float* a1, int* a2, D3DXVECTOR3* a3, float a4, float a5, float a6, unsigned char a7, int a8)
{
	CYyCamMng2* cammng = Globals::g_pCYyCamMng2;
	if (cammng == nullptr) {
		*a1 = 0.0;
		*a2 = 0;
		return 0;
	}
	if (a5 == 0.0) {
		a5 = CYySoundElem::sound_far;
	}
	if (a4 == 0.0) {
		a4 = CYySoundElem::sound_near;
	}
	if (a6 == 0.0) {
		a6 = CYySoundElem::sound_width;
	}
	else {
		a6 *= 512.0;
	}

	D3DXVECTOR3 v28 = cammng->field_60 - *a3;
	if (a8 == 2) {
		v28.y = 0.0;
	}
	else if (a8 != 1) {
		v28.y *= 3.0;
	}
	float v10 = sqrt(Globals::Vec3Inner(&v28, &v28));

	if (v10 < 0.1) {
		return *a2;
	}
	if (v10 <= a5) {
		if (v10 > a4) {
			*a2 = (long long) ((1.0 - (v10 - a4) / (a5 - a4)) * (double)*a2);
		}
	}
	else {
		if (a7 == 0) {
			*a2 = 0;
			return 0;
		}
		*a2 = 1;
	}

	CYyDb::g_pCYyDb->field_94.Vec3TransformDrop4(&v28, a3);
	float v30 = v28.z;
	if (v30 > 0.1) {
		*a2 = (long long)((double)*a2 * CYySoundElem::back_sound_rate);
	}
	else if (v30 < 0.0) {
		v30 = -v30;
	}
	
	if (v30 < 1.0) {
		v30 = 1.0;
	}
	//?
	else if (v30 == 0.0) {
		v30 = 0.001;
	}

	float v22 = CYyDb::g_pCYyDb->field_2F4 / v30 * v28.x / a6;
	if (v22 >= 1.0) {
		v22 = 1.0;
	}
	else if (v22 < -1.0) {
		v22 = -1.0;
	}
	float v26 = v22 * *a1 + 64.0;
	if (v26 > 127.0) {
		v26 = 127.0;
	}
	else if (v26 < 0.0) {
		v26 = 0.0;
	}
	*a1 = v26;
	if (*a2 == 0) {
		return a7;
	}
	return *a2;
}

const CYyClass* FFXI::CYy::CYySepRes::GetRuntimeClass()
{
	return &CYySepResClass;
}

void FFXI::CYy::CYySepRes::Close()
{
	CYySoundElem* elem = CYySoundElem::head;
	while (elem != nullptr) {
		if (elem->res == this->PreHeader.PointerToThisInRPL) {
			delete elem;
			elem = CYySoundElem::head;
			continue;
		}
		elem = elem->previous;
	}

	if (CYySoundElem::zone_sound == this) {
		CYySoundElem::SetZoneSound(nullptr);
	}

	for (int i = 0; i < CYy::CYySoundElem::se_req_num; ++i) {
		if (CYySoundElem::req_buffer[i] == (CYySepRes**)this->PreHeader.PointerToThisInRPL) {
			CYySoundElem::req_buffer[i] = nullptr;
		}
	}
}

int FFXI::CYy::CYySepRes::Play(int a2, int a3)
{
	float v4[4] = { 0.0, 0.0, 0.0, 0.0 };
	v4[0] = (float)a2;
	return this->Play(v4, a3);
}

int FFXI::CYy::CYySepRes::Play(float* a2, int a3)
{
	if (FFXI::SoundMng::SoundThreadRunning && SoundMng::CanPlaySoundResource((CYySepRes**)this->PreHeader.PointerToThisInRPL)) {
		FFXI::SoundMng::sound_command_send = 1;
		if (FFXI::SoundMng::g_pSoundMng)
			return SoundMng::SqsSeCheck((char*)&this->field_30, a2, a3);
	}

	return 0;
}

void FFXI::CYy::CYySepRes::Stop(int a2)
{
	SoundMng::sound_command_send = 1;
	if (SoundMng::SoundThreadRunning == false) {
		return;
	}

	if (FFXI::SoundMng::g_pSoundMng != nullptr) {
		FFXI::SoundMng::g_pSoundMng->QueueNewSoundCommand(0, a2, this->field_38, 0, 0, 0, 0, 0);
	}

	if (this->field_38 == 0x8BF || this->field_38 == 0x80F) {
		CYySoundElem::check_flag = false;
	}
}

bool FFXI::CYy::CYySepRes::CheckFourCC()
{
	CMoResource* v5{ nullptr };
	if (this->Header.RPLHead)
		v5 = *this->Header.RPLHead;
	else
		return false;

	if (v5->Header.FourCC == 'rtxe')
		return false;

	int v4 = this->Header.FourCC;
	int v6 = 0;
	for (int i = 0; i < 4; ++i) {
		if ((v4 & 0xFF) < '0' || (v4 & 0xFF) > '9')
			return false;
		v4 >>= 8;
	}

	v4 = this->Header.FourCC;
	if ((v4 & 0xFF000000) > 0x39000000)
		return false;
	if ((v4 & 0xFF0000) > 0x350000)
		return false;
	if ((v4 & 0xFF00) > 0x3900)
		return false;
	if ((v4 & 0xFF) > 0x31)
		return false;

	return true;
}