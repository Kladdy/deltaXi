#include "ParticleManager.h"
#include "PCH.hpp"

void ParticleManager::updateWindow()
{
	this->elapsedTime = Toolbox::getElapsedTime();

	updateKinematics();

	for(size_t i = 0; i < this->particles.size(); i++)
	{
		if (particles[i].isVisible && VectorTools::contains(particles[i].enlistedScenes, this->scene))
		{
			globals::mainWindow.inst.draw(particles[i].particle.circleShape);
		}
	}

	return;
}

void ParticleManager::mouseClicked(sf::Mouse::Button buttonPressed)
{
	if (buttonPressed == sf::Mouse::Left)
	{
		if (globals::mousePos.x > 400)
		{
			this->addParticle(globals::mousePos);
		}
	}

	return;
}

void ParticleManager::keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system)
{
	if (key == sf::Keyboard::A && control == true && alt == true && shift == true && system == true)
		Logger::log("Keyboard error (you are a maniac!)");

	return;
}

void ParticleManager::addParticle(vec2i pos)
{
	stringvector scenes { "default" };
	DrawableParticle drawableParticle = DrawableParticle(scenes, true, vec2f(pos.x, pos.y));
	if (this->gravityEnabled) drawableParticle.particle.acceleration_offset += this->gravityVector;
	this->particles.push_back(drawableParticle);
	return;
}

void ParticleManager::toggleGravity(bool gravityEnabled)
{
	if (this->gravityEnabled != gravityEnabled){
		this->gravityEnabled = gravityEnabled;
		for (size_t i = 0; i < this->particles.size(); i++)
		{
			Logger::log(std::to_string(i));
			particles[i].particle.acceleration_offset = this->gravityVector;
		}
	}
}

void ParticleManager::updateKinematics()
{

	for (size_t i = 0; i < this->particles.size(); i++)
	{
		Particle particle = particles[i].particle;
		particles[i].particle.velocity += (particle.acceleration + particle.acceleration_offset) * this->elapsedTime;
		particles[i].particle.setPos(particle.getPos() + particles[i].particle.velocity * this->elapsedTime);
	}
}

ParticleManager::ParticleManager(std::string scene, bool enableGravity, vec2f gravityVector)
{
	this->scene = scene;
	this->gravityEnabled = enableGravity;
	this->gravityVector = gravityVector;
}