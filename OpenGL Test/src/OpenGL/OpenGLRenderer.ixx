module;
#include <Gl/glew.h>
export module OpenGLRenderer;
import Renderer;
import Shader;
import Vertex;

export class OpenGLRenderer : Renderer
{
	void SetShader(Shader shader) override
	{

	}

	void DrawSquare(float width, float height) override
	{
		const Vertex vertices[] = {
			Vertex{-1, -1, 0},
			Vertex{1, -1, 0},
			Vertex{1, 1, 0},
			Vertex{-1, 1, 0}
		};

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}
};