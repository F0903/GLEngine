#include "Texture2D.h"
#include "Utility.h"
#include <GL/glew.h>

using namespace cimg_library;

static CImgDisplay dbugDisplay(250, 250, "", 3U, false, true);

Texture2D::Texture2D(const char* path) : image(path)
{
	this->path = path;
	name = GetFileName(path, false).c_str();

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &texID);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture2D::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::ShowDebugDisplay()
{
	auto localName = name;
	localName.append(" (Debug)");
	dbugDisplay.set_title(localName.c_str());
	dbugDisplay.show();
	dbugDisplay.display(image);
}

void Texture2D::CloseDebugDisplay()
{
	dbugDisplay.close();
}