#include "VertexBuffer.h"

#include "Debug.h"
#include <GL/glew.h>

void VertexBuffer::Init()
{
	GLE_GL_DEBUG_CALL(glGenBuffers(1, &bufID));
	GLE_GL_DEBUG_CALL(glBindBuffer(GL_ARRAY_BUFFER, bufID));
	GLE_GL_DEBUG_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));
	GLE_GL_DEBUG_CALL(glEnableVertexAttribArray(0));
	GLE_GL_DEBUG_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos)));
	GLE_GL_DEBUG_CALL(glEnableVertexAttribArray(1));
	GLE_GL_DEBUG_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord)));
	GLE_GL_DEBUG_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::VertexBuffer(const std::vector<Vertex>& verts) : IBindable(bufID, BindableType::VertexBuffer), vertices(verts)
{
	Init();
}

VertexBuffer::VertexBuffer(std::initializer_list<Vertex> init) : IBindable(bufID, BindableType::VertexBuffer), vertices(init)
{
	Init();
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &bufID);
}

void VertexBuffer::Bind() const
{
	if (IsBound()) return;
	IBindable::Bind();
	GLE_GL_DEBUG_CALL(glBindBuffer(GL_ARRAY_BUFFER, bufID));
}

void VertexBuffer::Unbind() const
{
	if (!IsBound()) return;
	IBindable::Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}