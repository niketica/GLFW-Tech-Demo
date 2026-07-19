#include "engine/adapter/asset/FileLoader.h"

#if defined(_WIN32)
#include <io.h>
#define WIN32_LEAN_AND_MEAN
// #define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace niketica::asset
{
	std::shared_ptr<void> FileLoader::Load(const std::string& assetPath, void* context = nullptr)
	{
		auto exePath = getExecutablePath();
		auto systemPath = exePath.parent_path() / ASSET_FILE;
		pakReader->load(systemPath.string());
		auto fileData = pakReader->getFile(assetPath);
		File file{ fileData };
		return std::make_shared<File>(file);
	}

    std::filesystem::path FileLoader::getExecutablePath()
    {
#ifdef _WIN32
        char buffer[MAX_PATH];
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        return std::filesystem::path(buffer);
#else
        char buffer[1024];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer));
        if (len != -1)
        {
            return std::filesystem::path(std::string(buffer, len));
        }
        return {};
#endif
    }
}
