#include "engine/adapter/renderer/RectangleBorderRenderer.h"

namespace niketica::renderer
{
    RectangleBorderRenderer::RectangleBorderRenderer()
    {
        init();
    }
    
    void RectangleBorderRenderer::init()
    {
        rectShader = std::make_unique<Shader>
        (
            "shaders/rect_border_shader.vert",
            "shaders/rect_border_shader.frag"
        );

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &quadVBO);
        glGenBuffers(1, &instanceVBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // --------------------------------------------------
        // Quad VBO
        // --------------------------------------------------

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(quad),
            quad,
            GL_STATIC_DRAW
        );

        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            2 * sizeof(float),
            nullptr
        );

        glEnableVertexAttribArray(0);

        // --------------------------------------------------
        // Instance VBO
        // --------------------------------------------------

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            0,
            nullptr,
            GL_DYNAMIC_DRAW
        );

        const auto stride = sizeof(niketica::component::RectangleBorder);

        // Position
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            stride,
            (void*)offsetof(
                niketica::component::RectangleBorder,
                position)
        );

        glEnableVertexAttribArray(1);

        // Size
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            stride,
            (void*)offsetof(
                niketica::component::RectangleBorder,
                size)
        );

        glEnableVertexAttribArray(2);

        // Fill color
        glVertexAttribPointer(
            3,
            4,
            GL_FLOAT,
            GL_FALSE,
            stride,
            (void*)offsetof(
                niketica::component::RectangleBorder,
                fillColor)
        );

        glEnableVertexAttribArray(3);

        // Border color
        glVertexAttribPointer(
            4,
            4,
            GL_FLOAT,
            GL_FALSE,
            stride,
            (void*)offsetof(
                niketica::component::RectangleBorder,
                borderColor)
        );

        glEnableVertexAttribArray(4);

        // Border thickness
        glVertexAttribPointer(
            5,
            1,
            GL_FLOAT,
            GL_FALSE,
            stride,
            (void*)offsetof(
                niketica::component::RectangleBorder,
                borderThickness)
        );

        glEnableVertexAttribArray(5);

        // Fill enabled
        glVertexAttribPointer(
            6,
            1,
            GL_FLOAT,
            GL_FALSE,
            stride,
            (void*)offsetof(
                niketica::component::RectangleBorder,
                fill)
        );

        glEnableVertexAttribArray(6);

        // All instance attributes advance once per instance.
        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        // --------------------------------------------------
        // EBO
        // --------------------------------------------------

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(indices),
            indices,
            GL_STATIC_DRAW
        );

        glBindVertexArray(0);
    }

    void RectangleBorderRenderer::submit(const niketica::component::RectangleBorder& rectangle)
    {
        rectangles.push_back(rectangle);
    }

    void RectangleBorderRenderer::render(
        const glm::mat4& projection,
        const glm::mat4& view)
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
            rectangles.size() * sizeof(niketica::component::RectangleBorder),
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

    void RectangleBorderRenderer::clear()
    {
        rectangles.clear();
    }
    
}
