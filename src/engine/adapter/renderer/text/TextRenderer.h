#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/core/renderer/ITextRenderer.h"
#include "engine/adapter/asset/File.h"
#include "engine/adapter/asset/AssetManager.h"
#include "engine/adapter/renderer/text/FreeTypeContext.h"
#include "engine/adapter/renderer/Shader.h"
#include "engine/adapter/renderer/text/Font.h"

namespace niketica::renderer
{
    struct FontKey
    {
        niketica::component::FontType type;
        uint32_t pixelSize;

        bool operator==(const FontKey& other) const
        {
            return type == other.type &&
                    pixelSize == other.pixelSize;
        }
    };

    struct FontKeyHash
    {
        size_t operator()(const FontKey& k) const
        {
            return std::hash<int>()((int)k.type) ^
                    (std::hash<uint32_t>()(k.pixelSize) << 1);
        }
    };

    struct TextVertex {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::vec4 color;
    };

    class TextRenderer : public ITextRenderer
    {
    public:
        TextRenderer(float width, float height);
        ~TextRenderer();
        
        void init();
        void begin(const glm::mat4& projection, const niketica::component::FontType& font, uint32_t pixelSize) override;
        void submitText(const niketica::component::FontType& font, uint32_t pixelSize, const std::string& text,
            glm::vec2 pos, float scale, glm::vec4 color) override;
        void flush() override;

    private:
        GLuint vao = 0;
        GLuint vbo = 0;
        std::unique_ptr<Shader> shader;

        TextVertex* mappedBuffer = nullptr;
        size_t vertexCount = 0;
        static constexpr size_t MAX_VERTICES = 100000;

        std::unordered_map<FontKey, std::shared_ptr<Font>, FontKeyHash> fonts;
        std::unordered_map<niketica::component::FontType, std::string> fontPaths =
        {
            { niketica::component::FontType::OPEN_SANS_REGULAR, "fonts/OpenSans-Regular.ttf" },
            // {niketica::component::FontType::OPEN_SANS_BOLD,    "fonts/OpenSans-Bold.ttf"},
            // {niketica::component::FontType::ROBOTO,            "fonts/Roboto-Regular.ttf"},
        };

        Font* getFont(niketica::component::FontType type, uint32_t pixelSize);

    };

}
