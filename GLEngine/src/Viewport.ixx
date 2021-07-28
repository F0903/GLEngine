module;
#include <GL/glew.h>
#include "OpenGL/gl_util.h"
export module Viewport;

export class Viewport
{
	Viewport()
	{}

	public:
	int width = -1, height = -1;

	public:
	static void Update(int width, int height)
	{
		glViewport(0, 0, width, height);
		width = width;
		height = height;
	}

	static Viewport& Get()
	{
		static Viewport viewport;
		return viewport;
	}

	void Init()
	{
		union
		{
			GLint view[4];
			struct
			{
				GLint a, b;
				GLint width, height;
			};
		};
		glGetIntegerv(GL_VIEWPORT, view);
		DEBUG_GL_CHECK();
		this->width = width;
		this->height = height;
	}

	friend int main();
};