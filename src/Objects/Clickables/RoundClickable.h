#ifndef ROUNDCLICKABLE_H
#define ROUNDCLICKABLE_H

#include "./BaseClickable.h"

class RoundClickable : public BaseClickable
{
private:

public:
	bool isHovered();
	bool isClicked();
	void onHover();
	void onClick();

	//bool isHovered;

	RoundClickable(stringvector enlistedScenes, bool isActive = true);
	~RoundClickable();
};

#endif // !ROUNDCLICKABLE_H
