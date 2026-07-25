#include "engine/adapter/systems/renderer/TextRendererSystem.h"

namespace niketica::systems
{

    void TextRendererSystem::render()
    {
        auto windowView = registry->view<niketica::component::Window>();
        const auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());

        auto viewText = registry->view<niketica::component::Text, niketica::component::Transform>();

        // Collect all text components and batch by font type and font size.
        using BatchMap =
            std::unordered_map<
                TextBatchKey,
                std::vector<entt::entity>,
                TextBatchHash>;
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
            renderer->begin(windowComponent.projection, key.font, key.size);

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
