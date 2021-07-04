module;
#include <Gl/glew.h>
#include "OpenGL/gl_util.h"
export module VertexBuffer;
import SizedPtr;
import Vertex;

export class VertexBuffer
{
	public:
	VertexBuffer()
	{
		glGenBuffers(1, &id);
	}

	private:
	unsigned int id;
	SizedPtr<Vertex> vertices{};

	public:
	void Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void SetData(const SizedPtr<Vertex> vertices)
	{
		this->vertices = vertices;
		Bind();
		glBufferData(GL_ARRAY_BUFFER, vertices.length * sizeof(Vertex), vertices.ptr, GL_STATIC_DRAW);
	}
};