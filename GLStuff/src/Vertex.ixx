module;
#include <vector>
export module Vertex;
import SizedPtr;

export struct Vertex
{
	float x, y, z;
	struct
	{
		float s, t;
	} texCoords;
};