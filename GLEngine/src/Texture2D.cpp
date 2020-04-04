#include "Texture2D.h"

#include <GL/glew.h>
#include <filesystem>
#include <iostream>

#include <stb_image/stb_image.h>

#include "Utility.h"
#include "Debug.h"

Texture2D::Texture2D(const char* path, TextureResizeMode resize, TextureWrapMode wrap, unsigned int slot) : slot(slot)
{	
	this->path = std::filesystem::exists(path) ? path : throw std::exception("Path to image does not exist...");
	name = GetFileName(path, false).c_str();
	
	stbi_set_flip_vertically_on_load(true);
	image.data = stbi_load(path, &image.x, &image.y, &image.channels, 4);

	GLE_GL_DEBUG_CALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLE_GL_DEBUG_CALL(glGenTextures(1, &texID));
	GLE_GL_DEBUG_CALL(glBindTexture(GL_TEXTURE_2D, texID));
	
	GLE_GL_DEBUG_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, resize == TextureResizeMode::Linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
	GLE_GL_DEBUG_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)resize));
	GLE_GL_DEBUG_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)wrap));
	GLE_GL_DEBUG_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)wrap));
	
	GLE_GL_DEBUG_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.x, image.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data));
	GLE_GL_DEBUG_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	GLE_GL_DEBUG_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture2D::~Texture2D()
{
	stbi_image_free(image.data);
	glDeleteTextures(1, &texID);
}

void Texture2D::Bind() const
{
	GLE_GL_DEBUG_CALL(glBindTexture(GL_TEXTURE_2D, texID));
}

void Texture2D::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}