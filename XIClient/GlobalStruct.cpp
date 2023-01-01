#include "GlobalStruct.h"
using namespace FFXI;
GlobalStruct FFXI::GlobalStruct::g_GlobalStruct{};
int FFXI::GlobalStruct::GetRegionCode()
{
	int rc = this->RegionCode;
	char check = *(char*)&this->field_18;

	if (check < 0) {
		if (rc == Constants::Enums::LanguageCode::French ||
			rc == Constants::Enums::LanguageCode::German)
			rc = Constants::Enums::LanguageCode::English;
	}

	return rc;
}

bool FFXI::GlobalStruct::Check10Flag(int a2)
{
	return ((1 << a2) & this->ClientExpansions) != 0;
}

bool FFXI::GlobalStruct::Check14Flag(int a2)
{
	return ((1 << a2) & this->ServerExpansions) != 0;
}
