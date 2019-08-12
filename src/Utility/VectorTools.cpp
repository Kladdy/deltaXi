#include "VectorTools.h"

bool VectorTools::contains(stringvector vector, std::string stringToMatch)
{
	return (std::find(vector.begin(), vector.end(), stringToMatch) != vector.end());
}

vec2f VectorTools::vec2i_to_vec2f(vec2i vector)
{
	return vec2f(vector.x, vector.y);
}

vec2i VectorTools::vec2f_to_vec2i(vec2f vector)
{
	return vec2i(round(vector.x), round(vector.y));
}

sf::Vector2f VectorTools::roundVector2f(sf::Vector2f vector)
{
	return sf::Vector2f(round(vector.x), round(vector.y));
}

vec2f VectorTools::mirrorOverLine(vec2f vector, float gradient)
{
	float k2 = std::pow(gradient, 2);
	return vec2f((vector.x*(1 - k2) + 2*gradient*vector.y) / (k2 + 1), (2*gradient*vector.x + vector.y*(k2 - 1)) / (k2 + 1));
}

vec2f VectorTools::mirrorHorizontal(vec2f vector)
{
	return vec2f(-vector.x, vector.y);
}

float VectorTools::distancePointToLine(vec2f point, float gradient, float interception)
{
	float a = -gradient;
	float b = 1.f;
	float c = -interception;

	return (a*point.x + b*point.y + c) / VectorTools::vectorLength(vec2f(a, b));
}

float VectorTools::distancePointToPoint(vec2f point1, vec2f point2)
{
	return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
}

float VectorTools::distancePointToPoint(vec2f point1, vec2i point2)
{
	return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
}

float VectorTools::distancePointToPoint(vec2i point1, vec2i point2)
{
	return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
}

float VectorTools::dotProduct(vec2f vector1, vec2f vector2)
{
	return (vector1.x*vector2.x + vector1.y*vector2.y);
}

float VectorTools::dotProductNormalize(vec2f vector, vec2f normal)
{
	float normalLength = VectorTools::vectorLength(normal);
	return (vector.x*normal.x + vector.y*normal.y) / normalLength;
}

float VectorTools::vectorLength(vec2f vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

float VectorTools::vectorLength(vec2i vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

float VectorTools::vectorLengthSquared(vec2f vector)
{
	return pow(vector.x, 2) + pow(vector.y, 2);
}

float VectorTools::vectorLengthSquared(vec2i vector)
{
	return pow(vector.x, 2) + pow(vector.y, 2);
}

sf::Vector2f VectorTools::normalizeVector(sf::Vector2f vector)
{
	float length = VectorTools::vectorLength(vector);
	return vector / length;
}