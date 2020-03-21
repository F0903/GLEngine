#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
	glUseProgram(0);
}

void Shader::Bind() const
{
	glUseProgram(programID);
}

void Shader::SetUniform(const char* name, float val)
{
	auto uni = GetUniformLocation(name);
	glUniform1f(uni, val);
}

void Shader::SetUniform(const char* name, float val1, float val2)
{
	auto uni = GetUniformLocation(name);
	glUniform2f(uni, val1, val2);
}

void Shader::SetUniform(const char* name, float val1, float val2, float val3)
{
	auto uni = GetUniformLocation(name);
	glUniform3f(uni, val1, val2, val3);
}

void Shader::SetUniform(const char* name, int val)
{
	auto uni = GetUniformLocation(name);
	glUniform1i(uni, val);
}

unsigned int Shader::CompileShaderPart(const ShaderPart& shader)
{
	auto id = glCreateShader((unsigned int)shader.type);
	const char* src[1]{ shader.source.c_str() };
	glShaderSource(id, 1, src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)_malloca(length);
		glGetShaderInfoLog(id, length, &length, msg);
		std::cerr << "Failed to compile shader!\n" << "Error: " << msg;
		_freea(msg);
		glDeleteShader(id);
		return 0;
	}
	glAttachShader(programID, id);
	return id;
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

ShaderPart* Shader::ParseShader(const char* path, const char* shaderPartSeparator)
{
	auto f = std::ifstream(path);
	std::stringstream ss[2];

	auto numOfCharsInSep = GetNumOfChars(shaderPartSeparator);

	ShaderPartType type = ShaderPartType::Invalid;
	std::string line;
	while (std::getline(f, line))
	{
		bool newShader = false;
		for (size_t i = 0; i < numOfCharsInSep; i++)
		{
			if (line[i] != shaderPartSeparator[i])
				continue;

			if (i != numOfCharsInSep - 1)
				continue;

			newShader = true;
			line.erase(0, numOfCharsInSep);
		}
		if (newShader)
		{
			type = line == "vertex" ? ShaderPartType::Vertex : ShaderPartType::Fragment;
			continue;
		}

		switch (type)
		{
			case ShaderPartType::Invalid:
				continue;
			case ShaderPartType::Vertex:
				ss[0] << line << '\n';
				break;
			case ShaderPartType::Fragment:
				ss[1] << line << '\n';
				break;
			default:
				throw "Unknown error occured during shader parsing.";
		}
	}
	if (type == ShaderPartType::Invalid)
		std::cerr << "Shader specified is invalid.\n";

	Vertex.source = ss[0].str();
	Fragment.source = ss[1].str();
	Vertex.shaderID = CompileShaderPart(Vertex);
	Fragment.shaderID = CompileShaderPart(Fragment);
}

Shader::Shader(const char* path)
{
	programID = glCreateProgram();

	ParseShader(path);

	glLinkProgram(programID);
	glValidateProgram(programID);

	glUseProgram(programID);
}

Shader::~Shader()
{
	glDeleteProgram(programID);
}
