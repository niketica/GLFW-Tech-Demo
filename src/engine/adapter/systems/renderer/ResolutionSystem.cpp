#include "engine/adapter/systems/renderer/ResolutionSystem.h"

namespace niketica::systems
{
    void ResolutionSystem::update(float dt)
    {
        if (!isResolutionDirty()) return;

        auto viewWindow = registry->view<niketica::component::Window>();
        const auto& window = viewWindow.get<niketica::component::Window>(viewWindow.front());

        auto viewRenderSettings = registry->view<niketica::component::RenderSettings>();
        const auto& renderSettings = viewRenderSettings.get<niketica::component::RenderSettings>(viewRenderSettings.front());

        auto viewViewport = registry->view<niketica::component::Viewport>();
        auto& viewport = viewViewport.get<niketica::component::Viewport>(viewViewport.front());
        engineServices->getRenderContext()->updateViewport(viewport, window, renderSettings);
        
        auto viewCamera = registry->view<niketica::component::Camera>();
        for (auto cameraEntity : viewCamera)
        {
            auto& camera = viewCamera.get<niketica::component::Camera>(cameraEntity);
            engineServices->getRenderContext()->updateCamera(camera, renderSettings);
        }
        
        registry->emplace<niketica::component::UIGlobalLayoutDirty>(registry->create());
    }

    bool ResolutionSystem::isResolutionDirty() const
    {
        bool resolutionDirty = false;
        auto viewResolutionDirty = registry->view<niketica::component::ResolutionDirty>();
        for (auto entity : viewResolutionDirty)
        {
            registry->remove<niketica::component::ResolutionDirty>(entity);
            resolutionDirty = true;
        }
        return resolutionDirty;
    }

}
