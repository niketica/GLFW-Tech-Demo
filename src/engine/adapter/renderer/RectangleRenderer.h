#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "component/Components.h"
#include "engine/core/renderer/IRectangleRenderer.h"
#include "engine/adapter/renderer/Shader.h"

namespace niketica::renderer
{
    class RectangleRenderer : public IRectangleRenderer
    {
    public:
        RectangleRenderer();
        ~RectangleRenderer() = default;

        void submit(const niketica::component::Rectangle& rectangle) override;
        
        void render() override;

        void clear() override;

    private:
        std::unique_ptr<Shader> basicShader;

        float vertices[12] =
        {
            // positions
            0.0f, 0.0f, 0.0f, // bottom left
            1.0f, 0.0f, 0.0f, // bottom right
            1.0f, 1.0f, 0.0f, // top right
            0.0f, 1.0f, 0.0f  // top left
        };

        unsigned int indices[6] =
        {
            0,1,2,
            2,3,0
        };

        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;

        void init();

        std::vector<niketica::component::Rectangle> rectangles;

    };
}
