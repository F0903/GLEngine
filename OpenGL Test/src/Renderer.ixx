module;
#include <Gl/glew.h>
#include "util.h"
export module Renderer;
import Shader;
import Vertex;

export class Renderer
{
	public:
	Renderer()
	{}

	private:
	inline static int viewWidth, viewHeight;

	public:
	static void UpdateViewport(int width, int height)
	{
		viewWidth = width;
		viewHeight = height;
	}

	void InitViewport()
	{
		union
		{
			GLint viewport[4];
			struct
			{
				GLint a, b;
				GLint width, height;
			};
		};
		glGetIntegerv(GL_VIEWPORT, viewport);
		viewWidth = width;
		viewHeight = height;
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

	void DrawSquare(float x, float y, float width, float height) const
	{
		const float normX = x / viewWidth;
		const float normY = y / viewHeight;
		const float normWidth = width / viewWidth;
		const float normHeight = height / viewHeight;
		const Vertex vertices[] = {
			Vertex{-normWidth + normX, normHeight + normY, 0},
			Vertex{normWidth + normX, normHeight + normY, 0},
			Vertex{normWidth + normX, -normHeight + normY, 0},
			Vertex{-normWidth + normX, -normHeight + normY, 0},
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