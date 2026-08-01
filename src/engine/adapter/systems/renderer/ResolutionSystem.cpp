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
        
        updateText(renderSettings);
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

    void ResolutionSystem::updateText(const niketica::component::RenderSettings& renderSettings)
    {
        auto viewUIStuff = registry->view<niketica::component::UIAnchor, niketica::component::UISize, niketica::component::LocalTransform, niketica::component::Text>();
        for (auto entity : viewUIStuff)
        {
            const auto& anchor = viewUIStuff.get<niketica::component::UIAnchor>(entity);
            const auto& size = viewUIStuff.get<niketica::component::UISize>(entity);
            auto& local = viewUIStuff.get<niketica::component::LocalTransform>(entity);
            auto& text = viewUIStuff.get<niketica::component::Text>(entity);

            float posX = 0.0f;
            float posY = 0.0f;

            const auto& uiReferenceResolution = renderSettings.uiReferenceResolution;

            float scaleX = uiReferenceResolution.x / 1920.0f;
            float scaleY = uiReferenceResolution.y / 1080.0f;

            float offsetX = anchor.offset.x * scaleX;
            float offsetY = anchor.offset.y * scaleY;

            switch (anchor.horizontal)
            {
            case niketica::component::AlignmentHorizontal::LEFT:
                posX = 0.0f + offsetX;
                break;
            case niketica::component::AlignmentHorizontal::RIGHT:
                posX = uiReferenceResolution.x + offsetX;
                break;
            case niketica::component::AlignmentHorizontal::CENTER:
                posX = (uiReferenceResolution.x * 0.5f) + offsetX;
                break;
            }

            switch (anchor.vertical)
            {
            case niketica::component::AlignmentVertical::TOP:
                posY = uiReferenceResolution.y + offsetY;
                break;
            case niketica::component::AlignmentVertical::BOTTOM:
                posY = 0.0f + offsetY;
                break;
            case niketica::component::AlignmentVertical::CENTER:
                posY = (uiReferenceResolution.y * 0.5f) + offsetY;
                break;
            }

            float sizeX = 0.0f;
            float sizeY = 0.0f;
            switch (size.widthMode)
            {
            case niketica::component::UISizeMode::PIXELS:
                sizeX = size.width * scaleX;
                break;
            case niketica::component::UISizeMode::PERCENT:
                sizeX = size.width * scaleX; // TODO calc percentage of window height?
                break;
            }
            
            switch (size.heightMode)
            {
            case niketica::component::UISizeMode::PIXELS:
                sizeY = size.height * scaleY;
                break;
            case niketica::component::UISizeMode::PERCENT:
                sizeY = size.height * scaleX; // TODO calc percentage of window height?
                break;
            }

            text.fontSize = (int)sizeY;
            local.position = { posX, posY, 0.0f };
            local.size = { sizeX, sizeY, 0.0f };
        }
    }

}
