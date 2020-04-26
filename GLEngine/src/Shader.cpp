#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Utility.h"
#include "Debug.h"
#include "TestResources.h"

ShaderStage::ShaderStage(const std::string& str) : shaderID(0)
{
	std::string localStr = str;
	ToLower(localStr);
	if (localStr == "vertex")
	{
		type = ShaderStageType::Vertex;
	}
	else if (localStr == "fragment")
	{
		type = ShaderStageType::Fragment;
	}
	else
	{
		type = ShaderStageType::Invalid;
	}
}

int Shader::GetUniformLocation(const char* name)
{
	if (uniformCache.find(name) != uniformCache.end())
		return uniformCache[name];
	auto uni = glGetUniformLocation(programID, name);
	if (uni != -1)
		uniformCache[name] = uni;
	return uni;
}

void Shader::Unbind() const
{
	if (!IsBound())
		return;
	IBindable::Unbind();
	glUseProgram(0);
}

void Shader::Bind() const
{
	if (IsBound())
		return;
	IBindable::Bind();
	glUseProgram(programID);
}

void Shader::SetUniform(const char* name, float val)
{
	auto uni = GetUniformLocation(name);
	GLE_GL_DEBUG_CALL(glUniform1f(uni, val));
}

void Shader::SetUniform(const char* name, float val1, float val2)
{
	auto uni = GetUniformLocation(name);
	GLE_GL_DEBUG_CALL(glUniform2f(uni, val1, val2));
}

void Shader::SetUniform(const char* name, float val1, float val2, float val3)
{
	auto uni = GetUniformLocation(name);
	GLE_GL_DEBUG_CALL(glUniform3f(uni, val1, val2, val3));
}

void Shader::SetUniform(const char* name, int val)
{
	auto uni = GetUniformLocation(name);
	GLE_GL_DEBUG_CALL(glUniform1i(uni, val));
}

void Shader::SetUniform(const char* name, unsigned int val)
{
	auto uni = GetUniformLocation(name);
	GLE_GL_DEBUG_CALL(glUniform1ui(uni, val));
}

bool Shader::CompileShaderPart(ShaderStage& shader)
{
	shader.shaderID = glCreateShader((unsigned int)shader.type);
	const char* src[1]{ shader.source.c_str() };
	GLE_GL_DEBUG_CALL(glShaderSource(shader.shaderID, 1, src, NULL));
	GLE_GL_DEBUG_CALL(glCompileShader(shader.shaderID));

	int result;
	glGetShaderiv(shader.shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader.shaderID, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)_malloca(length);
		glGetShaderInfoLog(shader.shaderID, length, &length, msg);
		std::cerr << "Failed to compile shader!\n" << "Error: " << msg;
		_freea(msg);
		glDeleteShader(shader.shaderID);
		return false;
	}
	GLE_GL_DEBUG_CALL(glAttachShader(programID, shader.shaderID));
	return true;
}

size_t GetNumOfChars(const char* str)
{
	size_t num = 0;
	while (*str)
	{
		num += 1;
		str++;
	}
	return num;
}

void Shader::ParseShader(const char* path, const char* shaderStageSeparator)
{
	bool fallback = !std::filesystem::exists(path);

	std::istream& shaderStream = *(fallback ? dynamic_cast<std::istream*>(&std::stringstream(testShader)) : dynamic_cast<std::istream*>(&std::ifstream(path))); // May cause eye cancer

	auto numOfCharsInSep = GetNumOfChars(shaderStageSeparator);

	std::vector<ShaderStage> stages;
	int currentStageNum = -1;
	std::string line;
	while (std::getline(shaderStream, line))
	{
		bool newStage = false;
		for (size_t i = 0; i < numOfCharsInSep + 1; i++)
		{
			if (line[0] != shaderStageSeparator[0])
				break;

			if (i == numOfCharsInSep)
			{
				currentStageNum++;
				newStage = true;

				line.erase(0, numOfCharsInSep);
				ShaderStage stage = line;
				stages.push_back(stage);
				continue;
			}
		}
		if (newStage)
			continue;

		stages[currentStageNum].source += line + '\n';
	}
	if (currentStageNum == -1)
		throw "Invalid shader specified.";

 	for (auto stage : stages)
	{
		CompileShaderPart(stage);
	}
	shaderStages = stages;
}

Shader::Shader(const char* path) : programID(glCreateProgram()), IBindable(programID, BindableType::Shader)
{
	ParseShader(path);

	GLE_GL_DEBUG_CALL(glLinkProgram(programID));
	GLE_GL_DEBUG_CALL(glValidateProgram(programID));

	GLE_GL_DEBUG_CALL(glUseProgram(programID));
}

Shader::~Shader()
{
	glDeleteProgram(programID);
}
