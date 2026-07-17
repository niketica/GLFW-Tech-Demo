#pragma once

#include <iostream>
#include <filesystem>

#include "engine/asset/IAssetLoader.h"
#include "engine/asset/PakReader.h"
#include "engine/asset/File.h"

namespace niketica::asset
{
	class FileLoader : public IAssetLoader {
	public:
		explicit FileLoader(PakReader* pakReader) : pakReader(pakReader) {};
		~FileLoader() override = default;

		std::shared_ptr<void> Load(const std::string& assetPath, void* context) override;

	private:
		PakReader* pakReader;

		std::filesystem::path getExecutablePath();

	};
}
