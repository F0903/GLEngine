#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

enum class ShaderPartType
{
	Invalid = -1,
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	COUNT = 3
};

struct ShaderPart
{
	const ShaderPartType type;
	std::string source;
	int shaderID;
};

class Shader
{
	private:
	int programID;

	std::unordered_map<const char*, unsigned int> uniformCache;

	ShaderPart Vertex{ ShaderPartType::Vertex };
	ShaderPart Fragment{ ShaderPartType::Fragment };

	int GetUniformLocation(const char* name);

	unsigned int CompileShaderPart(const ShaderPart& shader);
	ShaderPart* ParseShader(const char* path, const char* shaderPartSeparator = "@");

	public:
	Shader(const char* path);
	~Shader();

	void Unbind() const;
	void Bind() const;

	void SetUniform(const char* name, float val);
	void SetUniform(const char* name, float val1, float val2);
	void SetUniform(const char* name, float val1, float val2, float val3);
	void SetUniform(const char* name, int val);
};