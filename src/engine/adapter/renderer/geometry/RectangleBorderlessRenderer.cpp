#include "engine/adapter/renderer/geometry/RectangleBorderlessRenderer.h"

namespace niketica::renderer
{
    RectangleBorderlessRenderer::RectangleBorderlessRenderer()
    {
        init();
    }
    
    void RectangleBorderlessRenderer::init()
    {
        rectShader = std::make_unique<Shader>
        (
            "shaders/rect_shader.vert",
            "shaders/rect_shader.frag"
        );

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &quadVBO);
        glGenBuffers(1, &instanceVBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

        glBufferData
        (
            GL_ARRAY_BUFFER,
            sizeof(quad),
            quad,
            GL_STATIC_DRAW
        );

        glVertexAttribPointer
        (
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            2 * sizeof(float),
            nullptr
        );

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

        glBufferData
        (
            GL_ARRAY_BUFFER,
            0,
            nullptr,
            GL_DYNAMIC_DRAW
        );

        // Position
        glVertexAttribPointer
        (
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(RectangleBorderlessData),
            (void*)offsetof(RectangleBorderlessData, position)
        );

        glEnableVertexAttribArray(1);

        // Size
        glVertexAttribPointer
        (
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(RectangleBorderlessData),
            (void*)offsetof(RectangleBorderlessData, size)
        );

        glEnableVertexAttribArray(2);

        // Color
        glVertexAttribPointer
        (
            3,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(RectangleBorderlessData),
            (void*)offsetof(RectangleBorderlessData, color)
        );

        glEnableVertexAttribArray(3);

        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);

        // EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData
        (
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(indices),
            indices,
            GL_STATIC_DRAW
        );

        glBindVertexArray(0);
    }

    void RectangleBorderlessRenderer::submit(const RectangleBorderlessData& rectangle)
    {
        rectangles.push_back(rectangle);
    }

    void RectangleBorderlessRenderer::render
    (
        const glm::mat4& projection,
        const glm::mat4& view
    )
    {
        if (rectangles.empty()) return;

        rectShader->use();

        rectShader->setMat4("projection", projection);
        rectShader->setMat4("view", view);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

        glBufferData
        (
            GL_ARRAY_BUFFER,
            rectangles.size() * sizeof(RectangleBorderlessData),
            rectangles.data(),
            GL_DYNAMIC_DRAW
        );

        glDrawElementsInstanced
        (
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            nullptr,
            static_cast<GLsizei>(rectangles.size())
        );

        glBindVertexArray(0);
    }

    void RectangleBorderlessRenderer::clear()
    {
        rectangles.clear();
    }
    
}
