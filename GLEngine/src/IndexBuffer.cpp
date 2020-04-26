#include "IndexBuffer.h"

#include "Debug.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer() : IBindable(bufID, BindableType::IndexBuffer)
{
	GLE_GL_DEBUG_CALL(glGenBuffers(1, &bufID));
}

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& ind) : IBindable(bufID, BindableType::IndexBuffer), indecies(ind)
{
	GLE_GL_DEBUG_CALL(glGenBuffers(1, &bufID));
	GLE_GL_DEBUG_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufID));
	GLE_GL_DEBUG_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indecies.size(), indecies.data(), GL_STATIC_DRAW));
	GLE_GL_DEBUG_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &bufID);
}

void IndexBuffer::GenerateFromVerticies(int numOfVertices) const
{
	indecies.clear();
	// Not sure if working.
	for (size_t i = 0; i < numOfVertices; i += 4)
	{
		indecies.push_back(0 + i);
		indecies.push_back(1 + i);
		indecies.push_back(2 + i);
		indecies.push_back(2 + i);
		indecies.push_back(3 + i);
		indecies.push_back(0 + i);
	}

	//auto wasBound = IsBound();

	//if (!wasBound)
		GLE_GL_DEBUG_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufID));
	GLE_GL_DEBUG_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indecies.size(), indecies.data(), GL_STATIC_DRAW));
	//if (!wasBound)
		GLE_GL_DEBUG_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::Bind() const
{
	if (IsBound()) return;
	IBindable::Bind();
	GLE_GL_DEBUG_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufID));
}

void IndexBuffer::Unbind() const
{
	if (!IsBound()) return;
	IBindable::Unbind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}