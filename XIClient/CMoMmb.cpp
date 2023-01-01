#define WIN32_LEAN_AND_MEAN
#include "CMoMmb.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CTsZoneMap.h"
#include "d3dx9math.h"
using namespace FFXI::CYy;

int fourcclist[] = {
	'nawa', 'carb', '1loc', 'u-hs', 'mius'
};
FFXI::CYy::CMoMmb::CMoMmb()
{
	this->field_30 = 0;
}

void FFXI::CYy::CMoMmb::Open()
{
	//Not sure if this is a vec4
	D3DXVECTOR4 v7 = { 0, 0, 0, 0 };
	if (CYyDb::g_pCYyDb->g_pTsZoneMap->field_3C == FFXI::Constants::Enums::Zone::Metalworks) {
		for (int i = 0; i < sizeof(fourcclist) / sizeof(fourcclist[0]); ++i) {
			if (this->Header.FourCC == fourcclist[i]) {
				v7.y = -0.8f;
				break;
			}
		}
	}

	 *(void**)(&this->field_30) = (void*)CYyDb::g_pCYyDb->g_pTsZoneMap->OpenMmb((CYyResourceFile**)this->Header.RPLHead, &this->field_30, this->Header.FourCC, (this->Header.Type_Size >> 3) & 0x7FFFF0, &v7);
}
