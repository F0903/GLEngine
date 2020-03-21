#pragma once
#include <vector>

#include "Vec2.h"

class IndexBuffer
{
	private:
	unsigned int bufID;

	std::vector<unsigned int> indecies;

	public:
	IndexBuffer();
	IndexBuffer(std::vector<unsigned int> ind);
	~IndexBuffer();

	inline unsigned int IndexCount() const { return indecies.size(); }

	void Bind();
	void Unbind();

	void GenerateFromVerticies(int numOfVertices);
};