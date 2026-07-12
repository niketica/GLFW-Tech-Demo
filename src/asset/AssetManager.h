#pragma once

#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "asset/AssetHandle.h"
#include "asset/IAssetLoader.h"

namespace niketica::asset
{
    class AssetManager {
    public:
        static AssetManager& Get() {
            static AssetManager instance;
            return instance;
        }

        template<typename T>
        AssetHandle<T> Load(const std::string& path, void* context = nullptr) {
            std::string normalizedPath = normalizePath(path);
            size_t typeHash = typeid(T).hash_code();
            size_t fullHash = std::hash<std::string>{}(normalizedPath) ^ (typeHash << 1);

            auto it = m_AssetMap.find(fullHash);
            if (it != m_AssetMap.end()) {
                return std::static_pointer_cast<T>(it->second);
            }

            auto loader = GetLoader<T>();
            if (!loader) {
                throw std::runtime_error("No loader for asset type.");
            }

            auto asset = std::static_pointer_cast<T>(loader->Load(normalizedPath, context));
            m_AssetMap[fullHash] = asset;
            return AssetHandle<T>(asset);
        }

        template<typename T>
        void RegisterLoader(std::shared_ptr<IAssetLoader> loader) {
            m_Loaders[typeid(T).hash_code()] = loader;
        }

    private:
        std::unordered_map<size_t, std::shared_ptr<void>> m_AssetMap;
        std::unordered_map<size_t, std::shared_ptr<IAssetLoader>> m_Loaders;

        template<typename T>
        std::shared_ptr<IAssetLoader> GetLoader() {
            auto it = m_Loaders.find(typeid(T).hash_code());
            if (it != m_Loaders.end()) return it->second;
            return nullptr;
        }

        AssetManager() = default;

        std::string normalizePath(std::string path) const
        {
            std::transform(path.begin(), path.end(), path.begin(), [](char c) {
                return (c == '\\') ? '/' : std::tolower(c);
                });
            return path;
        }

    };
}
