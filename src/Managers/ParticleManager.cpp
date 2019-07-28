#include "ParticleManager.h"
#include "PCH.hpp"
#include "../Animations/Smootherstep.hpp"

namespace
{
	// Wall animation
	Smootherstep wallAnimationSmootherstep;
	float wallAnimationDuration = 1.f;
	float currentWallScale = 1.f;
	float wallAnimationScale = 1.f;
	bool isAnimatingWallScaling = false;
	sf::Vector2f particleBoxCenter;

}


void ParticleManager::update()
{
	this->elapsedTime = Toolbox::getElapsedTime();

	if (isAnimatingWallScaling)
		scaleWall();

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

	if (key == sf::Keyboard::Num1 || key == sf::Keyboard::Num2 || key == sf::Keyboard::Num3 || key == sf::Keyboard::Num4)
	{
		float scale = 0.f;
		switch (key){
			case sf::Keyboard::Num1:
				scale = 1 / 1.f;
				break;

			case sf::Keyboard::Num2:
				scale = 1 / 2.f;
				break;

			case sf::Keyboard::Num3:
				scale = 1 / 3.f;
				break;

			case sf::Keyboard::Num4:
				scale = 1 / 4.f;
				break;

			default:
				scale = currentWallScale;
		}

		if (scale != currentWallScale && !isAnimatingWallScaling)
		{
			wallAnimationSmootherstep = Smootherstep(wallAnimationDuration, currentWallScale, scale);
			wallAnimationSmootherstep.retartAnimation();
			isAnimatingWallScaling = true;
			wallAnimationScale = scale;
		}

	}

	if (key == sf::Keyboard::A && control == true && alt == true && shift == true && system == true)
		Logger::log("Keyboard error (you are a maniac!)");

	return;
}

void ParticleManager::addParticle(vec2f pos, vec2f velocity)
{
	stringvector scenes { "default" };
	sf::Color randomColor = globals::colorPalette.getRandomColorInPalette("particles1");
	DrawableParticle drawableParticle = DrawableParticle(scenes, true, pos, SimulationConstants::ps_ParticleRadius, randomColor, 1.f);
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

void ParticleManager::scaleWall()
{
	auto [ scale, animationIsComplete ] = wallAnimationSmootherstep.getAnimationState();
	int vertexOffset = std::round(scale * this->wallSideLength / 2);
	float x = particleBoxCenter.x;
	float y = particleBoxCenter.y;

	walls[0].updateWall(vec2f(x - vertexOffset, y - vertexOffset), vec2f(x + vertexOffset, y - vertexOffset));
	walls[1].updateWall(vec2f(x + vertexOffset, y - vertexOffset), vec2f(x + vertexOffset, y + vertexOffset));
	walls[2].updateWall(vec2f(x + vertexOffset, y + vertexOffset), vec2f(x - vertexOffset, y + vertexOffset));
	walls[3].updateWall(vec2f(x - vertexOffset, y + vertexOffset), vec2f(x - vertexOffset, y - vertexOffset));

	if (animationIsComplete){
		isAnimatingWallScaling = false;
		currentWallScale = wallAnimationScale;
	}
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

std::tuple<sf::Vector2f, sf::Vector2f> ParticleManager::ExtrapolatePositionsUponCollision(sf::Vector2f pos1, sf::Vector2f pos2, sf::Vector2f v1, sf::Vector2f v2, float totalRadius)
{
	const float timeResolution = Toolbox::getElapsedTime() / SimulationConstants::ps_extrapolationPrecision;
	sf::Vector2f pos1_coll;
	sf::Vector2f pos2_coll;

	for (int i = 0; i < SimulationConstants::ps_extrapolationPrecision; i++)
	{
		pos1_coll = pos1 - v1 * (timeResolution * i);
		pos2_coll = pos2 - v2 * (timeResolution * i);
		float distance = VectorTools::distancePointToPoint(pos1_coll, pos2_coll);
		if (distance >= totalRadius)
			return std::make_tuple(pos1_coll, pos2_coll);

	}

	// Collision point not found, taking default as previous positions
	return std::make_tuple(pos1, pos2);
}

std::tuple<sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f> ParticleManager::ParticleOnParticleCollision(Particle p1, Particle p2)
{
	// https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects

	// The positions should be the respective positions at the collision point
	sf::Vector2f pos1 = p1.getPos();
	sf::Vector2f pos2 = p2.getPos();

	sf::Vector2f v1 = p1.getVelocity();
	sf::Vector2f v2 = p2.getVelocity();

	auto [ pos1_coll, pos2_coll ]  = this->ExtrapolatePositionsUponCollision(pos1, pos2, v1, v2, p1.radius + p1.radius);

	float m1 = p1.getMass();
	float m2 = p2.getMass();

	sf::Vector2f posDifference = pos1_coll - pos2_coll;

	float massFactor = -2.f / (m1 + m2);
	float dotProductFactor = VectorTools::dotProduct(v1 - v2, posDifference) / VectorTools::vectorLengthSquared(posDifference);

	sf::Vector2f v1_upd = v1 + (m2 * massFactor) * (dotProductFactor * posDifference);
	sf::Vector2f v2_upd = v2 + (m1 * massFactor) * (dotProductFactor * -1 * posDifference);

	return std::make_tuple(pos1_coll, pos2_coll, v1_upd, v2_upd);
}

void ParticleManager::updateKinematics()
{
	size_t particlesSize = this->particles.size();

	for (size_t i = 0; i < particlesSize; i++)
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

			particles[i].particle.setPos(updatedPos);
			particles[i].particle.setVelocity(updatedVelocity);
		}

		for (size_t j = i + 1; j < particlesSize; j++)
		{
			Particle p1 = particles[i].particle;
			Particle p2 = particles[j].particle;

			if (VectorTools::distancePointToPoint(p1.getPos(), p2.getPos()) <= p1.radius + p2.radius)
			{
				auto [ pos1_upd, pos2_upd, v1_upd, v2_upd ]  = this->ParticleOnParticleCollision(p1, p2);

				particles[i].particle.setPos(pos1_upd);
				particles[j].particle.setPos(pos2_upd);

				particles[i].particle.setVelocity(v1_upd);
				particles[j].particle.setVelocity(v2_upd);

				float distanceExceeded = p1.radius + p2.radius - VectorTools::distancePointToPoint(pos1_upd, pos2_upd) ;

				if (distanceExceeded > 0.f) // Resolve particles sticking together
				{
					sf::Vector2f deltaPositionVector = VectorTools::normalizeVector(pos2_upd - pos1_upd);
					sf::Vector2f correctionVector = deltaPositionVector * (distanceExceeded / 2);

					particles[i].particle.setPos(pos1_upd - correctionVector);
					particles[j].particle.setPos(pos2_upd + correctionVector);
				}
			}
		}

	}
}

ParticleManager::ParticleManager(std::string scene, bool enableGravity, vec2f gravityVector) : circle(stringvector())
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
	this->wallRotation = 0;

	particleBoxCenter = topLeftCorner + (bottomRightCorner - topLeftCorner) / 2.f;
}