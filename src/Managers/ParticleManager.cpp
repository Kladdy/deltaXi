#include "ParticleManager.h"
#include "PCH.hpp"
#include "../Animations/Smootherstep.hpp"
#include "../Objects/Clickables/Global/Dials/Slider.hpp"

namespace
{
	// Texts
	std::map<std::string, sf::Text> leftPanelTexts;
	sf::Vector2f titlePos, titleBoundingBox, infoPos, infoBoundingBox;
	float textDistanceFromLeftWall = 30;
	float textDistanceFromParticleBox = 30;
	float titleHeight = 30;
	float infoHeight = 200;
	float distanceTopToTitle = 20;
	float textDistance = 10;
	sf::RectangleShape lineUnderInfo;
	float lineUnderInfoHeight;
	float lineThickness = 1;

	// Dials
	std::map<std::string, Slider> dialSliders;
	std::map<std::string, sf::Text> dialTexts;
	float sliderDialWidth = 150;
	float dialDistance = 50;
	float sliderDialTextOffset = -3;

	// State picker
	std::string currentState;
	sf::VertexArray statePickerFadeBox;
	std::map<std::string, sf::Text> statePickerTexts;
	float fadeBoxHeight = 100;
	float fadeFraction = 0.40f;
	float stateTextOffset = -10.f;
	Smootherstep stateFadeBoxAnimation;
	std::map<std::string, int> stateStringToIntMap;
	bool isAnimatingStateFadeBox = false;
	float stateFadeBoxAnimationDuration = 0.2f;

	// Stats
	std::map<std::string, sf::Text> statsTexts;
	float statsTextOffset = 20;
	sf::Clock collisionClock;
	std::vector<float> particleToParticleCollisions;
	std::vector<float> particleToWallCollisions;
	int amountPPCollisions = 0;
	int amountPWCollisions = 0;
	std::map<std::string, std::vector<float>> collisionsHistory;
	std::map<std::string, float> currentStats;
	size_t collisionsMaxMeanAmount = 50;

	// Colors
	int currentColorPaletteId = 1;

	// Wall animation
	Smootherstep wallAnimationSmootherstep;
	float wallAnimationDuration = 1.f;
	float currentWallScale = 1.f;
	float wallAnimationScale = 1.f;
	bool isAnimatingWallScaling = false;
	sf::Vector2f particleBoxCenter;

	// Particle removal animation
	std::map<int, std::pair<float, Smootherstep>> deletionClockSmootherStep;
	sf::Color deletedFillColor = sf::Color(204, 59, 59);

	// Extract keys from map
	template<typename TK, typename TV>
	std::vector<TK> extract_keys(std::map<TK, TV> const& input_map) {
		std::vector<TK> retval;
		for (auto const& element : input_map) {
			retval.push_back(element.first);
		}
		return retval;
	}

	bool isCointainedInOffsetRectangle(sf::Vector2f pos, sf::Vector2f topLeft, sf::Vector2f bottomRight, float offset)
	{
		bool isBoundedInX = ( pos.x > topLeft.x + offset && pos.x < bottomRight.x - offset);
		bool isBoundedInY = ( pos.y > topLeft.y + offset && pos.y < bottomRight.y - offset);

		return (isBoundedInX && isBoundedInY);
	}

	float avgOfVector(std::vector<float> v)
	{
		return accumulate( v.begin(), v.end(), 0.0)/v.size();
	}

	void pushBackAndRemoveHistory(std::string key, float value, size_t maxSize)
	{
		collisionsHistory[key].push_back(value);

		if (collisionsHistory[key].size() > maxSize)
			collisionsHistory[key].erase(collisionsHistory[key].begin());
	}

	void updateHistory(std::string key, float value, size_t maxSize, std::wstring textString)
	{
		currentStats[key] = value;
		pushBackAndRemoveHistory(key, value, maxSize);

		currentStats["avg" + key] = avgOfVector(collisionsHistory[key]);
		statsTexts[key].setString(textString + L": " + std::to_wstring(currentStats["avg" + key]));

	}

	std::tuple<sf::Vector2f, sf::Vector2f> getRectangleFromGlobalBounds(sf::FloatRect globalBounds)
	{
		sf::Vector2f topLeft;
		topLeft.x = globalBounds.left;
		topLeft.y = globalBounds.top;

		sf::Vector2f bottomRight;
		bottomRight = topLeft + sf::Vector2f(globalBounds.width, globalBounds.height);

		return std::make_tuple(topLeft, bottomRight);
	}
}


void ParticleManager::update()
{
	this->elapsedTime = Toolbox::getElapsedTime();
	sf::Vector2f mousePos = VectorTools::vec2i_to_vec2f(globals::mousePos);
	auto& window = globals::mainWindow.inst;

	if (isAnimatingWallScaling)
		scaleWall();

	if (!isAnimatingWallScaling) // Only update kinematics if we are not scaling the wall
		updateKinematics();

	if (isAnimatingStateFadeBox)
		updateStateFadeBox();

	updateStats();

	for(size_t i = 0; i < this->particles.size(); i++)
	{
		if (particles[i].isVisible && VectorTools::contains(particles[i].enlistedScenes, this->scene))
		{
			window.draw(particles[i].particle.circleShape);
		}
	}

	for(size_t i = 0; i < this->walls.size(); i++)
	{
		if (walls[i].isVisible && VectorTools::contains(walls[i].enlistedScenes, this->scene))
		{
			window.draw(walls[i].line);
		}
	}

	// Draw left panel texts
	for(auto& [key, value] : leftPanelTexts)
	{
		window.draw(value);
	}

	// Draw info text line
	window.draw(lineUnderInfo);

	// Draw state picker texts and fade box
	for(auto& [key, value] : statePickerTexts)
	{
		auto [topLeft, bottomRight] = getRectangleFromGlobalBounds(value.getGlobalBounds());
		if (isCointainedInOffsetRectangle(mousePos, topLeft, bottomRight, stateTextOffset))
			beginStateFadeBoxAnimation(key);

		window.draw(value);
	}
	window.draw(statePickerFadeBox);

	if (currentState == "dials")
	{
		// Draw dial sliders
		for(auto& [key, value] : dialSliders)
		{
			//value.knobIsHovered();
			value.draw();
		}

		// Draw dial texts
		for(auto& [key, value] : dialTexts)
		{
			window.draw(value);
		}
	}
	else if (currentState == "graphs")
	{
		// Draw stats texts
		for(auto& [key, value] : statsTexts)
		{
			window.draw(value);
		}
	}

	return;
}

void ParticleManager::mouseClicked(sf::Mouse::Button buttonPressed)
{
	sf::Vector2f mousePos = VectorTools::vec2i_to_vec2f(globals::mousePos);

	if (buttonPressed == sf::Mouse::Left)
	{
		if (isCointainedInOffsetRectangle(mousePos, this->baseTopLeftCorner, this->baseBottomRigthCorner, (float)SimulationConstants::ps_ParticleRadius))
		{
			int randomVelocity_x, randomVelocity_y, maxVelocity;
			maxVelocity = SimulationConstants::ps_maxRandomVelocity;
			randomVelocity_x = rand() % maxVelocity - maxVelocity / 2;
			randomVelocity_y = rand() % maxVelocity - maxVelocity / 2;
			this->addParticle(mousePos, vec2f(randomVelocity_x, randomVelocity_y));
		}

		if (currentState == "dials")
		{
			for(auto& [key, value] : dialSliders)
			{
				if(value.isHovered())
				{

					if(key == "containerSize" && !isAnimatingWallScaling)
					{
						value.onClick();
						float scale = 1.f / (float)(SimulationConstants::ps_particleContainerSizeVariations - value.getStateToBeSelected());
						if (scale != currentWallScale)
							beginScaling(scale);
					}
					else if (key == "colors")
					{
						value.onClick();
						int colorPaletteId = value.getStateToBeSelected() + 1;
						if (colorPaletteId != currentColorPaletteId)
							setParticleColor(colorPaletteId);
					}
				}

			}
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

void ParticleManager::addParticle(vec2f pos, vec2f velocity)
{
	if (isAnimatingWallScaling) // Disallow adding of particles if we are scaling the wall
		return;

	stringvector scenes { "default" };
	sf::Color randomColor = globals::colorPalette.getRandomColorInPalette("particles" + std::to_string(currentColorPaletteId));
	DrawableParticle drawableParticle = DrawableParticle(scenes, true, pos, SimulationConstants::ps_ParticleRadius, randomColor, 1.f);
	if (this->gravityEnabled) drawableParticle.particle.acceleration_offset += this->gravityVector;
	drawableParticle.particle.setVelocity(velocity);
	this->particles.push_back(drawableParticle);
	return;
}

void ParticleManager::deleteParticle(int index)
{
	// Check to see if particle is already being deleted
	if (deletionClockSmootherStep.count(index) != 0)
		return;

	// Create smootherstep clock that lasts for the rest of the wall animation
	float elapsedTime = wallAnimationSmootherstep.getElapsedTime();
	std::pair<float, Smootherstep> deletionClock;
	deletionClock.first = elapsedTime;
	deletionClock.second = Smootherstep(wallAnimationDuration - elapsedTime, 255.f, 0.f);
	deletionClockSmootherStep[index] = deletionClock;

	// Set color of the particle to be deleted
	particles[index].particle.circleShape.setFillColor(deletedFillColor);

	if (index != -1)
	{
		//particles[index].particle.circleShape.setFillColor(sf::Color(50, 50, 50));
	}
}

void ParticleManager::updateDeleteParticle()
{
	for (auto& [key, value] : deletionClockSmootherStep)
	{
		auto [ fade, animationIsComplete ] = value.second.getAnimationState();
		sf::Color c = particles[key].particle.circleShape.getFillColor();
		c.a = fade;
		particles[key].particle.circleShape.setFillColor(c);
	}
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

void ParticleManager::beginScaling(float scale)
{
	if (scale != currentWallScale && !isAnimatingWallScaling)
	{
		wallAnimationSmootherstep = Smootherstep(wallAnimationDuration, currentWallScale, scale);
		wallAnimationSmootherstep.retartAnimation();
		isAnimatingWallScaling = true;
		wallAnimationScale = scale;
	}
}

void ParticleManager::beginStateFadeBoxAnimation(std::string state)
{
	if (state != currentState && !isAnimatingStateFadeBox)
	{
		stateFadeBoxAnimation = Smootherstep(stateFadeBoxAnimationDuration, stateStringToIntMap[currentState], stateStringToIntMap[state]);
		wallAnimationSmootherstep.retartAnimation();
		isAnimatingStateFadeBox = true;
		currentState = state;
	}
}

void ParticleManager::scaleWall()
{
	auto [ scale, animationIsComplete ] = wallAnimationSmootherstep.getAnimationState();
	int vertexOffset = std::round(scale * this->wallSideLength / 2);
	float x = particleBoxCenter.x;
	float y = particleBoxCenter.y;

	auto upperLeft = sf::Vector2f(x - vertexOffset, y - vertexOffset);
	auto upperRight = sf::Vector2f(x + vertexOffset, y - vertexOffset);
	auto lowerRight = sf::Vector2f(x + vertexOffset, y + vertexOffset);
	auto lowerLeft = sf::Vector2f(x - vertexOffset, y + vertexOffset);

	walls[0].updateWall(upperLeft, upperRight);
	walls[1].updateWall(upperRight, lowerRight);
	walls[2].updateWall(lowerRight, lowerLeft);
	walls[3].updateWall(lowerLeft, upperLeft);

	for (size_t i = 0; i < particles.size(); i++)
	{
		sf::Vector2f pos = particles[i].particle.getPos();
		float maxOffset = vertexOffset - particles[i].particle.radius;

		bool isBoundedInX = ( pos.x > x - maxOffset && pos.x < x + maxOffset );
		bool isBoundedInY = ( pos.y > y - maxOffset && pos.y < y + maxOffset );
		if (!isBoundedInX || !isBoundedInY)
			deleteParticle(i);
	}

	updateDeleteParticle();

	if (animationIsComplete){
		isAnimatingWallScaling = false;
		currentWallScale = wallAnimationScale;

		this->baseTopLeftCorner = upperLeft;
		this->baseBottomRigthCorner = lowerRight;

		auto keys = extract_keys(deletionClockSmootherStep);

		// Remove all particles that have been faded out
		for (int i = keys.size() - 1; i >= 0 ; i--)
		{
			particles.erase(particles.begin() + keys[i]);
		}

		deletionClockSmootherStep.clear();
	}
}

void ParticleManager::updateStateFadeBox()
{
	auto [ scale, animationIsComplete ] = stateFadeBoxAnimation.getAnimationState();

	auto y = globals::windowSize.y;
	auto totalWidth = this->baseTopLeftCorner.x;
	auto fadeWidth = totalWidth * fadeFraction;
	auto unfadedWidth = totalWidth - 2 * fadeWidth;

	auto midPoint = scale - 2.f;
	auto currentMidPoint = totalWidth / 2.f * (1 + midPoint * 3.f / 5.f);

	auto leftX = currentMidPoint - unfadedWidth / 2;
	auto rightX = currentMidPoint + unfadedWidth / 2;

	statePickerFadeBox[2].position = sf::Vector2f(leftX, y - fadeBoxHeight);
	statePickerFadeBox[3].position = sf::Vector2f(leftX, y);
	statePickerFadeBox[4].position = sf::Vector2f(rightX, y - fadeBoxHeight);
	statePickerFadeBox[5].position = sf::Vector2f(rightX, y);

	if (animationIsComplete){
		isAnimatingStateFadeBox = false;
	}
}

void ParticleManager::toggleGravity(bool gravityEnabled)
{
	if (this->gravityEnabled != gravityEnabled){
		this->gravityEnabled = gravityEnabled;
		for (size_t i = 0; i < this->particles.size(); i++)
		{
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

	particleToParticleCollisions.push_back(collisionClock.getElapsedTime().asSeconds());

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

				particleToWallCollisions.push_back(collisionClock.getElapsedTime().asSeconds());
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

void ParticleManager::updateStats()
{
	if (collisionClock.getElapsedTime().asSeconds() > 0.1f)
	{
		TranslationController transaltionController;

		float timeElapsed = collisionClock.restart().asSeconds();

		int pp = particleToParticleCollisions.size();
		int pw = particleToWallCollisions.size();

		amountPPCollisions += pp;
		amountPWCollisions += pw;

		particleToParticleCollisions.clear();
		particleToWallCollisions.clear();

		statsTexts["P-P"].setString("P-P: " + std::to_string(amountPPCollisions));
		statsTexts["P-W"].setString("P-W: " + std::to_string(amountPWCollisions));

		updateHistory("P-P_per_sec", pp / timeElapsed, collisionsMaxMeanAmount, transaltionController.translate("sim_particle_P-P"));
		updateHistory("P-W_per_sec", pw / timeElapsed, collisionsMaxMeanAmount, transaltionController.translate("sim_particle_P-W"));

		size_t n = particles.size();

		float PPPerSecPerN = currentStats["P-P_per_sec"] / n;
		float PWPerSecPerN = currentStats["P-W_per_sec"] / n;

		pushBackAndRemoveHistory("PP/sn", PPPerSecPerN, collisionsMaxMeanAmount);
		pushBackAndRemoveHistory("PW/sn", PWPerSecPerN, collisionsMaxMeanAmount);

		float avgPPPerSecPerN = avgOfVector(collisionsHistory["PP/sn"]);
		float avgPWPerSecPerN = avgOfVector(collisionsHistory["PW/sn"]);

		statsTexts["P-P_per_sec_per_n"].setString("P-P /sn: " + std::to_string(avgPPPerSecPerN));
		statsTexts["P-W_per_sec_per_n"].setString("P-W /sn: " + std::to_string(avgPWPerSecPerN));

		float PPPerSecPerN2 = PPPerSecPerN / n;
		float PWPerSecPerN2 = PWPerSecPerN / n;

		pushBackAndRemoveHistory("PP/sn2", PPPerSecPerN2, collisionsMaxMeanAmount);
		pushBackAndRemoveHistory("PW/sn2", PWPerSecPerN2, collisionsMaxMeanAmount);

		float avgPPPerSecPerN2 = avgOfVector(collisionsHistory["PP/sn2"]);
		float avgPWPerSecPerN2 = avgOfVector(collisionsHistory["PW/sn2"]);

		statsTexts["P-P_per_sec_per_n2"].setString("P-P /sn^2: " + std::to_string(avgPPPerSecPerN2));
		statsTexts["P-W_per_sec_per_n2"].setString("P-W /sn^2: " + std::to_string(avgPWPerSecPerN2));
	}

}

void ParticleManager::setParticleColor(int colorPaletteId)
{
	for(auto& p : particles)
	{
		sf::Color randomColor = globals::colorPalette.getRandomColorInPalette("particles" + std::to_string(colorPaletteId));
		p.particle.setColor(randomColor);
	}

	currentColorPaletteId = colorPaletteId;
}

void ParticleManager::textSetup()
{
	titlePos = sf::Vector2f(textDistanceFromLeftWall, distanceTopToTitle);
	infoPos = sf::Vector2f(textDistanceFromLeftWall, distanceTopToTitle + textDistance + titleHeight);

	float maxWidth = this->baseTopLeftCorner.x - textDistanceFromLeftWall - textDistanceFromParticleBox;

	titleBoundingBox = sf::Vector2f(maxWidth, titleHeight);
	infoBoundingBox = sf::Vector2f(maxWidth, infoHeight);

	leftPanelTexts["Title"] = TextTools::createTranslatedTextLabel("sim_particles", titleBoundingBox, false, titlePos, titleHeight);
	leftPanelTexts["Info"] = TextTools::createTranslatedTextLabel("sim_particles_infotext", infoBoundingBox, true, infoPos);

	lineUnderInfoHeight = infoPos.y + infoHeight + textDistance;
	lineUnderInfo = sf::RectangleShape(sf::Vector2f(maxWidth, lineThickness));
	lineUnderInfo.setPosition(sf::Vector2f(textDistanceFromLeftWall, lineUnderInfoHeight));
}

void ParticleManager::dialsSetup()
{
	auto dialPos = sf::Vector2f(baseTopLeftCorner.x - textDistanceFromParticleBox - sliderDialWidth, lineUnderInfoHeight);
	std::string dialName = "";

	dialPos.y += dialDistance;
	dialName = "colors";
	dialSliders[dialName] = Slider(SimulationConstants::ps_particleColorVariations, sliderDialWidth, dialPos);
	dialTexts[dialName] = TextTools::createTranslatedTextLabel("sim_particle_dial_" + dialName, sf::Vector2f(textDistanceFromLeftWall, dialPos.y + sliderDialTextOffset));

	dialPos.y += dialDistance;
	dialName = "size";
	dialSliders[dialName] = Slider(SimulationConstants::ps_particleColorVariations, sliderDialWidth, dialPos);
	dialTexts[dialName] = TextTools::createTranslatedTextLabel("sim_particle_dial_" + dialName, sf::Vector2f(textDistanceFromLeftWall, dialPos.y + sliderDialTextOffset));

	dialPos.y += dialDistance;
	dialName = "containerSize";
	dialSliders[dialName] = Slider(SimulationConstants::ps_particleContainerSizeVariations, sliderDialWidth, dialPos, SimulationConstants::ps_particleContainerSizeVariations - 1);
	dialTexts[dialName] = TextTools::createTranslatedTextLabel("sim_particle_dial_" + dialName, sf::Vector2f(textDistanceFromLeftWall, dialPos.y + sliderDialTextOffset));

}

void ParticleManager::statePickerSetup()
{
	currentState = "dials";
	stateStringToIntMap["graphs"] = 1;
	stateStringToIntMap["dials"] = 2;
	stateStringToIntMap["actions"] = 3;

	statePickerFadeBox = sf::VertexArray(sf::TriangleStrip, 8);

	auto y = globals::windowSize.y;
	auto totalWidth = this->baseTopLeftCorner.x;
	auto fadeWidth = totalWidth * fadeFraction;

	statePickerFadeBox[0].position = sf::Vector2f(0, y-fadeBoxHeight);
	statePickerFadeBox[1].position = sf::Vector2f(0,y);
	statePickerFadeBox[2].position = sf::Vector2f(fadeWidth, y-fadeBoxHeight);
	statePickerFadeBox[3].position = sf::Vector2f(fadeWidth, y);
	statePickerFadeBox[4].position = sf::Vector2f(totalWidth - fadeWidth, y-fadeBoxHeight);
	statePickerFadeBox[5].position = sf::Vector2f(totalWidth - fadeWidth, y);
	statePickerFadeBox[6].position = sf::Vector2f(totalWidth, y-fadeBoxHeight);
	statePickerFadeBox[7].position = sf::Vector2f(totalWidth, y);

	statePickerFadeBox[0].color = sf::Color::Black;
	statePickerFadeBox[1].color = sf::Color::Black;
	statePickerFadeBox[2].color = sf::Color::Transparent;
	statePickerFadeBox[3].color = sf::Color::Transparent;
	statePickerFadeBox[4].color = sf::Color::Transparent;
	statePickerFadeBox[5].color = sf::Color::Transparent;
	statePickerFadeBox[6].color = sf::Color::Black;
	statePickerFadeBox[7].color = sf::Color::Black;

	statePickerTexts["graphs"] = TextTools::createTranslatedTextLabel("sim_particle_statepicker_graphs", sf::Vector2f(1.f/5.f * totalWidth, y-fadeBoxHeight / 2), globals::defaultTextSize*1.5, globals::defaultTextFont, true);
	statePickerTexts["dials"] = TextTools::createTranslatedTextLabel("sim_particle_statepicker_dials", sf::Vector2f(1.f/2.f * totalWidth, y-fadeBoxHeight / 2), globals::defaultTextSize*1.5, globals::defaultTextFont, true);
	statePickerTexts["actions"] = TextTools::createTranslatedTextLabel("sim_particle_statepicker_actions", sf::Vector2f(4.f/5.f * totalWidth, y-fadeBoxHeight / 2), globals::defaultTextSize*1.5, globals::defaultTextFont, true);

}

void ParticleManager::statsSetup()
{
	collisionClock.restart();

	auto statsTextPos = sf::Vector2f(0, lineUnderInfoHeight);

	statsTexts["P-P"] = TextTools::createTextLabel(L"P-P: ", sf::Vector2f(textDistanceFromLeftWall, statsTextPos.y + 1 * statsTextOffset));
	statsTexts["P-W"] = TextTools::createTextLabel(L"P-W: ", sf::Vector2f(textDistanceFromLeftWall, statsTextPos.y + 2 * statsTextOffset));
	statsTexts["P-P_per_sec"] =  TextTools::createTextLabel(L"P-P /s: ", sf::Vector2f(textDistanceFromLeftWall, statsTextPos.y + 4 * statsTextOffset));
	statsTexts["P-W_per_sec"] =  TextTools::createTextLabel(L"P-P /s: ", sf::Vector2f(textDistanceFromLeftWall, statsTextPos.y + 5 * statsTextOffset));
	statsTexts["P-P_per_sec_per_n"] =  TextTools::createTextLabel(L"P-P /s n: ", sf::Vector2f(textDistanceFromLeftWall, statsTextPos.y + 7 * statsTextOffset));
	statsTexts["P-W_per_sec_per_n"] =  TextTools::createTextLabel(L"P-P /s n: ", sf::Vector2f(textDistanceFromLeftWall, statsTextPos.y + 8 * statsTextOffset));
	statsTexts["P-P_per_sec_per_n2"] =  TextTools::createTextLabel(L"P-P /s n^2: ", sf::Vector2f(textDistanceFromLeftWall, statsTextPos.y + 10 * statsTextOffset));
	statsTexts["P-W_per_sec_per_n2"] =  TextTools::createTextLabel(L"P-P /s n^2: ", sf::Vector2f(textDistanceFromLeftWall, statsTextPos.y + 11 * statsTextOffset));
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

	textSetup();

	dialsSetup();

	statePickerSetup();

	statsSetup();

	particleBoxCenter = topLeftCorner + (bottomRightCorner - topLeftCorner) / 2.f;
}