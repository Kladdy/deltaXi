#include "LanguageController.hpp"

#include "./Languages/template_Language.hpp"
#include "./Languages/en_US.hpp"
#include "./Languages/sv_SE.hpp"

bool LanguageController::setLanguage(std::string languageCode)
{
	std::map<std::string, std::string> strings;

	if (languageCode == "en_US")
		strings = en_US::strings();
	else if (languageCode == "sv_SE")
		strings = sv_SE::strings();
	else {
		Logger::log("Language '" + languageCode + "' not supported.");
		return false;
	}

	globals::translation.updateLocale(languageCode, strings);
	return true;
}

LanguageController::LanguageController()
{
}

LanguageController::~LanguageController()
{
}
