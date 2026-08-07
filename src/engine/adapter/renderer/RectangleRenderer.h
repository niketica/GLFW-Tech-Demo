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
        
        void render(const glm::mat4& projection, const glm::mat4& view) override;

        void clear() override;

    private:
        std::unique_ptr<Shader> rectShader;

        unsigned int indices[6] =
        {
            0,1,2,
            2,3,0
        };

        float quad[8] =
        {
            0.f,0.f,
            1.f,0.f,
            1.f,1.f,
            0.f,1.f
        };

        unsigned int  quadVBO;
        unsigned int  instanceVBO;
        unsigned int  EBO;
        unsigned int  VAO;
        

        void init();

        std::vector<niketica::component::Rectangle> rectangles;

    };
}
