#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util.h"

import Window;
import Shader;

void initOpenGl()
{
	const auto code = glewInit();
	if (code != GLEW_OK)
	{
		DEBUG_ERR("Could not init glew. Err: " << code);
		throw "Could not init glew. Check console for error.";
	}
}

void render(GLFWwindow* win)
{
	int width, height;
	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0, 0, width, height);
}

int main()
{
	const auto window = Window(500, 500, render);

	initOpenGl();

	const auto shader = Shader("./resources/default.shader");
	shader.Use();

	while(!window.shouldClose())
		window.poll();
}