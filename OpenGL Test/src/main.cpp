#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util.h"

import Window;
import Shader;
import Renderer;

auto renderer = Renderer();

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

void checkErrors()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		DEBUG_ERR("OPENGL ERROR: " << err);
	}
}

int main()
{
	const auto window = Window(600, 500, "OpenGL Stuff");

	initOpenGl(window.GetRawWindow());

	renderer.Init();
	const auto shader = Shader("./resources/default.shader");
	renderer.SetShader(shader);

	while (!window.ShouldClose())
	{
		checkErrors();
		glClear(GL_COLOR_BUFFER_BIT);
		renderer.DrawSquare(0, 0, 50, 50);
		window.PollAndSwap();
	}
}