#include "systems/renderer/RectangleRenderSystem.h"

namespace niketica::systems
{

    void RectangleRenderSystem::update(float dt)
    {
        auto renderer = engineServices->getRenderContext()->getRectangleRenderer();
        renderer->clear();

        auto viewRect = registry->view<niketica::component::Rectangle, niketica::component::Transform>();
        for (auto entity : viewRect)
        {
            const bool hasBorder = registry->all_of<niketica::component::BorderColor, niketica::component::BorderThickness>(entity);
            const bool hasFill = registry->all_of<niketica::component::FillColor>(entity);

            const auto& transform = viewRect.get<niketica::component::Transform>(entity);

            if (hasBorder)
            {
                const auto& borderColor = registry->get<niketica::component::BorderColor>(entity);
                const auto& borderThickness = registry->get<niketica::component::BorderThickness>(entity);
                if (hasFill)
                {
                    const auto& fillColor = registry->get<niketica::component::FillColor>(entity);
                    auto rectBorderData = niketica::renderer::RectangleBorderData{
                        transform.position,
                        transform.size,
                        fillColor.color,
                        borderColor.color,
                        borderThickness.thickness,
                        1.0f
                    };
                    renderer->submit(rectBorderData);                    
                }
                else
                {
                    auto rectBorderData = niketica::renderer::RectangleBorderData{
                        transform.position,
                        transform.size,
                        {},
                        borderColor.color,
                        borderThickness.thickness,
                        0.0f
                    };
                    renderer->submit(rectBorderData);
                }
            }
            else if (hasFill)
            {
                const auto& fillColor = registry->get<niketica::component::FillColor>(entity);
                auto rectData = niketica::renderer::RectangleBorderlessData{
                    transform.position,
                    transform.size,
                    fillColor.color
                };
                renderer->submit(rectData);                
            }
        }
    }

    void RectangleRenderSystem::render()
    {
        // For now just assume there is always exactly 1 active camera.
        auto viewCamera = registry->view<niketica::component::Camera, niketica::component::ActiveCamera>();
        const auto &camera = viewCamera.get<niketica::component::Camera>(viewCamera.front());
        engineServices->getRenderContext()->getRectangleRenderer()->render(camera.projection, camera.view);        
    }

}
