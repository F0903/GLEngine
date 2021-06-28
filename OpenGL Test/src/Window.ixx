module;
#include <exception>
#include <GLFW/glfw3.h>
#include "util.h"
export module Window;

export using RenderFn = void(GLFWwindow* win);

export class Window
{
private:
	GLFWwindow* win;
	RenderFn* render;
	bool close;

private:
	void init(int width, int height)
	{
		if (!glfwInit())
		{
			const auto msg = "Could not init glfw.";
			DEBUG_LOG(msg);
			throw std::exception(msg);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _MAC_OS
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // _MAC_OS

		win = glfwCreateWindow(width, height, "Hello from GLFW!", NULL, NULL);
		if (!win)
		{
			const auto msg = "Window was null.";
			DEBUG_LOG(msg);
			throw std::exception(msg);
		}
		glfwMakeContextCurrent(win);
	}

public:
	Window(int width, int height, RenderFn render_func)
	{
		render = render_func;
		init(width, height);
	}

	~Window()
	{
		glfwDestroyWindow(win);
	}

	const bool shouldClose() const 
	{
		return glfwWindowShouldClose(win);
	}

	const void poll() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
		render(win);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
};