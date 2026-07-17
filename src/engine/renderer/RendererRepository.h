#pragma once

#include <memory>

#include "engine/renderer/SimpleRenderer.h"

namespace niketica::renderer
{
    class RendererRepository
    {
    public:
        RendererRepository();
        ~RendererRepository() = default;

        SimpleRenderer* getSimpleRenderer() const { return simpleRenderer.get(); }

    private:
        std::unique_ptr<SimpleRenderer> simpleRenderer;
        
    };
}