#include "XiZone.h"
#include "CTsZoneMap.h"
#include "XiWeather.h"
#include "CMoWeather.h"
#include "Globals.h"
#include "StorageLists.h"
#include "CYyDb.h"
#include "CYyTexMng.h"
#include "CYyTex.h"
#include "CMoDx.h"
#include "SomeVertexThing.h"
#include "CDx.h"
#include "CYySoundElem.h"
#include "CYyCamMng2.h"
#include "CGcMainSys.h"
#include "CYyResourceFile.h"
#include "CMoResourceMng.h"
#include "CYyGenerator.h"
#include "CYyScheduler.h"
#include "StatusNode.h"
#include "BaseProcessor.h"
#include "CXiActor.h"
#include "BlendStruct.h"
#include "FVF42Vertex.h"
#include <iostream>
using namespace FFXI::CYy;

const CYyClass XiZone::XiZoneClass{
	"XiZone", sizeof(XiZone), &CYyObject::CYyObjectClass
};
XiZone* XiZone::zone{ nullptr };
float XiZone::SomeFloat{ -1.0 };

DWORD RenderStates[] = {
	D3DRS_ALPHABLENDENABLE, false,
	D3DRS_FOGENABLE, false,
	D3DRS_LIGHTING, false,
	D3DRS_CULLMODE, D3DCULL_CCW,
	D3DRS_ZWRITEENABLE, false,
	D3DRS_FORCE_DWORD, false
};

DWORD TextureStageStates[] = {
	0, D3DTSS_COLOROP, D3DTOP_SELECTARG1,
	0, D3DTSS_COLORARG1, D3DTA_DIFFUSE,
	0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1,
	0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE,
	1, D3DTSS_COLOROP, D3DTOP_DISABLE,
	1, D3DTSS_ALPHAOP, D3DTOP_DISABLE,
	0, D3DTSS_FORCE_DWORD, false
};

int skydelay{ 0 };

void ClearActors() {
	CXiActor* actor = CXiActor::GetHead();
	while (actor != nullptr) {
		delete actor;
		actor = CXiActor::GetHead();
	}
}

//sub //TODO
//this func needs to be compared to client values to see if it's functioning the same
void DrawCelestialSphere(float a1, int a2, int a3, FFXI::Util::BlendStruct* a4, float* a5) {
	FFXI::Util::BlendStruct temp1{}, temp2{}, temp3{}, temp4{};

	for (int i = 0; i < (a2 - 1); ++i) {
		int count = (long long)((a5[1 + i] - a5[i]) * 12.048193);
		if (count == 0)
			count = 1;

		for (int j = 0; j < count; ++j) {
			float v48 = (double)j / count;
			float va5 = (double)(j + 1) / count;
			double v12 = 1.0 - v48;
			double v13 = 1.0 - va5;


			a4[i + 0].Scale(&temp1, v12);
			a4[i + 1].Scale(&temp2, v48);
			a4[i + 0].Scale(&temp3, v13);
			a4[i + 1].Scale(&temp4, va5);

			temp1.Offset(&temp2);
			temp3.Offset(&temp4);

			float angle = (v12 * a5[i] + v48 * a5[1 + i]) * FFXI::Constants::Values::ANGLE_PI_OVER_2;
			float v49 = cos(angle) * a1;
			float v34 = -(sin(angle) * a1);

			float angle2 = (v13 * a5[i] + va5 * a5[1 + i]) * FFXI::Constants::Values::ANGLE_PI_OVER_2;
			
			va5 = cos(angle2) * a1;
			float v35 = -(sin(angle2) * a1);
			
			CMoDx::svt1->UpdateSubValues(2 * (a3 + 1));
			FVF42Vertex* buf = (FVF42Vertex*)CMoDx::svt1->LockBuffer();
			buf[0].X = 0.0;
			buf[0].Y = v34;
			buf[0].Z = v49;
			buf[0].DiffuseColor = *(D3DCOLOR*)&temp1.field_0;

			buf[1].X = 0.0;
			buf[1].Y = v35;
			buf[1].Z = va5;
			buf[1].DiffuseColor = *(D3DCOLOR*)&temp3.field_0;

			buf += 2;
			for (int k = 1; k < a3; ++k) {
				double angle3 = (double)k * (double)FFXI::Constants::Values::ANGLE_2PI / (double)a3;
				
				buf[0].X = v49 * sin(angle3);
				buf[0].Y = v34;
				buf[0].Z = v49 * cos(angle3);
				buf[0].DiffuseColor = *(D3DCOLOR*)(&temp1.field_0);

				buf[1].X = va5 * sin(angle3);
				buf[1].Y = v35;
				buf[1].Z = va5 * cos(angle3);
				buf[1].DiffuseColor = *(D3DCOLOR*)(&temp3.field_0);

				buf += 2;
			}

			buf[0].X = 0.0;
			buf[0].Y = v34;
			buf[0].Z = v49;
			buf[0].DiffuseColor = *(D3DCOLOR*)(&temp1.field_0);

			buf[1].X = 0.0;
			buf[1].Y = v35;
			buf[1].Z = va5;
			buf[1].DiffuseColor = *(D3DCOLOR*)(&temp3.field_0);
			CMoDx::svt1->UnlockBuffer();
			if (skydelay <= 20) {
				skydelay += 1;
			}
			else {
				CMoDx::svt1->Render(0, 2 * a3, D3DPT_TRIANGLESTRIP, 0, -1);
			}
		}
	}
}
void DrawSky() {
	CMoDx* modx = FFXI::CYyDb::g_pCYyDb->g_pCMoDx;
	BaseProcessor* proc = FFXI::CYyDb::g_pCYyDb->pMoProcessor;
	FFXI::XiWeather* weather = XiZone::zone->Weather;

	if (modx == nullptr)
		return;

	if (weather == nullptr)
		return;

	if ((int)weather == -76)
		return;

	FFXI::CYyDb::g_pCYyDb->SetBackColor(weather->world.field_0, weather->world.field_1, weather->world.field_2);
	if (weather->world.field_12 <= 2)
		return;

	static StatusNode SkyState{};
	SkyState.ApplyRSandTSS(RenderStates, TextureStageStates);
	FFXI::Math::WMatrix* one{ nullptr }, * two{ nullptr };
	proc->PopFromStack(&one);
	proc->PopFromStack(&two);

	*one = modx->field_B50;
	*two = modx->field_D50;

	modx->SetWorldTransform(two);
	modx->SetWorldTransform(&proc->field_810);
	modx->PushViewTransform();
	
	one->_41 = 0.0;	one->_42 = 0.0;	one->_43 = 0.0;	one->_44 = 1.0;
	
	modx->SetViewTransform(one);

	FFXI::Util::BlendStruct bsarr[8]{};
	unsigned int* uintptr = (unsigned int*)weather->world.field_20;
	for (int i = 0; i < sizeof(bsarr) / sizeof(bsarr[0]); ++i) {
		bsarr[i].MixSet(uintptr[i]);
	}

	float v17 = FFXI::CYyDb::g_pCYyDb->field_2E0 - FFXI::CYyDb::g_pCYyDb->field_2DC;
	v17 *= 0.80000001;
	DrawCelestialSphere(v17, 8, weather->world.field_12, bsarr, &weather->world.field_40);
	
	FFXI::CYyDb::g_pCYyDb->SetBackColor(bsarr[0].field_2, bsarr[0].field_1, bsarr[0].field_0);
	FFXI::CYy::CDx::instance->field_178 = 128.0;
	
	modx->PopViewTransform();

	modx->field_EBC->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	modx->field_EBC->SetRenderState(D3DRS_LIGHTING, true);
	modx->field_EBC->SetRenderState(D3DRS_FOGENABLE, false);
	modx->field_EBC->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	modx->field_EBC->SetRenderState(D3DRS_ZWRITEENABLE, true);

	proc->PushToStack(two);
	proc->PushToStack(one);
}

FFXI::CYy::XiZone::XiZone()
{
	skydelay = 0;
	this->field_1C4 = 0;
	this->field_1C8.SetTickFrame(-1);
	this->field_1CE = 0;
	this->MinimumDrawDistance = 43.0;
	this->field_1D4 = 0;
	this->CurrentArea = this;
	this->field_1AC = 0;
	this->field_4 = 0;
	XiWeather::default_weather.field_8 = 0;
	XiWeather::default_weather.field_0 &= 0xFFFFFFFC;
	
	memset(XiWeather::default_weather.env2.field_0, 0x80, sizeof(XiWeather::default_weather.env2.field_0));
	XiWeather::default_weather.env1.field_10 = 100.0;
	XiWeather::default_weather.env1.field_14 = 80.0;
	XiWeather::default_weather.env1.field_18 = 1.0;

	XiWeather::default_weather.env2 = XiWeather::default_weather.env1;
	
	XiWeather::default_weather.world.field_0 = 0;
	XiWeather::default_weather.world.field_1 = 0;
	XiWeather::default_weather.world.field_2 = 0;
	XiWeather::default_weather.world.field_3 = 0x80;
	XiWeather::default_weather.world.field_4 = 80.0;
	XiWeather::default_weather.world.field_8 = 80.0;
	XiWeather::default_weather.world.field_C = 100.0;
	XiWeather::default_weather.world.field_10 = 1;
	XiWeather::default_weather.world.field_11 = 1;
	XiWeather::default_weather.world.field_12 = 0;
	XiWeather::default_weather.world.field_14 = 0x80808080;
	XiWeather::default_weather.world.field_18 = 0;

	this->SetCurrentWeather(&XiWeather::default_weather);
	this->SetWeatherTable(0);
}

FFXI::CYy::XiZone::~XiZone()
{
	this->Close();
}

const CYyClass* FFXI::CYy::XiZone::GetRuntimeClass()
{
	return &XiZone::XiZoneClass;
}

void FFXI::CYy::XiZone::VObj2(int* a2)
{
	if (a2[0] != 2) {
		if (a2[0] != 5)
			return;
	}

	if (XiZone::zone) {
		delete XiZone::zone;
		XiZone::zone = nullptr;
	}
}

void FFXI::CYy::XiZone::SetAction(int a2, CXiActor* a3, CXiActor* a4)
{
	CMoResource** unused{ nullptr };
	if (this->field_1AC) {
		CYyScheduler** sched = *(CYyScheduler***)this->field_1AC->FindResourceUnder(&unused, FFXI::Constants::Enums::ResourceType::Scheduler, a2);
		if (sched && *sched)
			(*sched)->Execute(a3, a4, 0, nullptr);
	}

}

void FFXI::CYy::XiZone::SysInit()
{
	if (zone) return;
	char* mem = StorageLists::instance->Get(sizeof(XiZone), Constants::Enums::MEM_MODE::Ex);
	if (mem)
		zone = new (mem) XiZone();
	else
		zone = nullptr;
}

void FFXI::CYy::XiZone::Open(int a1)
{
	std::cout << "XI ZONE OPEN START: " << a1 << std::endl;
	//sub //TODO
	//globals set
	if (a1 < 100 || a1 >= 540)
		zone->field_1CE = 1;
	if (a1 == 193)
		zone->MinimumDrawDistance = 0.0;
	else if (a1 == 50)
		zone->field_1D4 = 0.7f;

	XiZone::SomeFloat = -1.0;

	if (a1 >= 702 && a1 <= 703)
		CTsZoneMap::SetSomeFlag2(true);
	else
		CTsZoneMap::SetSomeFlag2(false);

	zone->ZoneID = a1;
	zone->SetWeatherTable(0);
	CXiActor::InitActorStatics();
	XiZone::SetFile(a1);
	CYyCamMng2::ResetCameraInitTimer();
	CYyDb::g_pCYyDb->g_pTsZoneMap->WeatherData.LoadAll(XiZone::zone);
}

void FFXI::CYy::XiZone::SetWeatherTable(int a2)
{
	if (a2 < sizeof(XiWeather::WeatherTables) / sizeof(XiWeather::WeatherTables[0]))
		this->WeatherTable = XiWeather::WeatherTables[a2];
	else
		this->WeatherTable = XiWeather::WeatherTables[0];
}

void FFXI::CYy::XiZone::SetPlaceCode(int a2)
{
	this->PlaceCode = a2;
}

void FFXI::CYy::XiZone::SetCurrentArea(XiArea* a2)
{
	this->CurrentArea = a2;
}

float FFXI::CYy::XiZone::GetDrawDistance(bool a2)
{
	float v9 = 100.0;
	float v10 = this->GetAnotherSomething(a2);
	if (this->WorldEnvironment)
		v9 = this->GetWorldC();
	float result = v9 * v10;
	if (this->MinimumDrawDistance <= 0.0)
		return v9;
	if (result < this->MinimumDrawDistance)
		return this->MinimumDrawDistance;

	return result;
}

void FFXI::CYy::XiZone::SysMove()
{
	if (XiZone::zone == nullptr) 
		return;

	XiArea::OnMove();

	if (this->Weather != nullptr)
		DrawSky();

	if (this->CurrentArea != nullptr) {
		if (this->CurrentArea->field_20 != nullptr) {
			CYySoundElem::SetZoneSound(*this->CurrentArea->field_20);
		}
		else {
			CYySoundElem::SetZoneSound(nullptr);
		}
	}
}

void FFXI::CYy::XiZone::SetFile(int a2)
{
	zone->ZoneID = a2;
	
	if ((((short)FFXI::Network::CGcMainSys::gcZoneFlag() >> 8) & 1) == 0 && a2 >= 700) {
		int v2 = a2 - 444;
		if (v2 >= 260 && v2 <= 262) {
			zone->field_1CF = 1;
		}
	}
	else
		zone->field_1CF = 0;

	zone->Close();

	if (CYyDb::pCYyTexMng)
		CYyDb::pCYyTexMng->field_10 = 1;
	CYyTex::SomeFlag = 1;

	CYyResourceFile* zoneDat = CYyDb::g_pCYyDb->g_pTsZoneMap->LoadZoneFile(a2);
	zone->InitWeather(zoneDat);

	if (CYyDb::pCYyTexMng)
		CYyDb::pCYyTexMng->field_10 = 0;
	CYyTex::SomeFlag = 0;
	
	CYyCamMng2* cammng = CYyDb::g_pCYyDb->CameraManager;
	cammng->SomeInit();

	zone->SetPlaceCode(0);
	zone->SetCurrentArea(zone);
	zone->SetAction('ini@', nullptr, nullptr);
}

void FFXI::CYy::XiZone::Close()
{
	ClearActors();
	this->ClearCombines();
	CYyDb::g_pCYyDb->g_pTsZoneMap->CleanSubstructs();
	this->DeleteCombineWeather();
	zone->field_1C8.SetTickFrame(-1);
	zone->field_1CC = 0;
	CYyDb::g_pCYyDb->g_pTsZoneMap->ClearZone();
	zone->field_1AC = 0;
	if (CYyDb::g_pCYyDb) {
		CYyDb::g_pCYyDb->field_330 = 1.0;
		CYyDb::g_pCYyDb->field_32E = 0x80;
		CYyDb::g_pCYyDb->field_32D = 0x80;
		CYyDb::g_pCYyDb->field_32C = 0x80;
		CYyDb::g_pCYyDb->field_32F = 0;
	}

}

void FFXI::CYy::XiZone::InitWeather(CYyResourceFile* a2)
{
	CMoResource** unused{ nullptr };
	CMoWeather** weather{ nullptr };
	this->field_1AC = a2;
	this->field_14 = a2;
	if (!a2) return;

	if (!CYyDb::g_pCYyDb->field_9) {
		weather = *(CMoWeather***)a2->FindResourceUnder(&unused, FFXI::Constants::Enums::ResourceType::Weather, 0);
		if (weather && *weather)
			XiWeather::SetDefault(&(*weather)->field_30);
		else
			XiWeather::SetDefault(nullptr);
	}
	CYyGenerator** gen = *(CYyGenerator***)CMoResourceMng::FindNextUnder(&unused, this->field_1AC, FFXI::Constants::Enums::ResourceType::Generater, 'taew');
	while (gen && *gen) {
		(*gen)->Unlink(0);
		CMoResource** next = (*gen)->PreHeader.NextRPL;
		CMoResource* res = next ? *next : nullptr;
		gen = *(CYyGenerator***)CMoResourceMng::FindNextUnder(&unused, res, FFXI::Constants::Enums::ResourceType::Generater, 0);
	}

	CYyDb::g_pCYyDb->g_pTsZoneMap->InitWeather(zone->field_1AC);
	zone->SetCurrentArea(zone);
}
