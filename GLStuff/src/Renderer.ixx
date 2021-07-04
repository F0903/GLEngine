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
import VertexBuffer;
import IndexBuffer;
import VertexArray;

export class Renderer
{
	public:
	Renderer()
	{
		DEBUG_GL_CHECK();
		Init();
		DEBUG_GL_CHECK();
	}

	private:
	inline static Viewport viewport;
	VertexBuffer vert = VertexBuffer();
	IndexBuffer ind = IndexBuffer();
	VertexArray arr = VertexArray();

	public:
	static void UpdateViewport(int width, int height)
	{
		viewport.width = width;
		viewport.height = height;
	}

	static Viewport GetViewport()
	{
		return viewport;
	}

	void InitViewport() const
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

	void Init()
	{
		InitViewport();
	}

	void SetShader(const Shader& shader)
	{
		shader.Use();
	}

	void Draw(Vertex* vertices, unsigned int* indices) const
	{

	}

	void DrawSquare(float x, float y, RenderSize width, RenderSize height)
	{
		const float widthVal = width.Get(viewport, NormalizationContext::Width);
		const float heightVal = height.Get(viewport, NormalizationContext::Height);
		const float normX = x / viewport.width;
		const float normY = y / viewport.height;

		DEBUG_GL_CALL(arr.Bind());
		DEBUG_GL_CALL(vert.SetData({
			Vertex{-widthVal + normX, heightVal + normY, 0},
			Vertex{widthVal + normX, heightVal + normY, 0},
			Vertex{widthVal + normX, -heightVal + normY, 0},
			Vertex{-widthVal + normX, -heightVal + normY, 0},
								   }));
		vert.SetAttribute(0, { 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) });
		DEBUG_GL_CHECK();
		DEBUG_GL_CALL(ind.SetData({
			0, 1, 3,
			3, 1, 2
								  }));

		DEBUG_GL_CALL(glDrawElements(GL_TRIANGLES, ind.GetDataSize(), GL_UNSIGNED_INT, 0));
	}
};