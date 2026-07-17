#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <algorithm>
#include <glad/glad.h>

#include "component/Sprite.h"
#include "component/Transform.h"
#include "component/Color.h"
#include "component/UserInterface.h"
#include "engine/renderer/Shader.h"
#include "engine/renderer/TextureLoader.h"

namespace niketica::renderer {

    struct NineSliceTexture
    {
        niketica::component::TextureHandle texture;
        float width;   // full texture width
        float height;  // full texture height
    };

    class SpriteInstancedRenderer
    {
    public:
        SpriteInstancedRenderer(TextureLoader* textureLoader);
        ~SpriteInstancedRenderer();

        void submit(const unsigned int textureID, const niketica::component::Sprite& sprite, const glm::vec3& position, const glm::vec2& size,
            const float scale);
        void submitUI(const unsigned int textureID, const niketica::component::Sprite& sprite, const glm::vec3& position, const glm::vec2& size,
            const float scale);
        void submitNineSlice(
            const niketica::component::Transform& tr,
            const niketica::component::UINineSlice& ns,
            const NineSliceTexture& tex,
            const float scale);

        void clear();
        void clearUI();

        void render(const glm::mat4& projection, const glm::mat4& view);
        void renderUI(const glm::mat4& projection, const glm::mat4& view);

    private:

        TextureLoader* textureLoader;

        struct InstanceData
        {
            glm::vec2 pos;
            glm::vec2 size;
            glm::vec2 uvOffset;
            glm::vec2 uvScale;
            glm::vec4 color;
            float z;
            float rotation;
            int textureID;
        };

        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int instanceVBO;

        std::unique_ptr<Shader> shader;
        std::vector<InstanceData> sprites;
        std::vector<InstanceData> spritesUI;
        std::vector<InstanceData> instanceBuffer;

        void initQuad();
        void submitQuad(
            float x,
            float y,
            float w,
            float h,
            const NineSliceTexture& tex,
            const glm::vec2& uvOffset,
            const glm::vec2& uvScale,
            float z);
    };

}
