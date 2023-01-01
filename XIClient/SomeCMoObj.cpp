#include "SomeCMoObj.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoDx.h"
#include "CDx.h"
using namespace FFXI::CYy;

void FFXI::CYy::SomeCMoObj::Update()
{
	CYyDb::g_pCYyDb->g_pCMoDx->field_DC8->GetTransform(D3DTS_VIEW, &this->field_10);
	this->field_90 = this->field_10;
	this->field_90._43 = 0.0;
	this->field_90._42 = 0.0;
	this->field_90._41 = 0.0;
	CYyDb::g_pCYyDb->g_pCMoDx->field_DC8->GetTransform(D3DTS_PROJECTION, &this->field_50);
}
