#include "systems/renderer/TextRendererSystem.h"

namespace niketica::systems
{

    void TextRendererSystem::render()
    {
        // For now just assume there is always exactly 1 active camera.
        auto viewCamera = registry->view<niketica::component::Camera, niketica::component::ActiveCamera>();
        const auto &camera = viewCamera.get<niketica::component::Camera>(viewCamera.front());

        auto viewText = registry->view<niketica::component::Text, niketica::component::Transform>();

        // Collect all text components and batch by font type and font size.
        using BatchMap = std::unordered_map<TextBatchKey, std::vector<entt::entity>, TextBatchHash>;
        BatchMap batches;
        for (auto entity : viewText)
        {
            const auto& text = viewText.get<niketica::component::Text>(entity);

            TextBatchKey key
            {
                text.fontType,
                (uint32_t)text.fontSize
            };

            batches[key].push_back(entity);
        }

        auto* renderer = engineServices->getRenderContext()->getTextRenderer();
        renderer->startFrame();
        for (const auto& [key, entities] : batches)
        {
            renderer->begin(camera.projection, key.font, key.size);

            for (auto entity : entities)
            {
                const auto& text = viewText.get<niketica::component::Text>(entity);
                const auto& transform = viewText.get<niketica::component::Transform>(entity);

                renderer->submitText(
                    text.fontType,
                    text.fontSize,
                    text.value,
                    {
                        transform.position.x,
                        transform.position.y
                    },
                    text.scale,
                    text.color
                );
            }

            renderer->flush();
        }
    }

}
