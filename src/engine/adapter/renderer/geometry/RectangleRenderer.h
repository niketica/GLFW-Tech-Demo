#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "component/Components.h"
#include "engine/core/renderer/IRectangleRenderer.h"
#include "engine/adapter/renderer/Shader.h"
#include "engine/adapter/renderer/geometry/RectangleBorderRenderer.h"
#include "engine/adapter/renderer/geometry/RectangleBorderlessRenderer.h"

namespace niketica::renderer
{
    class RectangleRenderer : public IRectangleRenderer
    {
    public:
        RectangleRenderer();
        ~RectangleRenderer() = default;

        void submit(const niketica::component::Rectangle& rectangle) override;
        void submit(const niketica::component::RectangleBorder& rectangle) override;
        void render(const glm::mat4& projection, const glm::mat4& view) override;
        void clear() override;

    private:
        std::unique_ptr<RectangleBorderRenderer> borderRenderer;
        std::unique_ptr<RectangleBorderlessRenderer> borderlessRenderer;

        void init();
    };
}
