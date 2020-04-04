#pragma once
#include <string>
#include <GL/glew.h>

enum class TextureResizeMode : int
{
	Linear = GL_LINEAR,
	Nearest = GL_NEAREST
};

enum class TextureWrapMode : int
{
	Repeat = GL_REPEAT,
	MirroredRepeat = GL_MIRRORED_REPEAT,
	ToBorder = GL_CLAMP_TO_BORDER,
	ToEdge = GL_CLAMP_TO_EDGE
};

class Texture2D
{
	private:
	unsigned int texID = 0;
	int slot = 0;

	struct TextureImage
	{
		void* data;
		int x, y, channels;
	}image;

	public:
	std::string name;
	std::string path;

	inline const void* GetRawData() const { return image.data; }

	inline int GetID() const { return texID; }
	inline int GetSlot() const { return slot; }

	Texture2D(const char* path, TextureResizeMode resize = TextureResizeMode::Linear, TextureWrapMode wrap = TextureWrapMode::Repeat, unsigned int slot = 0);
	~Texture2D();

	void Bind() const;
	void Unbind() const;
};