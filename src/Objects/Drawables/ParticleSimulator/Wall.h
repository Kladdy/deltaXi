#ifndef WALL_H
#define WALL_H

#include "PCH.hpp"
#include "../BaseDrawable.h"
#include "../Global/Shapes/ThinLineShape.h"

class Wall : public BaseDrawable
{
	private:
		vec2f vertex1;
		vec2f vertex2;

	public:
		sf::VertexArray line;
		float gradient;
		float interception;
		bool isHorizontal;

		float getXValue();
		void updateWall(vec2f vertex1, vec2f vertex2, sf::Color color = SimulationConstants().ps_wallColor);

		Wall(stringvector enlistedScenes, bool isVisible = true, vec2f vertex1 = vec2f(10, 10), vec2f vertex2 = vec2f(100, 200), sf::Color color = SimulationConstants().ps_wallColor);
};

#endif