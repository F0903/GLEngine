module;
#include <Gl/glew.h>
#include "OpenGL/gl_util.h"
export module VertexArray;
import VertexBuffer;
import IndexBuffer;

export struct VertexAttribute
{
	unsigned int dataMembers;
	GLenum dataMemberType;
	GLboolean normalized;
	int stride;
};

export class VertexArray
{
	public:
	VertexArray()
	{
		glGenVertexArrays(1, &id);
	}

	private:
	unsigned int id;
	VertexBuffer vertices;
	IndexBuffer indices;

	public:
	void Bind() const
	{
		glBindVertexArray(id);
	}

	void SetAttribute(const unsigned int num, const VertexAttribute attribute) const
	{
		DEBUG_GL_CHECK();
		glVertexAttribPointer(num, attribute.dataMembers, attribute.dataMemberType, attribute.normalized, attribute.stride, 0);
		glEnableVertexAttribArray(num);
		DEBUG_GL_CHECK();
	}

	VertexBuffer& GetVertexBuffer()
	{
		return vertices;
	}

	IndexBuffer& GetIndexBuffer()
	{
		return indices;
	}
};