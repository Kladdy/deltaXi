#include "VectorTools.h"

bool VectorTools::contains(stringvector vector, std::string stringToMatch)
{
	return (std::find(vector.begin(), vector.end(), stringToMatch) != vector.end());
}