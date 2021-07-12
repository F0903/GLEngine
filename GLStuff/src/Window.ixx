module;
#include <exception>
#include <GLFW/glfw3.h>
#include "util.h"
export module Window;

export class Window
{
	public:
	Window(int width, int height, const char* title)
	{
		Init(width, height, title);
	}

	~Window()
	{
		glfwDestroyWindow(win);
	}

	private:
	GLFWwindow* win;
	bool close;

	private:
	void Init(int width, int height, const char* title)
	{
		if (!glfwInit())
		{
			const auto msg = "Could not init glfw.";
			DEBUG_LOG(msg);
			throw std::exception(msg);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#ifdef _MAC_OS
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif // _MAC_OS

		win = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!win)
		{
			const auto msg = "Window was null.";
			DEBUG_LOG(msg);
			throw std::exception(msg);
		}
		glfwMakeContextCurrent(win);
	}

	public:
	Window WithTitle(const char* title) const
	{
		glfwSetWindowTitle(win, title);
		return *this;
	}

	const bool ShouldClose() const
	{
		return glfwWindowShouldClose(win);
	}

	const void PollAndSwap() const
	{
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	GLFWwindow* GetRawWindow() const
	{
		return win;
	}
};