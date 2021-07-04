module;
#include <Gl/glew.h>
#include "OpenGL/gl_util.h"
export module VertexBuffer;
import SizedPtr;
import Vertex;

export struct VertexAttribute
{
	unsigned int dataMembers;
	GLenum dataType;
	GLboolean normalized;
	int totalSize;
};

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
		glBufferData(GL_ARRAY_BUFFER, vertices.ptrSize, vertices.ptr, GL_STATIC_DRAW);
	}

	void SetAttribute(const unsigned int num, const VertexAttribute attribute) const
	{
		DEBUG_GL_CHECK();
		glVertexAttribPointer(num, attribute.dataMembers, attribute.dataType, attribute.normalized, attribute.totalSize, 0);
		glEnableVertexAttribArray(num);
		DEBUG_GL_CHECK();
	}
};