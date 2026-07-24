#include "engine/adapter/systems/renderer/TextRendererSystem.h"

namespace niketica::systems
{

    void TextRendererSystem::render()
    {
        auto windowView = registry->view<niketica::component::Window>();
        auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());

        auto viewText = registry->view<niketica::component::Text>();
        if (viewText.empty())
        {
            return;
        }

        // TODO handle multiple different font types and font sizes
        
        auto& firstText = registry->get<niketica::component::Text>(viewText.front());
        engineServices->getRenderContext()->getTextRenderer()->begin(windowComponent.projection, niketica::component::FontType::OPEN_SANS_REGULAR, firstText.fontSize);

        for (auto entity : viewText)
        {
            auto& text = registry->get<niketica::component::Text>(entity);            
            engineServices->getRenderContext()->getTextRenderer()->submitText(
                niketica::component::FontType::OPEN_SANS_REGULAR,
                text.fontSize,
                text.value,
                text.positionTopLeft,
                text.scale,
                text.color
            );
        }
        engineServices->getRenderContext()->getTextRenderer()->flush();
    }

}
