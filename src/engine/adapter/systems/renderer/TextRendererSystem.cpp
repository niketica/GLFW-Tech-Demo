#include "engine/adapter/systems/renderer/TextRendererSystem.h"

namespace niketica::systems
{

    void TextRendererSystem::render()
    {
        auto windowView = registry->view<niketica::component::Window>();
        auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());

        engineServices->getRenderContext()->getTextRenderer()->begin(windowComponent.projection, niketica::component::FontType::OPEN_SANS_REGULAR);
        auto viewText = registry->view<niketica::component::Text>();
        for (auto entity : viewText)
        {
            auto& text = registry->get<niketica::component::Text>(entity);            
            engineServices->getRenderContext()->getTextRenderer()->submitText(
                niketica::component::FontType::OPEN_SANS_REGULAR,
                text.value,
                text.positionTopLeft,
                text.scale,
                text.color
            );
        }
        engineServices->getRenderContext()->getTextRenderer()->flush();
    }

}
