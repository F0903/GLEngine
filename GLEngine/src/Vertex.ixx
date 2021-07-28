module;
#include <vector>
export module Vertex;
import SizedPtr;
import Vector;

export struct Vertex
{
	union
	{
		Vector<3> pos;
		struct
		{
			float x, y, z;
		};
	};

	struct
	{
		float s, t;
	} texCoords;
};