#include "CGcMainSys.h"
#include "PlaceHolders.h"


using namespace FFXI::Network;

int CGcMainSys::gcZoneFlag()
{
	if (Placeholder::pGlobalNowZone)
		throw "DO THIS";
	return 0;
}

void CGcMainSys::gcMainDivisionInit()
{
	int fieldSize = sizeof(this->field_15110);
	for (int i = 0; i < fieldSize; ++i)
		this->field_15110[i] = 32;
}

void CGcMainSys::gcFriendInit(int a1)
{
	this->FriendUpdateCycle = a1;
	this->field_130 = -1;
	this->field_128 = 0;
	this->field_808 = 0;
	this->field_5F0 = 0;
	this->field_5EC = 0;
	this->field_3D4 = 0;
	this->field_5E8 = 0;
	this->field_5DC = 0;
	this->field_804 = 0;
	this->field_7F8 = 0;
	this->field_80C = 0;
	this->gcFriendSearchClear();
}

void CGcMainSys::gcFriendSearchClear()
{
	this->field_1C4 = 0;
	this->field_3C8 = 0;
}
