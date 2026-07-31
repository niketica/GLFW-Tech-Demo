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

        mouseVirtual.x = u * renderSettings.virtualWidth;
        mouseVirtual.y = v * renderSettings.virtualHeight;

        return mouseVirtual;
    }

    // static inline const glm::vec2 screenToWorld
    // (
    //     const glm::vec2& windowPos,
    //     const niketica::component::Viewport& viewport,
    //     const niketica::component::Camera& camera,
    //     const niketica::component::RenderSettings& settings
    // )
    // {

    // }

}
