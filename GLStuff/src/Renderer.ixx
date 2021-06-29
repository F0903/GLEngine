module;
#include <Gl/glew.h>
#include "util.h"
export module Renderer;
import Shader;
import Vertex;
import Viewport;
import RenderSize;

export class Renderer
{
	private:
	inline static Viewport viewport;

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

	void InitViewport()
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

	void InitVertexData()
	{
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		unsigned int ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);
	}

	void Init()
	{
		InitViewport();
		InitVertexData();
	}

	void SetShader(const Shader& shader)
	{
		shader.Use();
	}

	void DrawSquare(float x, float y, RenderSize width, RenderSize height) const
	{
		const auto widthVal = width.Get(viewport, NormalizationContext::Width);
		const auto heightVal = height.Get(viewport, NormalizationContext::Height);
		const auto normX = x / viewport.width;
		const auto normY = y / viewport.height;
		const Vertex vertices[] = {
			Vertex{-widthVal + normX, heightVal + normY, 0},
			Vertex{widthVal + normX, heightVal + normY, 0},
			Vertex{widthVal + normX, -heightVal + normY, 0},
			Vertex{-widthVal + normX, -heightVal + normY, 0},
		};
		const unsigned int indices[] = {
			0, 1, 3,
			3, 1, 2
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
	}
};