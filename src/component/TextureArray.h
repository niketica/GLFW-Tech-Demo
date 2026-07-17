#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

#include "component/SpriteAnimation.h"

namespace niketica::component
{
    enum class TextureArrayFormat
    {
        RGBA8,
        DXT5
    };

    struct TextureArrayHandle
    {
        unsigned int id;
        uint32_t mipLevels;
        SpriteType type = SpriteType::UNDEFINED;
        float layerZ = 0.0f;
    };

    struct UploadDDSRequest
    {
        SpriteType type = SpriteType::UNDEFINED;
        std::string filePath;
    };

    struct RegisterSpriteSetRequest
    {
        CharacterType characterType; // HUMAN_MALE, HUMAN_FEMALE...
        SpriteType spriteType;   // SKIN, ARMOR...
        AnimationType animation; // RUN, WALK
        uint32_t variant;        // 0,1,2...
        std::vector<std::string> sheetPaths;
    };

    struct UploadRuntimeTextureRequest
    {
        SpriteType type = SpriteType::UNDEFINED;
        unsigned int sourceTexture;
    };

    struct RenderTextureArray
    {
        SpriteType type = SpriteType::UNDEFINED;
        const glm::vec4 uv;
        uint32_t color;
    };

    struct InstanceData
    {
        glm::vec2 pos;
        glm::vec2 size;
        glm::vec4 uv;
        uint32_t color;
        float z;
        uint32_t layer;
    };

    struct SpriteBatchCache
    {
        std::unordered_map<SpriteType, std::vector<InstanceData>> batches;
        bool dirty = true;
    };

}
