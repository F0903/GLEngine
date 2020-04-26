#pragma once

#include <string>

inline std::string& ToLower(std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		str[i] = std::tolower(str[i]);
	}
	return str;
}

inline std::string GetFileName(std::string path, bool keepExtension = true)
{
	auto offset = path.find_last_of("/\\");
	auto fileName = path.erase(0, offset + 1);
	if (!keepExtension)
	{
		auto extOffset = fileName.find('.');
		fileName = fileName.erase(extOffset, fileName.size() - extOffset);
	}
	return fileName;
}
