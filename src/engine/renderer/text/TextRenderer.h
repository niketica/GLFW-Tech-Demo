#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/renderer/text/FreeTypeContext.h"
#include "engine/asset/File.h"
#include "engine/renderer/Shader.h"
#include "engine/renderer/text/Font.h"
#include "engine/asset/AssetManager.h"

namespace niketica::renderer
{
    struct TextVertex {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::vec4 color;
    };

    class TextRenderer
    {
    public:
        TextRenderer(float width, float height);
        ~TextRenderer();
        
        void init();
        void begin(const glm::mat4& projection, const Font& font);
        void submitText(const Font& font, const std::string& text,
            glm::vec2 pos, float scale, glm::vec4 color);
        void flush();

        Font* getSampleFont()
        {
            return sampleFont.get();
        }

    private:
        GLuint vao = 0;
        GLuint vbo = 0;
        std::unique_ptr<Shader> shader;

        TextVertex* mappedBuffer = nullptr;
        size_t vertexCount = 0;
        static constexpr size_t MAX_VERTICES = 100000;

        std::shared_ptr<Font> sampleFont;
    };
}
