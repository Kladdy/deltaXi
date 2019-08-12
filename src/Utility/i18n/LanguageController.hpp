#ifndef LANGUAGECONTROLLER_HPP
#define LANGUAGECONTROLLER_HPP

#include "PCH.hpp"

class LanguageController
{
private:

public:
	static bool setLanguage(std::string languageCode);

	LanguageController();
	~LanguageController();
};

#endif // !LANGUAGECONTROLLER_HPP