#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util.h"
#include "OpenGL/gl_util.h"

//TODO: Convert solution to use CMake when VS and CMake gets proper interop with C++20 modules.

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

	const auto defaultShader = Shader("./resources/default.shader");
	const auto redShader = Shader("./resources/red.shader");

	Renderer::Init();
	Renderer::SetShader(&defaultShader);

	auto script = ScriptEngine();
	script.ExposeFn({ "DrawSquare", [](LuaState* state) -> int
	{
		//TODO: Expand with more options like percentages.
		const auto x = GetNumber(state, 1);
		const auto y = GetNumber(state, 2);
		const auto width = GetNumber(state, 3);
		const auto height = GetNumber(state, 4);
		Renderer::DrawSquare(RenderSize::Pixels(x), RenderSize::Pixels(y), RenderSize::Pixels(width), RenderSize::Pixels(height));
		return 0;
	} });
	script.Run("./resources/Test.lua");

	while (!window.ShouldClose())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		script.Update();
		window.PollAndSwap();
	}

	/*
	while (!window.ShouldClose())
	{
		DEBUG_GL_CHECK();
		glClear(GL_COLOR_BUFFER_BIT);
		Renderer::SetShader(&defaultShader);
		Renderer::DrawSquare(0.0_px, 0.0_px, 50.0_px, 50.0_px);
		Renderer::SetShader(&redShader);
		Renderer::DrawSquare(50.0_vw, 50.0_vh, 50.0_px, 50.0_px);
		Renderer::SetShader(&defaultShader);
		Renderer::DrawSquare(100.0_vw, 100.0_vh, 50.0_px, 50.0_px);
		window.PollAndSwap();
	}
	*/
}