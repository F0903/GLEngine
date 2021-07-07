module;
#include <Gl/glew.h>
#include <initializer_list>
#include "util.h"
#include "OpenGL/gl_util.h"
export module Renderer;
import Shader;
import Vertex;
import Viewport;
import RenderSize;
import VertexArray;

export class Renderer
{
	public:
	~Renderer()
	{
		delete vertexArray;
	}

	private:
	static inline Viewport viewport;
	static inline const Shader* currentShader = nullptr;
	static inline VertexArray* vertexArray = nullptr;

	private:
	static void InitViewport()
	{
		union
		{
			GLint view[4];
			struct
			{
				GLint a, b;
				GLint width, height;
			};
		};
		glGetIntegerv(GL_VIEWPORT, view);
		viewport.width = width;
		viewport.height = height;
	}

	public:
	static void Init()
	{
		vertexArray = new VertexArray();
		InitViewport();
	}

	static void UpdateViewport(int width, int height)
	{
		glViewport(0, 0, width, height);
		viewport.width = width;
		viewport.height = height;
	}

	static Viewport GetViewport()
	{
		return viewport;
	}

	static void SetShader(const Shader* shader)
	{
		currentShader = shader;
		currentShader->Use();
	}

	static void Draw(const SizedPtr<Vertex> vertices, const SizedPtr<unsigned int> indices)
	{
		vertexArray->Bind();
		auto& vert = vertexArray->GetVertexBuffer();
		auto& ind = vertexArray->GetIndexBuffer();

		vert.SetData(vertices);
		ind.SetData(indices);
		vertexArray->SetAttribute(0, { 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) });
		DEBUG_GL_CHECK();

		//TODO: Debug
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, ind.GetDataSize(), GL_UNSIGNED_INT, 0);
		DEBUG_GL_CHECK();
	}

	static void DrawSquare(RenderSize x, RenderSize y, RenderSize width, RenderSize height)
	{
		// Use a transformation matrix instead for UI?
		const float widthVal = width.Get(viewport, NormalizationContext::Width);
		const float heightVal = height.Get(viewport, NormalizationContext::Height);
		const float xPos = x.Get(viewport, NormalizationContext::Width) * (2 - widthVal * 2);
		const float yPos = y.Get(viewport, NormalizationContext::Height) * (2 - heightVal * 2);
		const float xOffset = 1.0f - widthVal;
		const float yOffset = 1.0f - heightVal;
		const float finalX = xPos - xOffset;
		const float finalY = -1 * (yPos - yOffset);

		Draw({
				Vertex{-widthVal + finalX, heightVal + finalY, 0},
				Vertex{widthVal + finalX, heightVal + finalY, 0},
				Vertex{widthVal + finalX, -heightVal + finalY, 0},
				Vertex{-widthVal + finalX, -heightVal + finalY, 0},
			 },
			 {
				0, 1, 3,
				3, 1, 2
			 });
	}
};