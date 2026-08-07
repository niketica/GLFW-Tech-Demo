#pragma once

#include <memory>
#include <glad/glad.h>

#include "engine/core/renderer/IRectangleRenderer.h"
#include "engine/adapter/renderer/Shader.h"

namespace niketica::renderer
{
    class RectangleRenderer : public IRectangleRenderer
    {
    public:
        RectangleRenderer();
        ~RectangleRenderer() = default;
        
        void render() override;

    private:
        std::unique_ptr<Shader> basicShader;

        float vertices[9] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };
        unsigned int VBO;
        unsigned int VAO;

        void init();

    };
}
