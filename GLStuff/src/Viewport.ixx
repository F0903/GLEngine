module;
#include <GL/glew.h>
export module Viewport;

export class Viewport
{
	Viewport()
	{
		Init();
	}

	public:
	int width, height;

	private:
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
		this->width = width;
		this->height = height;
	}

	public:
	static void Update(int width, int height)
	{
		glViewport(0, 0, width, height);
		width = width;
		height = height;
	}

	static const Viewport& Get()
	{
		static const Viewport viewport;
		return viewport;
	}
};