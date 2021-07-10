module;
#include <GL/glew.h>
#include <stb_image/stb_image.h>
export module Texture;
import Optional;

class Renderer;

export enum class TextureWrap : GLint
{
	Repeat = GL_REPEAT,
	MirrorRepeat = GL_MIRRORED_REPEAT,
	EdgeClamp = GL_CLAMP_TO_EDGE,
	BorderClamp = GL_CLAMP_TO_BORDER
};

export enum class TextureFiltering : GLint
{
	Linear = GL_LINEAR,
	Nearest = GL_NEAREST
};

export enum class MipmapFiltering : GLint
{
	NearestNearest = GL_NEAREST_MIPMAP_NEAREST,
	LinearNearest = GL_LINEAR_MIPMAP_NEAREST,
	NearestLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearLinear = GL_LINEAR_MIPMAP_LINEAR,
};

export class Texture
{
	public:
	Texture(const char* path)
	{
		glGenTextures(1, &id);
		Load(path);
	}

	~Texture()
	{
		Free();
	}

	private:
	unsigned int id;
	int width, height, channels;
	unsigned char* img;
	bool freed = false;

	private:
	void Load(const char* path)
	{
		stbi_set_flip_vertically_on_load(true);
		img = stbi_load(path, &width, &height, &channels, 0);
		if (!img) throw "Failed to load image.";
		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	}

	void Free()
	{
		if (freed) return;
		stbi_image_free(img);
		freed = true;
	}

	void GenerateMipMaps() const
	{
		Bind();
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	public:
	void SetWrapMode(TextureWrap mode) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)mode);
	}

	void SetFilteringMode(TextureFiltering mode, Optional<MipmapFiltering> mipMode) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipMode.null ? (GLint)mode : (GLint)mipMode.value);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)mode);
	}

	void Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	friend Renderer;
};