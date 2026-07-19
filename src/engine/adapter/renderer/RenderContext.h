#pragma once

#include "engine/core/renderer/IRenderContext.h"
#include "engine/adapter/renderer/TextureLoader.h"
#include "engine/adapter/renderer/SpriteInstancedRenderer.h"
#include "engine/adapter/renderer/text/TextRenderer.h"

namespace niketica::renderer
{

    class RenderContext : public IRenderContext
    {
    public:
        RenderContext() = default;
        ~RenderContext() = default;

        void init() override;

    };

}
