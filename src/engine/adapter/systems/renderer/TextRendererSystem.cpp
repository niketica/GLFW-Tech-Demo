#include "engine/adapter/systems/renderer/TextRendererSystem.h"

namespace niketica::systems
{

    void TextRendererSystem::render()
    {
        auto windowView = registry->view<niketica::component::Window>();
        const auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());

        auto viewText = registry->view<niketica::component::Text, niketica::component::Transform>();
        bool first = true;

        for (auto entity : viewText)
        {
            const auto& text = registry->get<niketica::component::Text>(entity);    
            if (first)
            {
                engineServices->getRenderContext()->getTextRenderer()->begin(windowComponent.projection, niketica::component::FontType::OPEN_SANS_REGULAR, text.fontSize);
                first = false;
            }
                 
            const auto& transform = registry->get<niketica::component::Transform>(entity);           
            engineServices->getRenderContext()->getTextRenderer()->submitText(
                niketica::component::FontType::OPEN_SANS_REGULAR,
                text.fontSize,
                text.value,
                glm::vec2{ transform.position.x, transform.position.y },
                text.scale,
                text.color
            );
        }
        engineServices->getRenderContext()->getTextRenderer()->flush();
    }

}
