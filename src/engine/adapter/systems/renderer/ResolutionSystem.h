#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"

namespace niketica::systems
{

    class ResolutionSystem : public ISystem
    {
    public:
        ResolutionSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}

        void input() override {}
        void update(float dt) override;
        void render() override {};
    
    private:
        bool isResolutionDirty() const;

        void updateUIText(const niketica::component::RenderSettings& renderSettings);
        void updateUINineSlice(const niketica::component::RenderSettings& renderSettings);

        glm::vec2 calculatePosition(const glm::vec2& resolution, const glm::vec2& scale, const niketica::component::UIAnchor& anchor) const;
        glm::vec2 calculateSize(const glm::vec2& scale, const niketica::component::UISize& size) const;

    };

}
