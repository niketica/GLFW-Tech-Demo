#pragma once

#include <memory>
#include <glad/glad.h>

#include "renderer/Shader.h"

namespace niketica::renderer
{
    class SimpleRenderer
    {
    public:
        SimpleRenderer();
        ~SimpleRenderer() = default;
        
        void render();

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
