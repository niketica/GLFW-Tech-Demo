#pragma once

#include <string>
#include <algorithm>

namespace niketica::compressor
{
	static std::string normalizePath(std::string& path)
	{
		std::transform(path.begin(), path.end(), path.begin(), [](char c) {
			return (c == '\\') ? '/' : std::tolower(c);
			});
		return path;
	}
}
