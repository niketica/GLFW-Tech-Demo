#pragma once

#include <memory>

namespace niketica::asset
{
    template<typename T>
    class AssetHandle {
    public:
        AssetHandle() = default;
        AssetHandle(std::shared_ptr<T> asset) : m_Asset(std::move(asset)) {}
        T* operator->() const { return m_Asset.get(); }
        T* get() const { return m_Asset.get(); }
        T& operator*() const { return *m_Asset; }
        bool IsValid() const { return m_Asset != nullptr; }

    private:
        std::shared_ptr<T> m_Asset;
    };
}
