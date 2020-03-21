#include "IndexBuffer.h"
#include <GL/glew.h>

bool bound;

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &bufID);
}

IndexBuffer::IndexBuffer(std::vector<unsigned int> ind) : indecies(ind)
{
	glGenBuffers(1, &bufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indecies.size(), indecies.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &bufID);
}

void IndexBuffer::GenerateFromVerticies(int numOfVertices)
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

	if (!bound)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indecies.size(), indecies.data(), GL_STATIC_DRAW);
	if (!bound)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufID);
	bound = true;
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	bound = false;
}