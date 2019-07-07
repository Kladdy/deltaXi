#ifndef VECTORTOOLS_H
#define VECTORTOOLS_H

class VectorTools
{
	private:

	public:
		// General
		static bool contains(stringvector vector, std::string stringToMatch);
		static vec2f vec2i_to_vec2f(vec2i vector);
		static vec2i vec2f_to_vec2i(vec2f vector);

		// Mathematics
		static vec2f mirrorOverLine(vec2f vector, float gradient);
		static vec2f mirrorHorizontal(vec2f vector);
		static float distancePointToLine(vec2f point, float gradient, float interception);
		static float dotProduct(vec2f vector1, vec2f vector2);
		static float dotProductNormalize(vec2f vector, vec2f normalize);
		static float vectorLength(vec2f vector);
		static float vectorLength(vec2i vector);
};

#endif