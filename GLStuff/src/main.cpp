#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util.h"
#include "OpenGL/gl_util.h"

import Window;
import Shader;
import Renderer;
import RenderSize;
import Matrix;
import ScriptEngine;

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
		Renderer::UpdateViewport(width, height);
	});
}

int main()
{
	const auto window = Window(600, 500, "OpenGL Stuff");

	initOpenGl(window.GetRawWindow());

	auto renderer = Renderer();

	const auto defaultShader = Shader("./resources/default.shader");
	const auto redShader = Shader("./resources/red.shader");

	auto script = ScriptEngine();
	script.Run("./resources/Test.lua");

	while (!window.ShouldClose())
	{
		DEBUG_GL_CHECK();
		glClear(GL_COLOR_BUFFER_BIT);
		renderer.SetShader(&defaultShader);
		renderer.DrawSquare(0.0_px, 0.0_px, 50.0_px, 50.0_px);
		renderer.SetShader(&redShader);
		renderer.DrawSquare(50.0_vw, 50.0_vh, 50.0_px, 50.0_px);
		renderer.SetShader(&defaultShader);
		renderer.DrawSquare(100.0_vw, 100.0_vh, 50.0_px, 50.0_px);
		window.PollAndSwap();
	}
}