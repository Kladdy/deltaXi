#include "./BaseClickable.h"

void BaseClickable::onHover()
{
	Logger::log("BaseClickable: onHover");
}

void BaseClickable::onClick()
{
	Logger::log("BaseClickable: onClick");
}

BaseClickable::BaseClickable(stringvector enlistedScenes, bool isActive)
{
	this->isActive = isActive;
	this->enlistedScenes = enlistedScenes;
}

BaseClickable::~BaseClickable()
{
}