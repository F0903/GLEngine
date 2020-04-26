#include "Renderer.h"

#include <GL/glew.h>
#include <filesystem>
#include <fstream>
#include <stb_image/stb_image_write.h>

void Renderer::Draw(const Shader& shader, const IndexBuffer& indecies, const VertexBuffer& vertices)
{
	shader.Bind();
	indecies.Bind();
	vertices.Bind();
	glDrawElements(GL_TRIANGLES, indecies.IndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Screenshot(const char* path)
{
	auto outPath = std::filesystem::path(path != 0 ? path : "./");
	
	long long int viewport[4];
	glGetInteger64v(GL_VIEWPORT, viewport);

	void* pixels = malloc(viewport[2] * viewport[3] * 4);
	glReadPixels(0, 0, viewport[2], viewport[3], GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	std::ofstream dump(outPath.string().append("rawscreenshot.bin"), std::ios::trunc | std::ios::binary);
	dump << pixels;
	dump.close();

	auto result = stbi_write_bmp(outPath.string().append("screenshot.bmp").c_str(), viewport[2], viewport[3], 4, pixels);
	if (result == 0)
		throw;

	free(pixels);
}