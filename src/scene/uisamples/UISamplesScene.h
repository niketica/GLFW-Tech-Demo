#pragma once

#include <vector>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"
#include "engine/core/scene/IScene.h"
#include "engine/core/systems/ISystemContext.h"
#include "systems/SystemContext.h"
#include "engine/builder/userinterface/UITextLabelBuilder.h"
#include "engine/builder/userinterface/UIRectangleBuilder.h"

namespace niketica::scene
{

    class UISamplesScene : public IScene
    {
    public:
        UISamplesScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        );
        ~UISamplesScene() = default;

        void input() override;
        void update(float dt) override;
        void render() override;
        void reset() override;
    
    private:        
        entt::registry* registry;
        std::unique_ptr<niketica::systems::ISystemContext> systemContext;
        niketica::engine::EngineServices* engineServices;

        void init();

    };
}
