#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include "../Objects/Drawables/DrawableParticle.h"

class ParticleManager
{
	private:
		std::vector<DrawableParticle> particles;
		std::string scene;

		// Physics
		float elapsedTime;
		bool gravityEnabled;
		vec2f gravityVector;

		void addParticle(vec2i pos = vec2i());
		void toggleGravity(bool gravityEnabled);
		void updateKinematics();

	public:
		void updateWindow();
		void mouseClicked(sf::Mouse::Button buttonPressed);
		void keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system);

		ParticleManager(std::string scene = "default", bool gravityEnabled = true, vec2f gravityVector = vec2f(0.f, 1000.f));
};

inline ParticleManager::ParticleManager(std::string scene, bool enableGravity, vec2f gravityVector)
{
	this->scene = scene;
	this->gravityEnabled = enableGravity;
	this->gravityVector = gravityVector;
}

#endif