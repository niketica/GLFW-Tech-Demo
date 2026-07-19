#pragma once

#include <array>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "engine/adapter/asset/File.h"
#include "engine/adapter/asset/AssetManager.h"
#include "engine/adapter/renderer/text/FreeTypeContext.h"

namespace niketica::renderer
{
    struct Glyph {
        glm::vec2 uvMin;
        glm::vec2 uvMax;
        glm::ivec2 size;
        glm::ivec2 bearing;
        uint32_t advance;
    };

    class Font {
    public:
        GLuint atlasTexture = 0;
        uint32_t atlasWidth = 0;
        uint32_t atlasHeight = 0;

        std::array<Glyph, 128> glyphs;
        float lineHeight = 0.0f;

        static std::shared_ptr<Font> load(const std::string& path, uint32_t pixelSize);
    };
}
