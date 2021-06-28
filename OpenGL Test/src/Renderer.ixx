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

	void DrawSquare(float width, float height) const
	{
		const float normWidth = width / viewWidth;
		const float normHeight = height / viewHeight;
		const Vertex vertices[] = {
			Vertex{-normWidth, normHeight, 0},
			Vertex{normWidth, normHeight, 0},
			Vertex{-normWidth, -normHeight, 0},
			Vertex{-normWidth, -normHeight, 0},
			Vertex{normWidth, normHeight, 0},
			Vertex{normWidth, -normHeight, 0}
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(Vertex));
	}
};