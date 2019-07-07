// DELETED - Saved for future reference

/*#include "ThinLineShape.h"

ThinLineShape::ThinLineShape(vec2f vertex1, vec2f vertex2)
{
	this->vertex1 = vertex1;
	this->vertex2 = vertex2;
	update();
}

std::size_t ThinLineShape::getPointCount() const
{
	Logger::log("getPointCount");
	return 2; // fixed, line only contains 2 points
}

vec2f ThinLineShape::getPoint(std::size_t index) const
{
	Logger::log("getPoint");
	Logger::logTypeLess<std::size_t>(index);
	if (index == 0) return vertex1;
	return vertex2;
}
*/