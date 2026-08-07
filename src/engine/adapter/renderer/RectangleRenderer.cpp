#include "engine/adapter/renderer/RectangleRenderer.h"

namespace niketica::renderer
{
    RectangleRenderer::RectangleRenderer()
    {
        init();
    }
    
    void RectangleRenderer::init()
    {
        basicShader = std::make_unique<Shader>("shaders/rect_shader.vert", "shaders/rect_shader.frag");

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData
        (
            GL_ARRAY_BUFFER,
            sizeof(vertices),
            vertices,
            GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData
        (
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(indices),
            indices,
            GL_STATIC_DRAW
        );

        glVertexAttribPointer
        (
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            (void*)0
        );

        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    void RectangleRenderer::submit(const niketica::component::Rectangle& rectangle)
    {
        rectangles.push_back(rectangle);
    }

    void RectangleRenderer::render()
    {
        basicShader->use();

        glBindVertexArray(VAO);

        for (const auto& rect : rectangles)
        {
            glm::mat4 transform(1.0f);

            transform = glm::translate
            (
                transform,
                glm::vec3(rect.position, 0.0f)
            );

            transform = glm::scale
            (
                transform,
                glm::vec3(rect.size, 1.0f)
            );

            basicShader->setMat4("uTransform", transform);
            basicShader->setVec4("uColor", rect.color);

            glDrawElements
            (
                GL_TRIANGLES,
                6,
                GL_UNSIGNED_INT,
                nullptr
            );
        }
    }

    void RectangleRenderer::clear()
    {
        rectangles.clear();
    }
    
}
