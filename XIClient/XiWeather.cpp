#include "XiWeather.h"
#include "Globals.h"
#include "CYyDb.h"
#include <string.h>
#include "XiZone.h"
#include "KzFQuat.h"
using namespace FFXI;

const int WeatherTable1[] = {
	'enif', 'ynus', 'dolc', 'tsim', 'wyrd', 'taeh', 'niar',
	'luqs', 'tsud', 'dnas', 'dniw', 'mots', 'wons', 'zilb',
	'rdht', 'tlob', 'arua', 'tgil', 'dgof', 'krad'
};
const int WeatherTable2[] = {
	'1nif', '1nus', '1olc', '1sim', '1yrd', '1aeh', '1iar',
	'1uqs', '1sud', '1nas', '1niw', '1ots', '1ons', '1ilb',
	'1dht', '1lob', '1rua', '1gil', '1gof', '1rad'
};
XiWeather XiWeather::default_weather{};
const int* XiWeather::WeatherTables[2] = { WeatherTable1, WeatherTable2 };

void FFXI::XiWeather::SetDefault(XiWeather* a1)
{
	CYyDb* mng = CYyDb::g_pCYyDb;
	if (a1 == nullptr) {
		mng->SetBackColor(128, 128, 128);
		mng->SetFarColor(128, 128, 128);
		mng->SetField300(128.0);
		mng->SetField2FC(128.0);
	}
	else {
		XiWeather::default_weather = *a1;
		mng->SetBackColor(
			default_weather.world.field_0, 
			default_weather.world.field_1, 
			default_weather.world.field_2);
		mng->SetFarColor(
		default_weather.env2.field_0[12],
		default_weather.env2.field_0[13],
		default_weather.env2.field_0[14]);

		mng->SetField300(default_weather.env2.field_10);
		mng->SetField2FC(default_weather.env2.field_14);
		if (CYy::XiZone::zone) {
			CYy::XiZone::zone->DeleteCombineWeather();
			CYy::XiArea* head = CYy::XiArea::GetHead();
			while (head) {
				head->SetCurrentWeather(&default_weather);
				head = head->field_4;
			}
		}
	}
}

FFXI::XiWeather::XiWeather()
{
	memset(this, 0, sizeof(XiWeather));
	this->field_0 |= 4;
	memset(this->env2.field_0, 0x80, sizeof(this->env2.field_0));
	this->env2.field_10 = 400.0;
	this->env2.field_14 = 40.0;
	
	this->env1 = this->env2;

	this->world.field_0 = 0x80;
	this->world.field_1 = 0x80;
	this->world.field_2 = 0x80;
	this->world.field_3 = 0x80;
	this->world.field_10 = 0;
	this->world.field_11 = 0;
	this->world.field_4 = 400.0;
	this->world.field_8 = 1.0;
	this->world.field_C = 400.0;
}

FFXI::XiWeather::~XiWeather()
{
}

void FFXI::XiWeather::Combine(XiWeather* a2, XiWeather* a3, float a4)
{
	this->field_0 ^= ((unsigned char)this->field_0 ^ (unsigned char)(a3->field_0 | a2->field_0)) & 1;
	this->env1.Combine(&a2->env1, &a3->env1, a4);
	this->env2.Combine(&a2->env2, &a3->env2, a4);
	if ((this->field_0 & 1) != 0) {
		D3DXVECTOR3 v17{};
		v17.x = a3->env2.field_0[4];
		v17.y = a3->env2.field_0[5];
		v17.z = a3->env2.field_0[6];
		Globals::Vec3Normalize(&v17);

		D3DXVECTOR3 v18{};
		v18.x = a2->env2.field_0[4];
		v18.y = a2->env2.field_0[5];
		v18.z = a2->env2.field_0[6];
		Globals::Vec3Normalize(&v18);

		FFXI::Math::KzFQuat v19{};
		v19.SetQuatFromVector(&v17, &v18, a4);

		FFXI::Math::WMatrix v20{};
		v19.GetMatrix(&v20);
		v20.Vec3TransformDrop4Self(&v17);

		this->env2.field_0[4] = v17.x * 127.0;
		this->env2.field_0[5] = v17.y * 127.0;
		this->env2.field_0[6] = v17.z * 127.0;
		
		v17.x = a3->env1.field_0[4];
		v17.y = a3->env1.field_0[5];
		v17.z = a3->env1.field_0[6];
		Globals::Vec3Normalize(&v17);

		v18.x = a2->env1.field_0[4];
		v18.y = a2->env1.field_0[5];
		v18.z = a2->env1.field_0[6];
		Globals::Vec3Normalize(&v18);

		v19.SetQuatFromVector(&v17, &v18, a4);
		v19.GetMatrix(&v20);
		v20.Vec3TransformDrop4Self(&v17);

		this->env1.field_0[4] = v17.x * 127.0;
		this->env1.field_0[5] = v17.y * 127.0;
		this->env1.field_0[6] = v17.z * 127.0;
	}

	//I don't understand this
	if ((int)this != -76 && (int)a2 != -76 && (int)a3 != -76)
		this->world.Combine(&a2->world, &a3->world, a4);
}
