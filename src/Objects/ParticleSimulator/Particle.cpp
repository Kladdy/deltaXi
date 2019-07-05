#include "Particle.h"

vec2f Particle::getPos()
{
	return pos;
}

void Particle::setPos(vec2f pos)
{
	this->pos = pos;
	return circleShape.setPosition(pos);
}

