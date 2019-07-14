#ifndef BASECLICKABLE_H
#define BASECLICKABLE_H

class BaseClickable
{
private:

public:
	bool isActive;
	stringvector enlistedScenes;

	void onHover();
	void onClick();

	BaseClickable(stringvector enlistedScenes, bool isActive = true);
	~BaseClickable();
};

#endif // !BASECLICKABLE_H