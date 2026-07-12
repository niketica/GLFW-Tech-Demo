#pragma once

#include <iostream>
#include <filesystem>

#include "asset/IAssetLoader.h"
#include "asset/PakReader.h"
#include "asset/File.h"

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
