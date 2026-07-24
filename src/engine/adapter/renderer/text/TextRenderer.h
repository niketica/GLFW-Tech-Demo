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

        Font* getSampleFont()
        {
            return sampleFont.get();
        }

        Font* getFont(uint32_t pixelSize)
        {
            auto it = fonts.find(pixelSize);            
            if (it == fonts.end())
            {
                fonts[pixelSize] = Font::load("fonts/OpenSans-Regular.ttf", pixelSize);
                return getFont(pixelSize);
            }
            else
            {
                return it->second.get();
            }
        }

    private:
        GLuint vao = 0;
        GLuint vbo = 0;
        std::unique_ptr<Shader> shader;

        TextVertex* mappedBuffer = nullptr;
        size_t vertexCount = 0;
        static constexpr size_t MAX_VERTICES = 100000;

        std::shared_ptr<Font> sampleFont;
        std::unordered_map<uint32_t, std::shared_ptr<Font>> fonts;
    };

}
