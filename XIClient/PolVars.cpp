#include "PolVars.h"
#include "CommandLineConfig.h"
#include "Enums.h"
FFXI::PolVars FFXI::PolVars::instance{};

void FFXI::PolVars::ParseCLA()
{
	char param[256]{};
	if (FFXI::Config::CommandLineConfig::instance.ArgGetParam("-lang", param, sizeof(param), 0) != nullptr) {
		int lang_index = atoi(param);
		if (lang_index == Constants::Enums::LanguageCode::English
			|| lang_index == Constants::Enums::LanguageCode::Japanese
			|| lang_index == Constants::Enums::LanguageCode::German
			|| lang_index == Constants::Enums::LanguageCode::French) {
			FFXI::PolVars::instance.RegionCode = lang_index;
		}
		else {
			MessageBox(NULL, L"-lang argument invalid parameter", L"Error", NULL);
			exit(-1);
		}
	}
}
