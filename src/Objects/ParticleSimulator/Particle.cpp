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

vec2f Particle::getVelocity()
{
	return this->velocity;
}

void Particle::setVelocity(vec2f velocity)
{
	this->velocity = velocity;
}

float Particle::getMass()
{
	return pow(this->radius, 2);
}

void Particle::setColor(sf::Color color)
{
	this->circleShape.setFillColor(color);
}

Particle::Particle(vec2f pos, int radius, sf::Color color, float elasticityModule)
{
	this->pos = pos;
	this->radius = radius;
	this->elasticityModule = elasticityModule;

	this->circleShape = sf::CircleShape(radius, SimulationConstants::ps_circlePointCount);
	this->circleShape.setFillColor(color);
	this->circleShape.setOrigin(radius, radius);
	this->circleShape.setPosition(pos);
}