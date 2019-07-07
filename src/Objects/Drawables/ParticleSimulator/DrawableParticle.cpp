#include "DrawableParticle.h"

DrawableParticle::DrawableParticle(stringvector enlistedScenes, bool isVisible, vec2f pos, int radius, sf::Color color, float elasticityModule)
{
	this->enlistedScenes = enlistedScenes;
	this->isVisible = isVisible;

	this->particle = Particle(pos, radius, color, elasticityModule);
}