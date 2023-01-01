#include "StringTables.h"
#include "DatIndices.h"
#include "Globals.h"
#include "Enums.h"

using namespace FFXI::Text;

char* FFXI::Text::XIStrTable[Constants::Enums::StrTables::XIStr_LAST];

//Not used?
DMsg FFXI::Text::GermanArticles, FFXI::Text::FrenchArticles;

//Part of DMsgTable
DMsg FFXI::Text::JobShortNames, FFXI::Text::ZoneShortNames, FFXI::Text::ZoneFullNames, FFXI::Text::Errors,
FFXI::Text::Menu1, FFXI::Text::Menu2, FFXI::Text::GearSlotNames, FFXI::Text::OdinFightMobs,
FFXI::Text::MiscMenuText, FFXI::Text::CharSelectText, FFXI::Text::CharSelectRaceGender,
FFXI::Text::JobFullNames;

DMsgTableEntry FFXI::Text::DMsgTables[] = {
			{Constants::Enums::StrTables::XIStr_CharSelectText, &CharSelectText},
			{Constants::Enums::StrTables::XIStr_JobShortNames, &JobShortNames},
			{Constants::Enums::StrTables::XIStr_JobFullNames, &JobFullNames},
			{Constants::Enums::StrTables::XIStr_ZoneShortNames, &ZoneShortNames},
			{Constants::Enums::StrTables::XIStr_ZoneFullNames, &ZoneFullNames},
			{Constants::Enums::StrTables::XIStr_CharSelectRaceGender, &CharSelectRaceGender},
			{Constants::Enums::StrTables::XIStr_Errors, &Errors},
			{Constants::Enums::StrTables::XIStr_LobbyHelpText, nullptr},
			{Constants::Enums::StrTables::XIStr_9, nullptr},
			{Constants::Enums::StrTables::XIStr_VariousHelpText, nullptr},
			{Constants::Enums::StrTables::XIStr_11, nullptr},
			{Constants::Enums::StrTables::XIStr_MiscMenuText, &MiscMenuText},
			{Constants::Enums::StrTables::XIStr_Menu1, &Menu1},
			{Constants::Enums::StrTables::XIStr_Menu2, &Menu2},
			{Constants::Enums::StrTables::XIStr_15, nullptr},
			{Constants::Enums::StrTables::XIStr_16, nullptr},
			{Constants::Enums::StrTables::XIStr_17, nullptr},
			{Constants::Enums::StrTables::XIStr_18, nullptr},
			{Constants::Enums::StrTables::XIStr_19, nullptr},
			{Constants::Enums::StrTables::XIStr_20, nullptr},
			{Constants::Enums::StrTables::XIStr_21, nullptr},
			{Constants::Enums::StrTables::XIStr_22, nullptr},
			{Constants::Enums::StrTables::XIStr_23, nullptr},
			{Constants::Enums::StrTables::XIStr_GearSlotNames, &GearSlotNames},
			{Constants::Enums::StrTables::XIStr_OdinFightMobs, &OdinFightMobs},
			{Constants::Enums::StrTables::XIStr_26, nullptr},
			{Constants::Enums::StrTables::XIStr_27, nullptr},
			{Constants::Enums::StrTables::XIStr_28, nullptr}
};

char* FFXI::Text::XiStrGet(int PageIndex, int EntryIndex) {
	int v2 = 0;
	while (DMsgTables[v2].PageIndex != PageIndex)
		++v2;

	if (v2 < FFXI::Constants::Enums::StrTables::XIStr_LASTDMSG) {
		DMsg* TablePointer = DMsgTables[v2].DataPointer;
		if (TablePointer && TablePointer->Buffer)
			return TablePointer->GetEntry0(EntryIndex);
	}

	char* XIStr = XIStrTable[PageIndex];
	if (!XIStr)
		return nullptr;

	return XIString::GetEntry(XIStr, EntryIndex);
}

void FFXI::Text::CleanStrTables()
{
	for (int i = 0; i < FFXI::Constants::Enums::StrTables::XIStr_LAST; ++i) {
		if (XIStrTable[i]) {
			delete[] XIStrTable[i];
			XIStrTable[i] = nullptr;
		}
	}
}

bool LoadLangIndependentTables() {

	if (DMsg::LoadFromFile(&GermanArticles, FFXI::Constants::DatIndices::DMSG_GermanArticles) &&
		DMsg::LoadFromFile(&FrenchArticles, FFXI::Constants::DatIndices::DMSG_FrenchArticles) &&
		DMsg::LoadFromFile(&ZoneFullNames, FFXI::Constants::DatIndices::DMSG_ZoneFullNames) &&
		DMsg::LoadFromFile(&ZoneShortNames, FFXI::Constants::DatIndices::DMSG_ZoneShortNames))
	{
		DMsg::LoadFromFile(&GearSlotNames, FFXI::Constants::DatIndices::DMSG_GearSlotNames);
	}

	if (!DMsg::LoadFromFile(&OdinFightMobs, FFXI::Constants::DatIndices::DMSG_OdinFightMobs))
		return false;

	if (!DMsg::LoadFromFile(&JobShortNames, FFXI::Constants::DatIndices::DMSG_JobShortNames))
		return false;

	return true;

}
bool FFXI::Text::LoadStringTables() {
	if (!LoadLangIndependentTables())
		return false;

	int RegionCode = FFXI::GlobalStruct::g_GlobalStruct.GetRegionCode();
	int Index{};
	if (RegionCode == Constants::Enums::LanguageCode::Japanese) {
		if (!Text::XIString::LoadFromFile(Constants::Enums::StrTables::XIStr_0, 45))
			return false;
		if (!Text::XIString::LoadFromFile(Constants::Enums::StrTables::XIStr_LobbyHelpText, 30))
			return false;
		if (!Text::XIString::LoadFromFile(Constants::Enums::StrTables::XIStr_VariousHelpText, 32))
			return false;
	}
	else if (RegionCode == Constants::Enums::LanguageCode::English) {
		if (!Text::XIString::LoadFromFile(Constants::Enums::StrTables::XIStr_0, 63))
			return false;
		if (!Text::XIString::LoadFromFile(Constants::Enums::StrTables::XIStr_LobbyHelpText, 48))
			return false;
		if (!Text::XIString::LoadFromFile(Constants::Enums::StrTables::XIStr_VariousHelpText, 50))
			return false;
	}
	else {
		Index = Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::LobbyHelpText);
		if (Text::XIString::LoadFromFile(Constants::Enums::StrTables::XIStr_LobbyHelpText, Index)) {
			Index = Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::VariousHelpText);
			Text::XIString::LoadFromFile(Constants::Enums::StrTables::XIStr_VariousHelpText, Index);
		}
	}

	if (!Text::DMsg::LoadFromFile(&CharSelectRaceGender, Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::CharSelectRaceGender))) return false;
	if (!Text::DMsg::LoadFromFile(&CharSelectText, Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::CharSelectText))) return false;
	if (!Text::DMsg::LoadFromFile(&JobFullNames, Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::JobFullNames))) return false;
	if (!Text::DMsg::LoadFromFile(&Errors, Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::Errors))) return false;
	if (!Text::DMsg::LoadFromFile(&Menu2, Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::MenuText2))) return false;
	if (!Text::DMsg::LoadFromFile(&Menu1, Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::MenuText1))) return false;
	if (!Text::DMsg::LoadFromFile(&MiscMenuText, Globals::GetLanguageDependentDatIndex(FFXI::Constants::Enums::LanguageDependentDats::MiscMenuText))) return false;
	return true;
}

void FFXI::Text::InitStrPages()
{
	memset(XIStrTable, 0, sizeof(XIStrTable));
}
