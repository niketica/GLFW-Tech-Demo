#pragma once

#include <memory>
#include <string>

namespace niketica::asset
{
    extern const char* ASSET_FILE;

    class IAssetLoader {
    public:
        virtual ~IAssetLoader() = default;
        virtual std::shared_ptr<void> Load(
            const std::string& path,
            void* context = nullptr
        ) = 0;
    };
}
