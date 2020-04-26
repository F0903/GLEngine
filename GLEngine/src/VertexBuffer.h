#pragma once
#include <vector>

#include "Vertex.h"
#include "IBindable.h"

class VertexBuffer : public IBindable
{
	private:
	unsigned int bufID;

	std::vector<Vertex> vertices;

	void Init();

	public:
	VertexBuffer(const std::vector<Vertex>& verts);
	VertexBuffer(std::initializer_list<Vertex> init);
	~VertexBuffer();

	inline unsigned int VertexCount() const { return vertices.size(); }

	void Bind() const override;
	void Unbind() const override;
};