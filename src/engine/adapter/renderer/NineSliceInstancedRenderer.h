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
#include "engine/core/renderer/INineSliceInstancedRenderer.h"
#include "engine/core/renderer/ITextureLoader.h"
#include "engine/adapter/renderer/Shader.h"

namespace niketica::renderer {

    class NineSliceInstancedRenderer : public INineSliceInstancedRenderer
    {
    public:
        NineSliceInstancedRenderer(ITextureLoader* textureLoader);
        ~NineSliceInstancedRenderer();

        void submit(
            const niketica::component::Transform& tr,
            const niketica::component::NineSlice& ns,
            const niketica::component::NineSliceTexture& tex,
            const float scale) override;
        void clear() override;
        void render(const glm::mat4& projection, const glm::mat4& view) override;

    private:

        ITextureLoader* textureLoader;

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
            const niketica::component::NineSliceTexture& tex,
            const glm::vec2& uvOffset,
            const glm::vec2& uvScale,
            float z);
        void submitUI(const unsigned int textureID, const niketica::component::Sprite& sprite, const glm::vec3& position, const glm::vec2& size,
            const float scale);
    };

}
