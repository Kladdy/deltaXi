#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include "../Objects/Drawables/ParticleSimulator/DrawableParticle.h"
#include "../Objects/Drawables/ParticleSimulator/Wall.h"

class ParticleManager
{
	private:
		std::string scene;
		vec2f wallBuilderVertex;

		// Drawables
		std::vector<DrawableParticle> particles;
		std::vector<Wall> walls;

		// Physics
		float elapsedTime;
		bool gravityEnabled;
		vec2f gravityVector;

		// Walls
		float wallRotation;
		float wallSideLength;
		vec2f baseTopLeftCorner;
		vec2f baseBottomRigthCorner;

		// Add functions
		void addParticle(vec2f pos = vec2f(), vec2f velocity = vec2f());
		void addWall(vec2f vertex1 = vec2f(10, 10), vec2f vertex2 = vec2f(100, 200), sf::Color color = SimulationConstants().ps_wallColor);
		void addRectangleWall(vec2f topLeftCorner, vec2f bottomRightCorner, sf::Color color = SimulationConstants().ps_wallColor);
		bool hasCollied(float initialDistance, float updatedDistance, float particleRadius);
		void rotateWall(float wallRotation);

		void toggleGravity(bool gravityEnabled);
		void updateKinematics();

	public:
		void updateWindow();
		void mouseClicked(sf::Mouse::Button buttonPressed);
		void keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system);

		ParticleManager(std::string scene = "default", bool gravityEnabled = false, vec2f gravityVector = vec2f(0.f, 3700.f));
};

#endif