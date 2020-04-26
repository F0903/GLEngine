#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

#include "IBindable.h"

enum class ShaderStageType
{
	Invalid = -1,
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
};

struct ShaderStage
{
	ShaderStageType type;
	std::string source;
	int shaderID;

	ShaderStage(const std::string& str);
};

class Shader : public IBindable
{
	private:
	unsigned int programID;

	std::unordered_map<const char*, unsigned int> uniformCache;

	std::vector<ShaderStage> shaderStages;

	int GetUniformLocation(const char* name);

	bool CompileShaderPart(ShaderStage& shader);
	void ParseShader(const char* path, const char* shaderStageSeparator = "@");

	public:
	Shader(Shader&& other) = default;
	Shader(const char* path);
	~Shader();

	void Unbind() const override;
	void Bind() const override;

	void SetUniform(const char* name, float val);
	void SetUniform(const char* name, float val1, float val2);
	void SetUniform(const char* name, float val1, float val2, float val3);
	void SetUniform(const char* name, int val);
	void SetUniform(const char* name, unsigned int val);
};