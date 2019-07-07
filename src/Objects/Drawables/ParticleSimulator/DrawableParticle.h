#ifndef DRAWABLEPARTICLE_H
#define DRAWABLEPARTICLE_H

#include "PCH.hpp"
#include "Objects/ParticleSimulator/Particle.h"
#include "../BaseDrawable.h"

class DrawableParticle : public BaseDrawable
{
	private:

	public:
		Particle particle;

		DrawableParticle(stringvector enlistedScenes, bool isVisible = true, vec2f pos = vec2f(0.f,0.f), int radius = SimulationConstants::ps_ParticleRadius, sf::Color color = SimulationConstants().ps_ParticleColor, float elasticityModule = 0.8f);
};

#endif