module;
#include <Gl/glew.h>
export module VertexArray;
import VertexBuffer;
import IndexBuffer;

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
		vertices.Bind();
		indices.Bind();
	}
};