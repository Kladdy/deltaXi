#include "DrawableParticle.h"
#include "Objects/ParticleSimulator/Particle.h"

DrawableParticle::DrawableParticle(stringvector enlistedScenes, bool isVisible, vec2f pos, int radius, sf::Color color)
{
	this->enlistedScenes = enlistedScenes;
	this->isVisible = isVisible;

	this->particle = Particle(pos, radius, color);
}