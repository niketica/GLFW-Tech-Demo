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
    class RectangleBorderRenderer
    {
    public:
        RectangleBorderRenderer();
        ~RectangleBorderRenderer() = default;

        void submit(const RectangleBorderData& rectangle);        
        void render(const glm::mat4& projection, const glm::mat4& view);
        void clear();

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

        std::vector<RectangleBorderData> rectangles;

    };
}
