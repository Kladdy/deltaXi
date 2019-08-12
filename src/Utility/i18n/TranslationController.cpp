#include "TranslationController.hpp"
#include "PCH.hpp"

namespace
{
	std::string currentLocale;
	std::map<std::string, std::wstring> localeStrings;
}

void TranslationController::updateLocale(std::string locale, std::map<std::string, std::wstring> strings)
{
	currentLocale = locale;
	localeStrings = strings;

	Logger::log(localeStrings["updateLocale_success"]);
}

std::wstring TranslationController::translate(std::string key)
{
	std::wstring string = localeStrings[key];

	if (string == L"")
	{
		Logger::log("Invalid key: " + key);
		return L"INVALID_KEY=" + TextTools::stringTOwstring(key);
	}

	return string;
}

TranslationController::TranslationController()
{
}

TranslationController::~TranslationController()
{
}