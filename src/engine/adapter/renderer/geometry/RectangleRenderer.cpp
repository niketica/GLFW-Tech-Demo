#include "engine/adapter/renderer/geometry/RectangleRenderer.h"

namespace niketica::renderer
{
    RectangleRenderer::RectangleRenderer()
    {
        init();
    }
    
    void RectangleRenderer::init()
    {
        borderRenderer = std::make_unique<RectangleBorderRenderer>();
        borderlessRenderer = std::make_unique<RectangleBorderlessRenderer>();
    }

    void RectangleRenderer::submit(const niketica::component::Rectangle& rectangle)
    {
        borderlessRenderer->submit(rectangle);
    }

    void RectangleRenderer::submit(const niketica::component::RectangleBorder& rectangle)
    {
        borderRenderer->submit(rectangle);
    }

    void RectangleRenderer::render
    (
        const glm::mat4& projection,
        const glm::mat4& view
    )
    {
        borderlessRenderer->render(projection, view);
        borderRenderer->render(projection, view);
    }

    void RectangleRenderer::clear()
    {
        borderlessRenderer->clear();
        borderRenderer->clear();
    }
    
}
