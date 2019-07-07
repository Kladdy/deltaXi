#include "Wall.h"

float Wall::getXValue()
{
	if (this->isHorizontal)
		return this->vertex1.x;
	else
		Logger::log("Wall: Cannot get x-value - wall is not horizontal");
		return (float)INFINITY;
}

void Wall::updateWall(vec2f vertex1, vec2f vertex2, sf::Color color)
{
	this->vertex1 = vertex1;
	this->vertex2 = vertex2;

	vec2f delta = vertex2 - vertex1;
	this->gradient = delta.y / delta.x;
	this->interception = vertex1.y - this->gradient*vertex1.x;

	this->line = sf::VertexArray(sf::Lines, 0);
	this->line.append(sf::Vertex(vertex1, color));
	this->line.append(sf::Vertex(vertex2, color));

	this->isHorizontal = (fabs(this->gradient) == (float)INFINITY);
}

Wall::Wall(stringvector enlistedScenes, bool isVisible, vec2f vertex1, vec2f vertex2, sf::Color color)
{
	this->enlistedScenes = enlistedScenes;
	this->isVisible = isVisible;

	this->updateWall(vertex1, vertex2, color);
}