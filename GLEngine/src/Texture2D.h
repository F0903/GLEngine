#pragma once
#include <string>
#include <CImg.h>

class Texture2D
{
	private:
	unsigned int texID;
	cimg_library::CImg<unsigned char> image;

	public:
	std::string name;
	std::string path;

	inline unsigned char* GetRawData() const { return (unsigned char*)image.data(); }

	inline unsigned int GetID() const { return texID; }

	Texture2D(const char* path);
	~Texture2D();

	void Bind() const;
	void Unbind() const;

	void ShowDebugDisplay();
	void CloseDebugDisplay();
};