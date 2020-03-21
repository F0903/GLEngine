#pragma once
#include <vector>

#include "Vertex.h"

class VertexBuffer
{
	private:
	unsigned int bufID;

	std::vector<Vertex> vertices;

	void Init();

	public:
	VertexBuffer(std::vector<Vertex>& verts);
	VertexBuffer(std::initializer_list<Vertex> init);
	~VertexBuffer();

	inline unsigned int VertexCount() const { return vertices.size(); }

	void Bind() const;
	void Unbind() const;
};