#ifndef PARTICLE_H
#define PARTICLE_H

#include "PCH.hpp"
#include "SimulationConstants.h"

class Particle
{
	private:
		vec2f pos;

	public:
		int radius;
		vec2f velocity;
		vec2f acceleration;
		vec2f acceleration_offset;
		sf::CircleShape circleShape;

		vec2f getPos();
		void setPos(vec2f pos);
		Particle(vec2f pos = vec2f(0.f,0.f), int radius = SimulationConstants::ps_ParticleRadius, sf::Color color = SimulationConstants().ps_ParticleColor);
};

inline Particle::Particle(vec2f pos, int radius, sf::Color color)
{
	this->pos = pos;
	this->radius = radius;

	this->circleShape = sf::CircleShape(radius, SimulationConstants::ps_circlePointCount);
	this->circleShape.setFillColor(color);
	this->circleShape.setOrigin(radius, radius);
	this->circleShape.setPosition(pos);
}

#endif