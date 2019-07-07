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

	for(size_t i = 0; i < this->walls.size(); i++)
	{
		if (walls[i].isVisible && VectorTools::contains(walls[i].enlistedScenes, this->scene))
		{
			globals::mainWindow.inst.draw(walls[i].line);
		}
	}

	return;
}

void ParticleManager::mouseClicked(sf::Mouse::Button buttonPressed)
{
	if (buttonPressed == sf::Mouse::Left)
	{
		if (globals::mousePos.x > 0)
		{
			int randomVelocity_x, randomVelocity_y, maxVelocity;
			maxVelocity = SimulationConstants::ps_maxRandomVelocity;
			randomVelocity_x = rand() % maxVelocity - maxVelocity / 2;
			randomVelocity_y = rand() % maxVelocity - maxVelocity / 2;
			this->addParticle(VectorTools::vec2i_to_vec2f(globals::mousePos), vec2f(randomVelocity_x, randomVelocity_y));
		}
	}

	return;
}

void ParticleManager::keyPressed(sf::Keyboard::Key key, bool control, bool alt, bool shift, bool system)
{
	if (key == sf::Keyboard::W && this->wallBuilderVertex != vec2f())
	{
		this->addWall(this->wallBuilderVertex, VectorTools::vec2i_to_vec2f(globals::mousePos));
		Logger::log("Added wall");
		this->wallBuilderVertex = vec2f();
		return;
	}

	/*if (key == sf::Keyboard::W)
	{
		this->wallBuilderVertex = VectorTools::vec2i_to_vec2f(globals::mousePos);
		Logger::log("Set vertex1 position");
		return;
	}*/

	if (key == sf::Keyboard::Up)
	{
		if (this->wallRotation < 0.991f)
			this->rotateWall(this->wallRotation + 0.01f);
	}

	if (key == sf::Keyboard::Down)
	{
		if (this->wallRotation > 0.0009f)
			this->rotateWall(this->wallRotation - 0.01f);
	}

	if (key == sf::Keyboard::A && control == true && alt == true && shift == true && system == true)
		Logger::log("Keyboard error (you are a maniac!)");

	return;
}

void ParticleManager::addParticle(vec2f pos, vec2f velocity)
{
	stringvector scenes { "default" };
	DrawableParticle drawableParticle = DrawableParticle(scenes, true, pos, SimulationConstants::ps_ParticleRadius, SimulationConstants().ps_ParticleColor, 1.f);
	if (this->gravityEnabled) drawableParticle.particle.acceleration_offset += this->gravityVector;
	drawableParticle.particle.setVelocity(velocity);
	this->particles.push_back(drawableParticle);
	return;
}

void ParticleManager::addWall(vec2f vertex1, vec2f vertex2, sf::Color color)
{


	if (vertex1 == vertex2) return Logger::logExtra("Both wall verticies are the same: terminated wall creation");

	stringvector scenes { "default" };
	Wall wall = Wall(scenes, true, vertex1, vertex2, color);

	this->walls.push_back(wall);
	return;
}

 void ParticleManager::addRectangleWall(vec2f topLeftCorner, vec2f bottomRightCorner, sf::Color color)
{
	this->addWall(vec2f(topLeftCorner.x, topLeftCorner.y), vec2f(bottomRightCorner.x, topLeftCorner.y), color);
	this->addWall(vec2f(bottomRightCorner.x, topLeftCorner.y), vec2f(bottomRightCorner.x, bottomRightCorner.y), color);
	this->addWall(vec2f(bottomRightCorner.x, bottomRightCorner.y), vec2f(topLeftCorner.x, bottomRightCorner.y), color);
	this->addWall(vec2f(topLeftCorner.x, bottomRightCorner.y), vec2f(topLeftCorner.x, topLeftCorner.y), color);

	return;
}

inline bool sameSign(float a, float b) {
    return a*b >= 0.0f;
}

bool ParticleManager::hasCollied(float initialDistance, float updatedDistance, float particleRadius)
{
	return (std::fabs(initialDistance) < particleRadius || std::fabs(updatedDistance) < particleRadius || !sameSign(initialDistance, updatedDistance));
}

void ParticleManager::rotateWall(float wallRotation)
{
	this->wallRotation = wallRotation;
	float offset = wallRotation * this->wallSideLength;
	vec2f tL = this->baseTopLeftCorner;
	vec2f bR = this-> baseBottomRigthCorner;

	walls[0].updateWall(vec2f(tL.x + offset, tL.y), vec2f(bR.x, tL.y + offset));
	walls[1].updateWall(vec2f(bR.x, tL.y + offset), vec2f(bR.x - offset, bR.y));
	walls[2].updateWall(vec2f(bR.x - offset, bR.y), vec2f(tL.x, bR.y - offset));
	walls[3].updateWall(vec2f(tL.x, bR.y - offset), vec2f(tL.x + offset, tL.y));
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

		vec2f updatedVelocity = particles[i].particle.getVelocity() + (particle.acceleration + particle.acceleration_offset) * this->elapsedTime;
		vec2f updatedPos = particle.getPos() + updatedVelocity * this->elapsedTime;

		for (size_t j = 0; j < this->walls.size(); j++)
		{
			float initialDistance;
			float updatedDistance;
			vec2f normalVector;
			float normalVelocity;
			vec2f mirroredVelocity;

			Wall wall = walls[j];

			if (wall.isHorizontal) {
				initialDistance = particle.getPos().x - wall.getXValue();
				updatedDistance = updatedPos.x - wall.getXValue();
			}
			else {
				initialDistance = VectorTools::distancePointToLine(particle.getPos(), wall.gradient, wall.interception);
				updatedDistance = VectorTools::distancePointToLine(updatedPos, wall.gradient, wall.interception);
			}

			if (hasCollied(initialDistance, updatedDistance, particle.radius))
			{
				if (wall.isHorizontal)
					normalVector = vec2f(1, 0);
				else
					normalVector = vec2f(-wall.gradient, 1);

				normalVelocity = VectorTools::dotProductNormalize(updatedVelocity, normalVector);
				float e1 = (std::fabs(initialDistance) - particle.radius) / std::fabs(normalVelocity);
				float e2 = this->elapsedTime - e1;

				if (wall.isHorizontal)
					mirroredVelocity = VectorTools::mirrorHorizontal(updatedVelocity);
				else
					mirroredVelocity = VectorTools::mirrorOverLine(updatedVelocity, wall.gradient);

				//Logger::log("Updated: " + std::to_string(updatedVelocity.y));
				//Logger::log("Mirrored: " + std::to_string(mirroredVelocity.y));

				updatedPos = particle.getPos() + updatedVelocity * e1 + mirroredVelocity * e2;
				updatedVelocity = particle.elasticityModule * mirroredVelocity;
			}
		}

		particles[i].particle.setPos(updatedPos);
		particles[i].particle.setVelocity(updatedVelocity);
	}
}

ParticleManager::ParticleManager(std::string scene, bool enableGravity, vec2f gravityVector)
{
	this->scene = scene;
	this->gravityEnabled = enableGravity;
	this->gravityVector = gravityVector;

	vec2f topLeftCorner;
	vec2f bottomRightCorner;
	int pixelsFromEdge = SimulationConstants::ps_pixelsFromEdge;
	int heigth = globals::windowSize.y - 2 * pixelsFromEdge;

	bottomRightCorner.x = globals::windowSize.x - pixelsFromEdge;
	bottomRightCorner.y = globals::windowSize.y - pixelsFromEdge;
	topLeftCorner.x = bottomRightCorner.x - heigth;
	topLeftCorner.y = pixelsFromEdge;

	this->addRectangleWall(topLeftCorner, bottomRightCorner, sf::Color::White);
	this->wallSideLength = heigth;
	this->baseTopLeftCorner = topLeftCorner;
	this->baseBottomRigthCorner = bottomRightCorner;
}