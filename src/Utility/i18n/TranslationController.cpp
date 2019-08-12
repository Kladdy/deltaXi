#include "TranslationController.hpp"
#include "PCH.hpp"

namespace
{
	std::string currentLocale;
	std::map<std::string, std::string> localeStrings;
}

void TranslationController::updateLocale(std::string locale, std::map<std::string, std::string> strings)
{
	currentLocale = locale;
	localeStrings = strings;

	Logger::log(localeStrings["updateLocale_success"]);
}

std::string TranslationController::translate(std::string key)
{
	std::string string = localeStrings[key];

	if (string == "")
	{
		Logger::log("Invalid key: " + key);
		return "INVALID_KEY=" + key;
	}

	return string;
}

TranslationController::TranslationController()
{
}

TranslationController::~TranslationController()
{
}