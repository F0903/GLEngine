#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util.h"
#include "OpenGL/gl_util.h"

import Window;
import Shader;
import Renderer;
import RenderSize;
import Matrix;

void initOpenGl(GLFWwindow* win)
{
	const auto code = glewInit();
	if (code != GLEW_OK)
	{
		DEBUG_ERR("Could not init glew. Err: " << code);
		throw "Could not init glew. Check console for error.";
	}

	int width, height;
	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(win, [](GLFWwindow* win, int width, int height)
	{
		glViewport(0, 0, width, height);
		Renderer::UpdateViewport(width, height);
	});
}

int main()
{
	auto t1 = Matrix<float, 3>({ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} });
	auto t2 = Matrix<float, 3>({ {6, 4, 5}, {3, 5, 8}, {3, 2, 3} });

	const auto window = Window(600, 500, "OpenGL Stuff");

	initOpenGl(window.GetRawWindow());

	auto renderer = Renderer();
	const auto shader = Shader("./resources/default.shader");
	renderer.SetShader(&shader);

	while (!window.ShouldClose())
	{
		DEBUG_GL_CHECK();
		glClear(GL_COLOR_BUFFER_BIT);
		renderer.DrawSquare(0, 0, 50.0_px, 50.0_px);
		window.PollAndSwap();
	}
}