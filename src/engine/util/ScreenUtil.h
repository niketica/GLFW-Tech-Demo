#pragma once

#include <glm/glm.hpp>

#include "component/Components.h"

namespace niketica::util
{

    static inline const glm::vec2 screenToVirtual
    (
        const glm::vec2& windowPos,
        const niketica::component::Viewport& viewport,
        const niketica::component::RenderSettings& renderSettings
    )
    {
        glm::vec2 mouseViewport;
        glm::vec2 mouseVirtual;

        mouseViewport.x = windowPos.x - viewport.x;
        mouseViewport.y = windowPos.y - viewport.y;

        float u = mouseViewport.x / viewport.width;
        float v = mouseViewport.y / viewport.height;

        mouseVirtual.x = u * renderSettings.uiReferenceResolution.x;
        mouseVirtual.y = v * renderSettings.uiReferenceResolution.y;

        return mouseVirtual;
    }

    static inline const glm::vec2 screenToWorld
    (
        const glm::vec2& windowPos,
        const niketica::component::Viewport& viewport,
        const niketica::component::Camera& camera,
        const niketica::component::RenderSettings& settings
    )
    {
        glm::vec2 p = screenToVirtual(windowPos, viewport, settings);
        glm::vec4 world = glm::inverse(camera.view) * glm::vec4(p.x, p.y, 0.0f, 1.0f);
        return { world.x, world.y };
    }

}
