module;
#include <Gl/glew.h>
#include <initializer_list>
#include "util.h"
#include "OpenGL/gl_util.h"
export module Renderer;
import Shader;
import Texture;
import Vertex;
import RenderSize;
import VertexArray;

export class Renderer
{
	private:
	static inline Shader* currentShader;
	static inline Texture* currentTexture;
	static inline VertexArray* vertexArray;

	public:
	static void Init()
	{
		vertexArray = new VertexArray();
	}

	static void Free()
	{
		delete vertexArray;
	}

	static void SetShader(Shader* shader)
	{
		currentShader = shader;
		currentShader->Use();
	}

	static void SetTexture(Texture* tex)
	{
		currentTexture = tex;
		currentTexture->Bind();
		currentTexture->Free();
	}

	static void Draw(const SizedPtr<Vertex> vertices, const SizedPtr<unsigned int> indices)
	{
		vertexArray->Bind();
		auto& vert = vertexArray->GetVertexBuffer();
		auto& ind = vertexArray->GetIndexBuffer();

		vert.SetData(vertices);
		ind.SetData(indices);
		vertexArray->SetAttribute(0, { 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 }); // Position
		vertexArray->SetAttribute(1, { 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoords) }); // Texture coords
		DEBUG_GL_CHECK();

		currentTexture->Bind();
		currentShader->Use();
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, ind.GetDataSize(), GL_UNSIGNED_INT, 0);
		DEBUG_GL_CHECK();
	}

	static void DrawSquare(RenderSize x, RenderSize y, RenderSize width, RenderSize height)
	{
		// Use a transformation matrix instead for UI?
		const float widthVal = width.Get(NormalizationContext::Width);
		const float heightVal = height.Get(NormalizationContext::Height);
		const float xPos = x.Get(NormalizationContext::Width) * (2 - widthVal * 2);
		const float yPos = y.Get(NormalizationContext::Height) * (2 - heightVal * 2);
		const float xOffset = 1.0f - widthVal;
		const float yOffset = 1.0f - heightVal;
		const float finalX = xPos - xOffset;
		const float finalY = -1 * (yPos - yOffset);

		Draw({
				Vertex{-widthVal + finalX, heightVal + finalY, 0, {0, 1.0}},
				Vertex{widthVal + finalX, heightVal + finalY, 0, {1.0, 1.0}},
				Vertex{widthVal + finalX, -heightVal + finalY, 0, {1.0, 0}},
				Vertex{-widthVal + finalX, -heightVal + finalY, 0, {0, 0}},
			 },
			 {
				0, 1, 3,
				3, 1, 2
			 });
	}
};