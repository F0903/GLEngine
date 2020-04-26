#pragma once
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class Renderer
{
	public:
	
	static void Draw(const Shader& shader, const IndexBuffer& indecies, const VertexBuffer& vertices);

	static void Screenshot(const char* path = 0);
};