#ifndef PARTICLE_H
#define PARTICLE_H

#include "PCH.hpp"
#include "SimulationConstants.h"

class Particle
{
	private:
		vec2f pos;
		vec2f velocity;

	public:
		int radius;
		float elasticityModule;
		vec2f acceleration;
		vec2f acceleration_offset;
		sf::CircleShape circleShape;

		vec2f getPos();
		void setPos(vec2f pos);
		vec2f getVelocity();
		void setVelocity(vec2f velocity);

		Particle(vec2f pos = vec2f(0.f,0.f), int radius = SimulationConstants::ps_ParticleRadius, sf::Color color = SimulationConstants().ps_ParticleColor, float elasticityModule = 0.8f);
};

#endif