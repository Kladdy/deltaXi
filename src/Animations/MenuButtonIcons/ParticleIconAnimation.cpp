#include "ParticleIconAnimation.hpp"
#include "../../Objects/ParticleSimulator/Particle.h"
#include "PCH.hpp"

namespace
{
	sf::Vector2i windowSize;
	float particleSpeed = 3.0f;
	float rotationRadius;
	int movingParticleIndex = 0;
	int amountParticles;
	std::map<std::string, Particle> particles;
	std::vector<float> angles;
	std::vector<int> priorColors;
	std::string colorPalette = "particles1";

}

void ParticleIconAnimation::addParticle(int index)
{
	std::string key = std::to_string(index);

	float angle = (index - 1.f) / ((float)amountParticles - 1.f) * 2.f * M_PI;
	if (index == 0)
		angle = 1.f / ((float)amountParticles - 1.f) * M_PI;

	auto [ randomColor, colorIndex ] = globals::colorPalette.getRandomColorInPaletteExcludePrior(colorPalette, -1);

	sf::Vector2f pos = sf::Vector2f(cos(angle), sin(angle)) * rotationRadius;
	Particle p = Particle(pos, SimulationConstants::ps_ParticleRadius, randomColor, 1.f);
	particles[key] = p;

	angles.push_back(angle);
	priorColors.push_back(colorIndex);
}

void ParticleIconAnimation::updateAnimation(sf::Vector2f pos)
{
	float elapsedTime = Toolbox::getElapsedTime();
	int i = movingParticleIndex;

	int nextParticleIndex = i + 1;
	if (nextParticleIndex == amountParticles)
		nextParticleIndex = 0;

	std::string thisKey = std::to_string(i);
	std::string nextKey = std::to_string(nextParticleIndex);

	Particle thisParticle = particles[thisKey];
	Particle nextParticle = particles[nextKey];

	if (VectorTools::distancePointToPoint(thisParticle.getPos(), nextParticle.getPos()) <= thisParticle.radius + nextParticle.radius)
	{
		movingParticleIndex = nextParticleIndex;

		auto [ randomColor, colorIndex ] = globals::colorPalette.getRandomColorInPaletteExcludePrior(colorPalette, priorColors[nextParticleIndex]);
		particles[nextKey].circleShape.setFillColor(randomColor);
		priorColors[nextParticleIndex] = colorIndex;

	}

	angles[i] += particleSpeed * elapsedTime;

	for (int index = 0; index < amountParticles; index++)
	{
		std::string key = std::to_string(index);
		particles[key].setPos(pos + sf::Vector2f(cos(angles[index]), sin(angles[index])) * rotationRadius);
		globals::mainWindow.inst.draw(particles[key].circleShape);
	}
}

ParticleIconAnimation::ParticleIconAnimation(int particleAmount)
{
	amountParticles = particleAmount;
	particles.clear();

	rotationRadius = globals::windowSize.y / 15;

	for (int i = 0; i < amountParticles; i++) // Add particles in a for-loop
	{
		addParticle(i);
	}
}

ParticleIconAnimation::ParticleIconAnimation()
{
}

ParticleIconAnimation::~ParticleIconAnimation()
{
}
