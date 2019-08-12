#include "LanguageController.hpp"

#include "./Languages/template_Language.hpp"
#include "./Languages/en_US.hpp"
#include "./Languages/sv_SE.hpp"
#include <locale>

bool LanguageController::setLanguage(std::string languageCode)
{
	std::map<std::string, std::wstring> strings;

	if (languageCode == "en_US")
		strings = en_US::strings();
	else if (languageCode == "sv_SE")
		strings = sv_SE::strings();
	else {
		Logger::log("Language '" + languageCode + "' not supported.");
		return false;
	}

	// Set locale for wcout to print unicode correctly
	// TODO: Change methods depending on OS-type
	std::locale::global(std::locale("en_US.UTF-8"));
	std::wcout.imbue(std::locale());

	globals::translation.updateLocale(languageCode, strings);

	return true;
}

LanguageController::LanguageController()
{
}

LanguageController::~LanguageController()
{
}
