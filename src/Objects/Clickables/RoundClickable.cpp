#include "RoundClickable.h"

#include "PCH.hpp"

bool RoundClickable::isHovered()
{
	Logger::log("isHovered!");
	return true;
}

bool RoundClickable::isClicked()
{
	Logger::log("isClicked!");
	return true;
}

void RoundClickable::onHover()
{
	Logger::log("Hover!");
}

void RoundClickable::onClick()
{
	Logger::log("Click!");
}

RoundClickable::RoundClickable(stringvector enlistedScenes, bool isActive) : BaseClickable(enlistedScenes, isActive)
{
	this->isActive = isActive;
	this->enlistedScenes = enlistedScenes;
}

RoundClickable::~RoundClickable()
{
}
