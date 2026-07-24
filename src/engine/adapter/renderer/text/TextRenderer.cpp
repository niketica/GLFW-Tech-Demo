#include "engine/adapter/renderer/text/TextRenderer.h"

namespace niketica::renderer
{
    TextRenderer::TextRenderer(float width, float height)
    {
        shader = std::make_unique<Shader>(
            "shaders/text.vert",
            "shaders/text.frag"
        );

        sampleFont = Font::load("fonts/OpenSans-Regular.ttf", 14);

        init();
    }

    TextRenderer::~TextRenderer()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);

        try
        {
            renderer::FreeTypeContext::release();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception during FreeTypeContext release: " << e.what() << std::endl;
        }
    }

    void TextRenderer::init()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferStorage(GL_ARRAY_BUFFER,
            MAX_VERTICES * sizeof(TextVertex),
            nullptr,
            GL_MAP_WRITE_BIT |
            GL_MAP_PERSISTENT_BIT |
            GL_MAP_COHERENT_BIT);

        mappedBuffer = (TextVertex*)glMapBufferRange(
            GL_ARRAY_BUFFER,
            0,
            MAX_VERTICES * sizeof(TextVertex),
            GL_MAP_WRITE_BIT |
            GL_MAP_PERSISTENT_BIT |
            GL_MAP_COHERENT_BIT
        );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)8);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)16);
    }

    void TextRenderer::begin(const glm::mat4& projection, const niketica::component::FontType& font)
    {

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        vertexCount = 0;

        shader->use();
        shader->setMat4("uProjection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, getSampleFont()->atlasTexture);
        shader->setInt("uFontAtlas", 0);

        glBindVertexArray(vao);

        if (vao == 0)
        {
            std::cerr << "ERROR::TextRenderer::begin - VAO not initialized!" << std::endl;
        }
    }

    void TextRenderer::submitText(
        const niketica::component::FontType& font,
        const std::string& text,
        glm::vec2 pos,
        float scale,
        glm::vec4 color)
    {
        float x = pos.x;
        float y = pos.y;

        for (unsigned char c : text)
        {
            const Glyph& g = getSampleFont()->glyphs[c];

            // Pixel-space glyph position
            float xpos = x + g.bearing.x * scale;
            float ypos = y + (g.bearing.y - g.size.y) * scale;

            float w = g.size.x * scale;
            float h = g.size.y * scale;

            // Screen-space quad (pixel coordinates)
            TextVertex vertices[6] = {
                {{xpos,     ypos + h}, g.uvMin,                         color},
                {{xpos,     ypos},     {g.uvMin.x, g.uvMax.y},          color},
                {{xpos + w, ypos},     g.uvMax,                         color},

                {{xpos,     ypos + h}, g.uvMin,                         color},
                {{xpos + w, ypos},     g.uvMax,                         color},
                {{xpos + w, ypos + h}, {g.uvMax.x, g.uvMin.y},          color},
            };

            memcpy(mappedBuffer + vertexCount, vertices, sizeof(vertices));
            vertexCount += 6;

            // Advance cursor (FreeType advance is in 1/64 pixels)
            x += (g.advance >> 6) * scale;
        }
    }

    void TextRenderer::flush()
    {
        if (vertexCount == 0) return;

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        vertexCount = 0;
    }

}
