#ifndef TRANSLATIONCONTROLLER_HPP
#define TRANSLATIONCONTROLLER_HPP

class TranslationController
{
private:

public:
	void updateLocale(std::string locale, std::map<std::string, std::wstring> strings);
	std::wstring translate(std::string key);

	TranslationController();
	~TranslationController();
};

#endif // !TRANSLATIONCONTROLLER_HPP
