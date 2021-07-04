module;
#include <GL/glew.h>
#include <fstream>
#include <format>
#include <exception>
#include "util.h"
export module Shader;

using HandlerFn = const void(const char* source, int* const shaderID, const int programID);

export class ShaderError : public std::exception
{
	private:
	const char* msg;

	public:
	ShaderError(const char* msg) : msg(msg) {}

	const char* what() const override
	{
		return msg;
	}
};

struct ShaderHandler
{
	const char* shaderName;
	HandlerFn* handler;
};

export class Shader
{
	private:
	Shader() = default;

	public:
	Shader(const char* filePath)
	{
		DEBUG_LOG("Starting parse of " << filePath);
		std::ifstream shader(filePath, std::ios::in | std::ios::binary);
		const size_t len = shader.seekg(0, shader.end).tellg();
		shader.seekg(0, shader.beg);
		char* source = new char[len + 1];
		shader.read(source, len);
		source[len] = '\0';
		ParseShader(source);
		delete[] source;
	}

	private:
	struct Delimiter
	{
		const static int len = 4;
		const char value[len] = "//#";
	} const delimiter;

	const static int handlerCount = 2;
	const ShaderHandler shaderHandlers[handlerCount] = {
		ShaderHandler{"VERTEX", HandleVertex},
		ShaderHandler{"FRAGMENT", HandleFragment}
	};

	int shaderProgramID = -1;

	private:
	const inline bool IsDelimiter(const char* char_ptr, const size_t i = 0) const
	{
		return (*(char_ptr + i) == delimiter.value[0]
				&& *(char_ptr + i + 1) == delimiter.value[1]
				&& *(char_ptr + i + 2) == delimiter.value[2]);
	}

	static const void HandleVertex(const char* source, int* const shaderId, const int programID)
	{
		DEBUG_LOG("VERTEX SHADER:\n" << source << '\n');
		*shaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(*shaderId, 1, &source, 0);
		glCompileShader(*shaderId);

		int  success;
		char infoLog[512];
		glGetShaderiv(*shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(*shaderId, 512, NULL, infoLog);
			DEBUG_ERR("Vertex shader failed to parse with error: " << infoLog);
			return;
		}

		glAttachShader(programID, *shaderId);
	}

	static const void HandleFragment(const char* source, int* const shaderId, const int programID)
	{
		DEBUG_LOG("FRAGMENT SHADER:\n" << source << '\n');
		*shaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(*shaderId, 1, &source, 0);
		glCompileShader(*shaderId);

		int  success;
		char infoLog[512];
		glGetShaderiv(*shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(*shaderId, 512, NULL, infoLog);
			DEBUG_ERR("Fragment shader failed to parse with error: " << infoLog);
			return;
		}

		glAttachShader(programID, *shaderId);
	}

	const void ParseShaderPartSource(char* source, int* const shaderId, ShaderHandler handler) const
	{
		size_t len = -1;
		size_t i;
		for (i = 0; *(source + i); i++)
		{
			if (!IsDelimiter(source, i)) [[likely]]
				continue;
			break;
		}
		len = i;

		char* partSource = new char[len + 1];
		for (size_t i = 0; i < len; i++)
		{
			partSource[i] = source[i];
		}
		partSource[len] = '\0';

		handler.handler(partSource, shaderId, shaderProgramID);
		delete[] partSource;
	}

	const void ParseShaderPartType(char* source, int* const shaderId) const
	{
		for (size_t i = 0; i < handlerCount; i++)
		{
			const auto handler = shaderHandlers[i];
			int matchCount = 0;
			size_t j;
			for (j = 0; *(handler.shaderName + j); j++)
			{
				const auto nameChar = *(handler.shaderName + j);
				const auto sourceChar = *(source + j);
				if (nameChar != sourceChar)
					continue;
				++matchCount;
			}
			if (matchCount != j) continue;
			source += j;
			ParseShaderPartSource(source, shaderId, handler);
		}
	}

	const void ParseShader(char* source)
	{
		shaderProgramID = glCreateProgram();
		int shaderIDs[handlerCount]{};
		int i = 0;
		while (*source)
		{
			if (!IsDelimiter(source)) [[likely]] {
				source += 1;
				continue;
			}
			source += 3;
			ParseShaderPartType(source, &shaderIDs[i]);
			++i;
		}
		glLinkProgram(shaderProgramID);

		int  success;
		char infoLog[512];
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
			DEBUG_ERR("Could not link shaders to shader program. Err: " << infoLog);
		}
		CleanUpShaders(shaderIDs);
	}

	const void CleanUpShaders(int* shaderIDs)
	{
		for (size_t i = 0; i < handlerCount; i++)
		{
			glDeleteShader(shaderIDs[i]);
		}
	}

	public:
	static Shader FromSource(char* source)
	{
		auto shader = Shader();
		shader.ParseShader(source);
		return shader;
	}

	const void Use() const
	{
		if (shaderProgramID == -1)
			throw ShaderError("Shader program was -1. This means the program could not link or create properly.");
		glUseProgram(shaderProgramID);
	}
};