#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include "../Objects/Drawables/ParticleSimulator/DrawableParticle.h"

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

		ParticleManager(std::string scene = "default", bool gravityEnabled = true, vec2f gravityVector = vec2f(0.f, 3700.f));
};

#endif