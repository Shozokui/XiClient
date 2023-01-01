#include "YmCombineWeather.h"
#include "CYyResourceFile.h"
#include "CYyGenerator.h"
#include "Globals.h"
#include "CYyDb.h"
#include "ResourceList.h"
#include "XiZone.h"
#include "CMoWeather.h"
#include "CYyScheduler.h"
#include "CYySepRes.h"

using namespace FFXI::CYy;

const CYyClass YmCombineWeather::YmCombineWeatherClass{
	"YmCombineWeather", sizeof(YmCombineWeather), &CYyObject::CYyObjectClass
};

const CYyClass* FFXI::CYy::YmCombineWeather::GetRuntimeClass()
{
	return &YmCombineWeather::YmCombineWeatherClass;
}

bool __cdecl ActivateWeatherGenerators(CMoResource** _a1, void* _a2)
{
	CYyGenerator* gen{ nullptr };
	if (_a1 != nullptr) {
		gen = (CYyGenerator*) *_a1;
	}

	bool p = (gen->flags & 0x1000) == 0;
	bool p0 = XiZone::zone->PlaceCode != 0;
	bool p1 = gen->IsNever();
	unsigned char* s = gen->GetElemStatus();
	bool p2 = (*s & 0x00000004) != 0;
	bool p3 = (*s & 0x04000000) != 0;

	if (p || p0 && (p1 && p2 || p3)) {
		return false;
	}

	gen->Activate();
	//A2 is YmCombineWeather
	gen->CMoAttachments::field_34 = _a2;
	return false;
}

FFXI::CYy::YmCombineWeather::YmCombineWeather(XiArea* a2, int a3, int a4)
{
	this->Area = a2;
	this->WeatherFile = a2->field_14;
	this->field_34 = 0;
	if (!this->WeatherFile) return;

	ResourceList v30{};
	CYyResourceFile** file{ nullptr };
	this->WeatherFile->AnotherResourceSearcher((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, 'taew');
	if (file && *file)
		this->WeatherFile = *file;
	
	this->WeatherFile->AnotherResourceSearcher((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, a3);
	if (!file || !*file)
		this->WeatherFile->AnotherResourceSearcher((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, 'ynus');

	if (file && *file) {
		this->WeatherFile = *file;
		if (this->Area->field_14) {
			this->Area->field_14->AnotherResourceSearcher((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, 'taew');
			if (file && *file) {
				v30.Clear();
				v30.PrepareFromResource(*file, FFXI::Constants::Enums::ResourceType::Scheduler, 0, -1);
				CYyScheduler* sched = (CYyScheduler*)v30.GetNext(false);
				while (sched) {
					sched->Stop();
					sched = (CYyScheduler*)v30.GetNext(false);
				}
			}
		}
		if (this->WeatherFile) {
			v30.Clear();
			v30.PrepareFromResource(this->WeatherFile, FFXI::Constants::Enums::ResourceType::Scheduler, 0, -1);
			CYyScheduler* sched = (CYyScheduler*)v30.GetNext(false);
			while (sched) {
				if (sched->IsLoopScheduler())
					sched->Execute(0, nullptr);
				sched = (CYyScheduler*)v30.GetNext(false);
			}
		}
		this->field_18.SetTickFrame(a4);
		this->field_14.SetTickFrame(this->field_18.Time);
		this->AreasCombineWeather = this->Area->CombineWeather;
		this->Area->CombineWeather = this;
		this->fourCC = a3;
		this->field_35 = 0;
		this->PrevSound = nullptr;
		this->NextSound = nullptr;
		this->field_20 = 0;
		this->PrevWeather = nullptr;
		this->NextWeather = nullptr;
		this->field_36 = 0;
		this->field_3A = 0;
		this->field_3E = 0;
		this->field_42 = 0;
		this->WeatherFile->FindResourceWithCallback(FFXI::Constants::Enums::ResourceType::Generater, 0, ActivateWeatherGenerators, this);
	}
	else {
		this->WeatherFile->AnotherResourceSearcher((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, 'taew');
		if (file && *file)
			this->WeatherFile = *file;
	}
}

FFXI::CYy::YmCombineWeather::~YmCombineWeather()
{
	if (this->Area->field_20 == this->PrevSound)
		this->Area->field_20 = nullptr;

	ResourceList v9{};
	v9.Clear();
	v9.PrepareFromResource(this->Area->field_14, FFXI::Constants::Enums::ResourceType::Rmp, 0, -1);
	CYyResourceFile* next = (CYyResourceFile*)v9.GetNext(false);
	bool found = false;
	while (next) {
		if (next == this->WeatherFile) {
			found = true;
			break;
		}
		next = (CYyResourceFile*)v9.GetNext(false);
	}

	v9.PrepareFromLastCreated(CYyDb::g_pCYyDb->pCMoResourceMng, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
	CYyGenerator* gen = (CYyGenerator*)v9.GetNext(false);
	while (gen) {
		if ((YmCombineWeather*)gen->CMoAttachments::field_34 == this) {
			gen->CMoAttachments::field_34 = 0;
			if (found) {
				gen->MaybeDeactivate();
				v9.PrepareFromLastCreated(CYyDb::g_pCYyDb->pCMoResourceMng, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
			}
		}
		gen = (CYyGenerator*)v9.GetNext(false);
	}

	v9.PrepareFromResource(this->WeatherFile, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
	gen = (CYyGenerator*)v9.GetNext(false);
	while (gen) {
		if ((YmCombineWeather*)gen->CMoAttachments::field_34 == this) {
			gen->CMoAttachments::field_34 = 0;
			if (found) {
				gen->MaybeDeactivate();
				v9.PrepareFromResource(this->WeatherFile, FFXI::Constants::Enums::ResourceType::Generater, 0, -1);
			}
		}
		gen = (CYyGenerator*)v9.GetNext(false);
	}

	if (this->Area->Weather == this->field_20)
		this->Area->SetCurrentWeather(nullptr);

	this->ReleaseCurrentWeather();
	YmCombineWeather* parent = this->GetParentWeather();
	if (parent)
		parent->AreasCombineWeather = this->AreasCombineWeather;
	else {
		this->Area->CombineWeather = this->AreasCombineWeather;
	}

	v9.PrepareFromResource(this->WeatherFile, FFXI::Constants::Enums::ResourceType::Scheduler, 0, -1);
	CYyScheduler* sched = (CYyScheduler*)v9.GetNext(false);
	while (sched) {
		sched->Stop();
		sched = (CYyScheduler*)v9.GetNext(false);
	}
}

void FFXI::CYy::YmCombineWeather::OnMoveAll()
{
	this->OnMove();
}

void FFXI::CYy::YmCombineWeather::OnMove()
{
	if (this->AreasCombineWeather)
		this->AreasCombineWeather->OnMove();

	if (this->field_14.Time) {
		if (this->field_14.DecNeck())
			this->field_14.SetTickFrame(0);
	}
	else {
		if (this->AreasCombineWeather) {
			delete this->AreasCombineWeather;
			this->AreasCombineWeather = nullptr;
		}
	}

	this->field_1C = 1.0;
	this->MakeCurrentWeather();
	if (this->AreasCombineWeather == nullptr)
		return;

	this->field_1C = (double)(this->field_18.Time - this->field_14.Time) / (double)this->field_18.Time;
	if (this->AreasCombineWeather->field_20 == nullptr)
		return;

	if (this->field_20 == nullptr)
		return;

	XiWeather* weather = YmCombineWeather::CreateCurrentWeather();
	weather->Combine(this->field_20, this->AreasCombineWeather->field_20, this->field_1C);
	this->ReleaseCurrentWeather();
	this->field_20 = weather;
	this->AreasCombineWeather->field_1C = 1.0 - this->field_1C;
}

void FFXI::CYy::YmCombineWeather::MakeCurrentWeather()
{
	XiDateTime v22{};
	XiDateTime::GetWeatherTimeNow(&v22);
	if (!this->PrevSound && !this->field_35)
		this->FindSetPrevSound();
	if (!this->NextSound && !this->field_35)
		this->FindSetNextSound();

	if (this->NextSound != nullptr) {
		if (*this->NextSound != nullptr) {
			if (this->PrevSound != nullptr) {
				if (*this->PrevSound != nullptr) {
					if (this->PrevSound != this->NextSound) {
						XiDateTime v19{}, v20{}, v21{}, v25{};
						XiDateTime::GetWeatherTimeNow(&v20);
						(*this->PrevSound)->GetActivateTime(&v25);
						v21.SetTickFrame(v25.Time);
						(*this->NextSound)->GetActivateTime(&v25);
						v19.SetTickFrame(v25.Time);
						v20.SetTickFrame(v20.Time - v21.Time);
						v19.SetTickFrame(v19.Time - v21.Time);
						if (v19.Time < 0) {
							XiDateTime ey2(5184000);
							v19.SetTickFrame(v19.Time + ey2.Time);
						}
						if (v20.Time < 0) {
							XiDateTime ey2(5184000);
							v20.SetTickFrame(v20.Time + ey2.Time);
						}
						if (v20.Time > v19.Time) {
							this->FindSetPrevSound();
							this->FindSetNextSound();
						}
					}
				}
			}
		}
	}

	if (this->PrevWeather) {
		if (this->NextWeather != this->PrevWeather) {
			XiDateTime v7{}, v8{}, v19{}, v20{}, v21{};
			XiDateTime::GetWeatherTimeNow(&v20);
			this->PrevWeather->GetActivateTime(&v7);
			this->NextWeather->GetActivateTime(&v8);
			v21.SetTickFrame(v7.Time);
			v19.SetTickFrame(v8.Time);
			v20.SetTickFrame(v20.Time - v21.Time);
			v19.SetTickFrame(v19.Time - v21.Time);
			if (v19.Time < 0) {
				XiDateTime a2(5184000);
				v19.SetTickFrame(a2.Time + v19.Time);
			}
			if (v20.Time < 0) {
				XiDateTime a2(5184000);
				v20.SetTickFrame(a2.Time + v20.Time);
			}
			if (v20.Time > v19.Time) {
				this->FindSetPrevWeather();
				this->FindSetNextWeather();
			}
		}
	}
	else {
		this->FindSetPrevWeather();
		this->FindSetNextWeather();
	}

	if (this->PrevWeather && this->NextWeather) {
		this->ReleaseCurrentWeather();
		if (this->PrevWeather == this->NextWeather) {
			this->field_20 = &this->PrevWeather->field_30;
		}
		else {
			this->field_20 = YmCombineWeather::CreateCurrentWeather();
			XiDateTime v19{}, v20{}, prev{}, next{};
			this->PrevWeather->GetActivateTime(&prev);
			this->NextWeather->GetActivateTime(&next);
			if (next.Time >= prev.Time) {
				XiDateTime v21(next.Time - prev.Time);
				v20.SetTickFrame(v21.Time);
			}
			else {
				XiDateTime v21(5184000);
				v21.SetTickFrame(v21.Time - prev.Time);
				XiDateTime v100(v21.Time + next.Time);
				v20.SetTickFrame(v100.Time);
			}

			if (v22.Time >= prev.Time) {
				XiDateTime v100(v22.Time - prev.Time);
				v19.SetTickFrame(v100.Time);
			}
			else {
				XiDateTime v21(5184000);
				v21.SetTickFrame(v21.Time - prev.Time);
				XiDateTime v100(v22.Time + v21.Time);
				v19.SetTickFrame(v100.Time);
			}

			float fl{ 0.0 };
			if (v20.Time)
				fl = (double)v19.Time / (double)v20.Time;

			this->field_20->Combine(&this->NextWeather->field_30, &this->PrevWeather->field_30, fl);
		}
	}
}

void FFXI::CYy::YmCombineWeather::ReleaseCurrentWeather()
{
	if (this->field_20 && (this->field_20->field_0 & 4) != 0)
		CYyObject::Unwrap((char*)this->field_20);
	this->field_20 = nullptr;
}

FFXI::XiWeather* FFXI::CYy::YmCombineWeather::CreateCurrentWeather()
{
	void* mem = CYyObject::Wrap(sizeof(XiWeather), FFXI::Constants::Enums::MEM_MODE::Elem);
	if (mem) {
		XiWeather* weather = (XiWeather*)mem;
		weather->field_0 |= 4;
		return weather;
	}
	return nullptr;
}

void FFXI::CYy::YmCombineWeather::FindSetPrevWeather()
{
	CMoWeather* weather = this->FindPrevWeather(this->fourCC);
	if (weather)
		this->SetPrevWeather(weather);
}

CMoWeather* FFXI::CYy::YmCombineWeather::FindPrevWeather(int)
{
	XiDateTime v11{}, v12(1866240000), v15{};
	XiDateTime::GetWeatherTimeNow(&v11);
	CMoWeather* v4{ nullptr }, * v5{ nullptr };

	ResourceList v18{};
	v18.Clear();
	v18.PrepareFromResource(this->WeatherFile, FFXI::Constants::Enums::ResourceType::Weather, 0, -1);
	CMoWeather* next = (CMoWeather*)v18.GetNext(false);
	if (!next)
		return nullptr;

	while (next) {
		next->GetActivateTime(&v15);
		if (v15.Time <= v11.Time) {
			XiDateTime v13(v11.Time - v15.Time);
			if (v13.Time < v12.Time) {
				v12.SetTickFrame(v13.Time);
				v5 = next;
			}
		}
		v4 = next;
		next = (CMoWeather*)v18.GetNext(false);
	}

	if (v5)
		return v5;

	return v4;
}

void FFXI::CYy::YmCombineWeather::SetPrevWeather(CMoWeather* a2)
{
	this->PrevWeather = a2;
}

void FFXI::CYy::YmCombineWeather::FindSetNextWeather()
{
	CMoWeather* weather = this->FindNextWeather(this->fourCC);
	if (weather)
		this->SetNextWeather(weather);
}

CMoWeather* FFXI::CYy::YmCombineWeather::FindNextWeather(int)
{
	XiDateTime v14{}, v15(1866240000), v16(1866240000), v19{}, v20{};
	XiDateTime::GetWeatherTimeNow(&v14);
	ResourceList v24{};
	v24.Clear();
	v24.PrepareFromResource(this->WeatherFile, FFXI::Constants::Enums::ResourceType::Weather, 0, -1);
	CMoWeather* next = (CMoWeather*)v24.GetNext(false);
	if (next == nullptr)
		return nullptr;

	CMoWeather* v4{ nullptr }, *v5 { nullptr };
	while (next) {
		int time = v15.Time;
		next->GetActivateTime(&v19);
		if (v19.Time < time) {
			v15.SetTickFrame(v19.Time);
			v5 = next;
		}
		if (v19.Time > v14.Time) {
			XiDateTime v17(v19.Time - v14.Time);
			if (v17.Time < v16.Time) {
				v16.SetTickFrame(v17.Time);
				v4 = next;
			}
		}
		next = (CMoWeather*)v24.GetNext(false);
	}
	if (v4)
		return v4;
	return v5;
}

void FFXI::CYy::YmCombineWeather::SetNextWeather(CMoWeather* a2)
{
	this->NextWeather = a2;
}

void FFXI::CYy::YmCombineWeather::FindSetPrevSound()
{
	CYySepRes** sound = this->FindPrevSound(this->fourCC);
	if (sound && *sound) {
		this->SetPrevSound(sound);
		return;
	}
	this->field_35 = 1;
}

CYySepRes** FFXI::CYy::YmCombineWeather::FindPrevSound(int)
{
	XiDateTime v18{}, v19(1866240000);
	XiDateTime::GetWeatherTimeNow(&v18);

	CYySepRes* v6{ nullptr },* v20{ nullptr };
	if (this->WeatherFile) {
		CYyResourceFile** file{ nullptr };
		this->WeatherFile->AnotherResourceSearcher((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, 'odni');
		CYyResourceFile* v5{ nullptr };
		if (file)
			v5 = *file;

		ResourceList v27{};
		v27.Clear();
		v27.PrepareFromResource(this->WeatherFile, FFXI::Constants::Enums::ResourceType::Sep, 0, -1);
		CYySepRes* next = (CYySepRes*)v27.GetNext(false);
		while (next) {
			CMoResource* v10{ nullptr };
			if (next->Header.RPLHead)
				v10 = *next->Header.RPLHead;

			if (v10 != v5 || XiZone::zone->PlaceCode) {
				if (!v5 || v10 == v5 || !XiZone::zone->PlaceCode) {
					if (next->CheckFourCC()) {
						XiDateTime v24{};
						next->GetActivateTime(&v24);
						if (v24.Time < v18.Time) {
							XiDateTime v22(v18.Time - v24.Time);
							if (v22.Time < v19.Time) {
								v19.SetTickFrame(v22.Time);
								v20 = next;
							}
						}
						v6 = next;
					}
				}
			}
			next = (CYySepRes*)v27.GetNext(false);
		}
		if (v20)
			return (CYySepRes**)v20->PreHeader.PointerToThisInRPL;
	}
	if (v6)
		return (CYySepRes**)v6->PreHeader.PointerToThisInRPL;

	return nullptr;
}

void FFXI::CYy::YmCombineWeather::SetPrevSound(CYySepRes** a2)
{
	this->PrevSound = a2;
}

void FFXI::CYy::YmCombineWeather::FindSetNextSound()
{
	CYySepRes** sound = this->FindNextSound(this->fourCC);
	if (sound && *sound) {
		this->SetNextSound(sound);
		return;
	}

	this->field_35 = 1;
}

CYySepRes** FFXI::CYy::YmCombineWeather::FindNextSound(int a2)
{
	XiDateTime v21{}, v22(1866240000), v23(1866240000);
	CYySepRes* v5{ nullptr }, * v6{ nullptr }, * v24{ nullptr };
	XiDateTime::GetWeatherTimeNow(&v21);

	if (this->WeatherFile) {
		CYyResourceFile** v7{ nullptr };
		this->WeatherFile->AnotherResourceSearcher((CMoResource***)&v7, FFXI::Constants::Enums::ResourceType::Rmp, 'odni');
		CYyResourceFile* v5{ nullptr };
		if (v7)
			v5 = *v7;

		ResourceList v33{};
		v33.Clear();
		v33.PrepareFromResource(this->WeatherFile, FFXI::Constants::Enums::ResourceType::Sep, 0, -1);
		CYySepRes* next = (CYySepRes*)v33.GetNext(false);
		while (next) {
			CMoResource* v10{ nullptr };
			if (next->Header.RPLHead)
				v10 = *next->Header.RPLHead;
			if (v10 != v5 || XiZone::zone->PlaceCode) {
				if (!v5 || v10 == v5 || !XiZone::zone->PlaceCode) {
					if (next->CheckFourCC()) {
						XiDateTime v28{};
						next->GetActivateTime(&v28);
						if (v28.Time < v22.Time)
							v22.SetTickFrame(v28.Time);
						if (v28.Time > v21.Time) {
							XiDateTime v26(v28.Time - v21.Time);
							if (v26.Time < v23.Time) {
								v23.SetTickFrame(v26.Time);
								v24 = next;
							}
						}
						v6 = next;
					}
				}
			}
			next = (CYySepRes*)v33.GetNext(false);
		}
		if (v24)
			return (CYySepRes**)v24->PreHeader.PointerToThisInRPL;
	}
	if (v6)
		return (CYySepRes**)v6->PreHeader.PointerToThisInRPL;

	return nullptr;
}

void FFXI::CYy::YmCombineWeather::SetNextSound(CYySepRes** a2)
{
	this->NextSound = a2;
}

YmCombineWeather* FFXI::CYy::YmCombineWeather::GetParentWeather()
{
	YmCombineWeather* weat = this->Area->CombineWeather;
	YmCombineWeather* result{ nullptr };
	
	while (weat) {
		if (weat == this)
			break;
		result = weat;
		weat = weat->AreasCombineWeather;
	}

	return result;
}

void FFXI::CYy::YmCombineWeather::UpdateWeather()
{
	this->FindSetPrevSound();
	this->FindSetNextSound();
	this->FindSetPrevWeather();
	this->FindSetNextWeather();
}
