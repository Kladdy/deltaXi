#ifndef DRAWABLEPARTICLE_H
#define DRAWABLEPARTICLE_H

#include "PCH.hpp"
#include "Objects/ParticleSimulator/Particle.h"

class DrawableParticle
{
	private:

	public:
		Particle particle;
		bool isVisible;
		stringvector enlistedScenes;

		DrawableParticle(stringvector enlistedScenes, bool isVisible = true, vec2f pos = vec2f(0.f,0.f), int radius = SimulationConstants::ps_ParticleRadius, sf::Color color = SimulationConstants().ps_ParticleColor);
};

inline DrawableParticle::DrawableParticle(stringvector enlistedScenes, bool isVisible, vec2f pos, int radius, sf::Color color)
{
	this->enlistedScenes = enlistedScenes;
	this->isVisible = isVisible;

	this->particle = Particle(pos, radius, color);
}


#endif