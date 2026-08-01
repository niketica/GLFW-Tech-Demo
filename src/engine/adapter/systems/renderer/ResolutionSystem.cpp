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

            const auto& uiReferenceResolution = renderSettings.uiReferenceResolution;
            float scaleX = uiReferenceResolution.x / 1920.0f;
            float scaleY = uiReferenceResolution.y / 1080.0f;

            auto calculatedPosition = calculateTextPosition({ uiReferenceResolution.x, uiReferenceResolution.y }, { scaleX, scaleY }, anchor);
            auto calculatedSize = calculateTextSize({ scaleX, scaleY }, size);

            text.fontSize = (int)calculatedSize.y;
            local.position = { calculatedPosition.x, calculatedPosition.y, 0.0f };
            local.size = { calculatedSize.x, calculatedSize.y, 0.0f };
        }
    }

    glm::vec2 ResolutionSystem::calculateTextPosition(const glm::vec2& resolution, const glm::vec2& scale, const niketica::component::UIAnchor& anchor) const
    {
        float offsetX = anchor.offset.x * scale.x;
        float offsetY = anchor.offset.y * scale.y;

        float posX = 0.0f;
        float posY = 0.0f;

        switch (anchor.horizontal)
        {
        case niketica::component::AlignmentHorizontal::LEFT:
            posX = 0.0f + offsetX;
            break;
        case niketica::component::AlignmentHorizontal::RIGHT:
            posX = resolution.x + offsetX;
            break;
        case niketica::component::AlignmentHorizontal::CENTER:
            posX = (resolution.x * 0.5f) + offsetX;
            break;
        }

        switch (anchor.vertical)
        {
        case niketica::component::AlignmentVertical::TOP:
            posY = resolution.y + offsetY;
            break;
        case niketica::component::AlignmentVertical::BOTTOM:
            posY = 0.0f + offsetY;
            break;
        case niketica::component::AlignmentVertical::CENTER:
            posY = (resolution.y * 0.5f) + offsetY;
            break;
        }

        return { posX, posY };
    }

    glm::vec2 ResolutionSystem::calculateTextSize(const glm::vec2& scale, const niketica::component::UISize& size) const
    {
        float sizeX = 0.0f;
        float sizeY = 0.0f;
        switch (size.widthMode)
        {
        case niketica::component::UISizeMode::PIXELS:
            sizeX = size.width * scale.x;
            break;
        case niketica::component::UISizeMode::PERCENT:
            sizeX = size.width * scale.x; // TODO calc percentage of window height?
            break;
        }
        
        switch (size.heightMode)
        {
        case niketica::component::UISizeMode::PIXELS:
            sizeY = size.height * scale.y;
            break;
        case niketica::component::UISizeMode::PERCENT:
            sizeY = size.height * scale.y; // TODO calc percentage of window height?
            break;
        }

        return { sizeX, sizeY };
    }

}
