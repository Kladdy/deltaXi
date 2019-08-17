//Class written by Overdrivr: https://github.com/Overdrivr
#include "RoundedRectangleShape.hpp"

RoundedRectangleShape::RoundedRectangleShape(const sf::Vector2f& size, float radius, unsigned int cornerPointCount)
{
	mySize = size;
	myRadius = radius;
	myCornerPointCount = cornerPointCount;
	update();
}

void RoundedRectangleShape::setSize(const sf::Vector2f& size)
{
	mySize = size;
	update();
}

const sf::Vector2f& RoundedRectangleShape::getSize() const
{
	return mySize;
}

void RoundedRectangleShape::setCornersRadius(float radius)
{
	myRadius = radius;
	update();
}

float RoundedRectangleShape::getCornersRadius() const
{
	return myRadius;
}

void RoundedRectangleShape::setCornerPointCount(unsigned int count)
{
	myCornerPointCount = count;
	update();
}

std::size_t RoundedRectangleShape::getPointCount() const
{
	return myCornerPointCount * 4;
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const
{
	if (index >= myCornerPointCount * 4)
		return sf::Vector2f(0, 0);

	float deltaAngle = 90.0f / (myCornerPointCount - 1);
	sf::Vector2f center;
	unsigned int centerIndex = index / myCornerPointCount;
	unsigned int offset = 0;
	static const float pi = 3.141592654f;

	switch (centerIndex)
	{
	case 0: center.x = mySize.x - myRadius; center.y = myRadius; break;
	case 1: center.x = myRadius; center.y = myRadius; break;
	case 2: center.x = myRadius; center.y = mySize.y - myRadius; break;
	case 3: center.x = mySize.x - myRadius; center.y = mySize.y - myRadius; break;
	}

	return sf::Vector2f(myRadius*cos(deltaAngle*(index - centerIndex)*pi / 180) + center.x,
		-myRadius * sin(deltaAngle*(index - centerIndex)*pi / 180) + center.y);
}