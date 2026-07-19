#pragma once

#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "component/TextureHandle.h"
#include "engine/core/renderer/ITextureLoader.h"
#include "engine/adapter/asset/File.h"
#include "engine/adapter/asset/AssetManager.h"
#include "engine/adapter/renderer/TextureArray.h"

namespace niketica::renderer
{
    struct TextureResource
    {
        GLuint glTextureID;
        std::string path;
    };

    // --- DDS HEADER STRUCTS ---
    struct DDS_HEADER {
        uint32_t size;
        uint32_t flags;
        uint32_t height;
        uint32_t width;
        uint32_t pitchOrLinearSize;
        uint32_t depth;
        uint32_t mipMapCount;
        uint32_t reserved1[11];
        struct {
            uint32_t size;
            uint32_t flags;
            uint32_t fourCC;
            uint32_t rgbBitCount;
            uint32_t rMask;
            uint32_t gMask;
            uint32_t bMask;
            uint32_t aMask;
        } ddspf;
        uint32_t caps;
        uint32_t caps2;
        uint32_t caps3;
        uint32_t caps4;
        uint32_t reserved2;
    };

    struct MemoryReader
    {
        const uint8_t* ptr;
        const uint8_t* end;

        MemoryReader(const uint8_t* data, size_t size)
            : ptr(data), end(data + size) {
        }

        template<typename T>
        bool read(T& out)
        {
            if (ptr + sizeof(T) > end)
                return false;
            std::memcpy(&out, ptr, sizeof(T));
            ptr += sizeof(T);
            return true;
        }

        bool readBytes(void* dst, size_t size)
        {
            if (ptr + size > end)
                return false;
            std::memcpy(dst, ptr, size);
            ptr += size;
            return true;
        }
    };

    class TextureLoader : public ITextureLoader
    {
    public:
        TextureLoader();
        ~TextureLoader();

        niketica::component::TextureHandle acquire(const char* path, bool compressed = true) override;
        void release(const niketica::component::TextureHandle& textureHandle);

        // GLuint getGLTextureID(unsigned int textureID);
        unsigned int getAdapterTextureID(unsigned int textureID) override;

        unsigned long long getNrOfTextures() const
        {
            return textureIDMap.size() + nrOfDDSUploadedIntoArray;
        }

        bool uploadDDSIntoArrayLayer(
            niketica::renderer::TextureArray& array,
            uint16_t layer,
            const uint8_t* data,
            size_t dataSize);

        bool uploadDDSIntoArrayLayer(
            GLuint glTextureArrayID,
            uint32_t mipLevels,
            uint16_t layer,
            const uint8_t* data,
            size_t dataSize);

        component::TextureHandle addTexture(const char* path, unsigned int glTextureID);

    private:
        static const uint32_t DDS_MAGIC = 0x20534444;
        static const unsigned int MAX_NR_OF_TEXTURES = 10000;

        int nrOfDDSUploadedIntoArray = 0;

        std::vector<unsigned int> textureIDs;
        std::unordered_map<unsigned int, TextureResource> textureIDMap;
        std::unordered_map<std::string, unsigned int> pathToTextureIDMap;

        unsigned int loadUncompressedTexture(const char* path);
        unsigned int loadCompressedTexture(const char* path);

        unsigned int loadCompressedTextureFromMemory(
            const uint8_t* data,
            size_t dataSize,
            const char* debugName
        );
    };
}
