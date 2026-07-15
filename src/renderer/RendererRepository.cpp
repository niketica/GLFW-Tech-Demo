#include "renderer/RendererRepository.h"

namespace niketica::renderer
{
    
    RendererRepository::RendererRepository()
    {
        simpleRenderer = std::make_unique<SimpleRenderer>();
    }

}