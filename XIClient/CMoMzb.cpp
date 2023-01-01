#include "CMoMzb.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CTsZoneMap.h"

FFXI::CYy::CMoMzb::CMoMzb()
{
}

void FFXI::CYy::CMoMzb::Open()
{
	this->DoSomething3();
	if (this->Header.FourCC == 'enuf') return;

	CYyResourceFile* head = (CYyResourceFile*)this->DoSomething3();
	CYyDb::g_pCYyDb->g_pTsZoneMap->OpenMzb(this + 1, head);
}
