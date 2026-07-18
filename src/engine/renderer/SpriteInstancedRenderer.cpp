#include "engine/renderer/SpriteInstancedRenderer.h"

namespace niketica::renderer {

    SpriteInstancedRenderer::SpriteInstancedRenderer(TextureLoader* textureLoader)
        : textureLoader(textureLoader)
    {
        shader = std::make_unique<Shader>(
            "shaders/sprite_instanced.vert",
            "shaders/sprite_instanced.frag"
        );

        instanceBuffer.reserve(20000);

        initQuad();
    }

    SpriteInstancedRenderer::~SpriteInstancedRenderer()
    {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteBuffers(1, &instanceVBO);
    }

    void SpriteInstancedRenderer::submit(const unsigned int textureID, const niketica::component::Sprite& sprite, const glm::vec3& position, const glm::vec2& size,
        const float scale)
    {
        InstanceData data;
        data.pos = { position.x, position.y };

        data.size = { size.x, size.y };
        data.size *= scale;

        data.uvOffset = { sprite.uv.x, sprite.uv.y };
        data.uvScale = { sprite.uv.z, sprite.uv.w };

        data.z = position.z; // TODO add z to position
        data.rotation = 0.0f;
        data.color = { 1,1,1,1 }; // TODO remove color tinting from shader
        data.textureID = textureLoader->getGLTextureID(textureID);

        sprites.push_back(data);
    }

    void SpriteInstancedRenderer::submitUI(const unsigned int textureID, const niketica::component::Sprite& sprite, const glm::vec3& position, const glm::vec2& size,
        const float scale)
    {
        InstanceData data;
        data.pos = { position.x, position.y };

        data.size = { size.x, size.y };
        data.size *= scale;

        data.uvOffset = { sprite.uv.x, sprite.uv.y };
        data.uvScale = { sprite.uv.z, sprite.uv.w };
        data.z = position.z; // TODO add z to position
        data.rotation = 0.0f;
        data.color = { 1,1,1,1 }; // TODO remove color tinting from shader
        data.textureID = textureLoader->getGLTextureID(textureID);

        spritesUI.push_back(data);
    }

    void SpriteInstancedRenderer::clear()
    {
        sprites.clear();
    }

    void SpriteInstancedRenderer::clearUI()
    {
        spritesUI.clear();
    }

    void SpriteInstancedRenderer::initQuad()
    {
        // Basic quad covering (0,0) to (1,1)
        float vertices[] = {
            // x, y,   u, v
            0.f, 0.f,  0.f, 0.f,
            1.f, 0.f,  1.f, 0.f,
            0.f, 1.f,  0.f, 1.f,

            0.f, 1.f,  0.f, 1.f,
            1.f, 0.f,  1.f, 0.f,
            1.f, 1.f,  1.f, 1.f,
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glGenBuffers(1, &instanceVBO);

        glBindVertexArray(quadVAO);

        // Quad vertices (static)
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        // UVs (unused in instanced shader but provided for completeness)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // Instance data buffer (dynamic)
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * 20000, nullptr, GL_DYNAMIC_DRAW);

        // Instance attributes
        std::size_t offset = 0;

        // pos (vec2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
        glVertexAttribDivisor(2, 1);
        offset += sizeof(glm::vec2);

        // size (vec2)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
        glVertexAttribDivisor(3, 1);
        offset += sizeof(glm::vec2);

        // uvOffset (vec2)
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
        glVertexAttribDivisor(4, 1);
        offset += sizeof(glm::vec2);

        // uvScale (vec2)
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
        glVertexAttribDivisor(5, 1);
        offset += sizeof(glm::vec2);

        // color (vec4)
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
        glVertexAttribDivisor(6, 1);
        offset += sizeof(glm::vec4);

        // z (float)
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
        glVertexAttribDivisor(7, 1);
        offset += sizeof(float);

        // rotation (float)
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offset);
        glVertexAttribDivisor(8, 1);

        glBindVertexArray(0);
    }

    void SpriteInstancedRenderer::render(const glm::mat4& projection, const glm::mat4& view)
    {
        if (sprites.empty())
        {
            return;
        }

        instanceBuffer.clear();

        // Sort by z (back to front)
        std::sort(sprites.begin(), sprites.end(),
            [](const InstanceData& a, const InstanceData& b) {
                return a.z < b.z;
            }
        );

        shader->use();
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);

        glBindVertexArray(quadVAO);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        unsigned int currentTexture = 0;
        size_t instanceCount = 0;

        for (const auto& sprite : sprites)
        {
            if (sprite.textureID != currentTexture && instanceCount > 0)
            {
                // Flush previous batch
                glBindTexture(GL_TEXTURE_2D, currentTexture);

                glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
                void* ptr = glMapBufferRange(
                    GL_ARRAY_BUFFER,
                    0,
                    instanceBuffer.size() * sizeof(InstanceData),
                    GL_MAP_WRITE_BIT |
                    GL_MAP_INVALIDATE_BUFFER_BIT
                );
                memcpy(ptr, instanceBuffer.data(),
                    instanceBuffer.size() * sizeof(InstanceData));
                glUnmapBuffer(GL_ARRAY_BUFFER);

                glDrawArraysInstanced(GL_TRIANGLES, 0, 6,
                    static_cast<GLsizei>(instanceBuffer.size()));

                instanceBuffer.clear();
                instanceCount = 0;
            }

            currentTexture = sprite.textureID;
            instanceBuffer.push_back(sprite);
            instanceCount++;
        }

        // Flush last batch
        if (!instanceBuffer.empty())
        {
            glBindTexture(GL_TEXTURE_2D, currentTexture);

            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                instanceBuffer.size() * sizeof(InstanceData),
                instanceBuffer.data()
            );

            glDrawArraysInstanced(GL_TRIANGLES, 0, 6,
                static_cast<GLsizei>(instanceBuffer.size()));
        }

    }

    void SpriteInstancedRenderer::renderUI(const glm::mat4& projection, const glm::mat4& view)
    {
        if (spritesUI.empty()) return;

        instanceBuffer.clear();

        // Sort by z (back to front)
        std::sort(spritesUI.begin(), spritesUI.end(),
            [](const InstanceData& a, const InstanceData& b) {
                return a.z < b.z;
            }
        );

        shader->use();
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);

        glBindVertexArray(quadVAO);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        unsigned int currentTexture = 0;
        size_t instanceCount = 0;

        for (const auto& sprite : spritesUI)
        {
            if (sprite.textureID != currentTexture && instanceCount > 0)
            {
                // Flush previous batch
                glBindTexture(GL_TEXTURE_2D, currentTexture);

                glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
                void* ptr = glMapBufferRange(
                    GL_ARRAY_BUFFER,
                    0,
                    instanceBuffer.size() * sizeof(InstanceData),
                    GL_MAP_WRITE_BIT |
                    GL_MAP_INVALIDATE_BUFFER_BIT
                );
                memcpy(ptr, instanceBuffer.data(),
                    instanceBuffer.size() * sizeof(InstanceData));
                glUnmapBuffer(GL_ARRAY_BUFFER);

                glDrawArraysInstanced(GL_TRIANGLES, 0, 6,
                    static_cast<GLsizei>(instanceBuffer.size()));

                instanceBuffer.clear();
                instanceCount = 0;
            }

            currentTexture = sprite.textureID;
            instanceBuffer.push_back(sprite);
            instanceCount++;
        }

        // Flush last batch
        if (!instanceBuffer.empty())
        {
            glBindTexture(GL_TEXTURE_2D, currentTexture);

            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                instanceBuffer.size() * sizeof(InstanceData),
                instanceBuffer.data()
            );

            glDrawArraysInstanced(GL_TRIANGLES, 0, 6,
                static_cast<GLsizei>(instanceBuffer.size()));
        }

    }

    void SpriteInstancedRenderer::submitNineSlice(
        const component::Transform& tr,
        const component::UINineSlice& ns,
        const NineSliceTexture& tex,
        const float scale)
    {
        const float x = tr.position.x * scale;
        const float y = tr.position.y * scale;
        const float z = tr.position.z;
        const float w = tr.size.x * scale;
        const float h = tr.size.y * scale;

        const float L = ns.left;
        const float R = ns.right;
        const float T = ns.top;
        const float B = ns.bottom;

        const float midW = std::max(0.0f, w - L - R);
        const float midH = std::max(0.0f, h - T - B);

        const float texW = tex.width;
        const float texH = tex.height;

        // Sprite rect in UV space (top-left origin)
        const float spriteU0 = ns.spriteOffset.x / texW;
        const float spriteV0 = ns.spriteOffset.y / texH;
        const float spriteU1 = (ns.spriteOffset.x + ns.spriteSize.x) / texW;
        const float spriteV1 = (ns.spriteOffset.y + ns.spriteSize.y) / texH;

        // Convert to OpenGL bottom-left UV space
        const float spriteVTop = 1.0f - spriteV0;
        const float spriteVBottom = 1.0f - spriteV1;

        // Border sizes in UV space
        const float uL = L / texW;
        const float uR = R / texW;
        const float vT = T / texH;
        const float vB = B / texH;

        // Inner (stretchable) UV sizes
        const float uMid = (ns.spriteSize.x - L - R) / texW;
        const float vMid = (ns.spriteSize.y - T - B) / texH;

        // --- Bottom Left
        submitQuad(
            x, y,
            L, B,
            tex,
            { spriteU0, spriteVBottom },
            { uL, vB },
            z
        );

        // --- Bottom
        submitQuad(
            x + L, y,
            midW, B,
            tex,
            { spriteU0 + uL, spriteVBottom },
            { uMid, vB },
            z
        );

        // --- Bottom Right
        submitQuad(
            x + L + midW, y,
            R, B,
            tex,
            { spriteU1 - uR, spriteVBottom },
            { uR, vB },
            z
        );

        // --- Left
        submitQuad(
            x, y + B,
            L, midH,
            tex,
            { spriteU0, spriteVBottom + vB },
            { uL, vMid },
            z
        );

        // --- Center
        submitQuad(
            x + L, y + B,
            midW, midH,
            tex,
            { spriteU0 + uL, spriteVBottom + vB },
            { uMid, vMid },
            z
        );

        // --- Right
        submitQuad(
            x + L + midW, y + B,
            R, midH,
            tex,
            { spriteU1 - uR, spriteVBottom + vB },
            { uR, vMid },
            z
        );

        // --- Top Left
        submitQuad(
            x, y + B + midH,
            L, T,
            tex,
            { spriteU0, spriteVTop - vT },
            { uL, vT },
            z
        );

        // --- Top
        submitQuad(
            x + L, y + B + midH,
            midW, T,
            tex,
            { spriteU0 + uL, spriteVTop - vT },
            { uMid, vT },
            z
        );

        // --- Top Right
        submitQuad(
            x + L + midW, y + B + midH,
            R, T,
            tex,
            { spriteU1 - uR, spriteVTop - vT },
            { uR, vT },
            z
        );
    }

    void SpriteInstancedRenderer::submitQuad(
        float x,
        float y,
        float w,
        float h,
        const NineSliceTexture& tex,
        const glm::vec2& uvOffset,
        const glm::vec2& uvScale,
        float z)
    {
        component::Sprite sprite{};

        sprite.uv =
        {
            uvOffset.x,
            1.0f - uvOffset.y - uvScale.y,
            uvScale.x,
            uvScale.y
        };

        component::Transform transform{};
        transform.position = { x, y, z };
        transform.size = { w, h, 1.0f };
        transform.scale = { 1.0f, 1.0f, 1.0f };

        submitUI(tex.texture.id, sprite, transform.position, transform.size, 1.0f);
    }

}
