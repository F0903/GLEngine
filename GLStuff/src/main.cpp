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
import Texture;

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

	auto defaultShader = Shader("./resources/default.shader");

	Renderer::Init();
	Renderer::SetShader(&defaultShader);

	auto script = ScriptEngine();

	script.ExposeFn({ "SetTexture", [](LuaState* state) -> int
	{
		const auto path = GetString(state, 1);
		auto tex = new Texture(path);
		tex->SetWrapMode(TextureWrap::BorderClamp);
		tex->SetFilteringMode(TextureFiltering::Nearest, nullptr);
		Renderer::SetTexture({ tex, true });
		return 0;
	} });

	script.ExposeFn({ "DrawSquare", [](LuaState* state) -> int
	{
		const auto x = GetPctOrNum(state, 1);
		const auto y = GetPctOrNum(state, 2);
		const auto width = GetPctOrNum(state, 3);
		const auto height = GetPctOrNum(state, 4);
		Renderer::DrawSquare(RenderSize::FromPctOrNum(x), RenderSize::FromPctOrNum(y), RenderSize::FromPctOrNum(width), RenderSize::FromPctOrNum(height));
		return 0;
	} });

	script.Run("./resources/Test.lua");

	while (!window.ShouldClose())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		script.Update();
		window.PollAndSwap();
	}
	Renderer::Free();
}