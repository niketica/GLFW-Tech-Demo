#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/builder/userinterface/UITextLabelBuilder.h"
#include "engine/builder/userinterface/UIPanelBuilder.h"
#include "engine/core/EngineServices.h"
#include "engine/core/scene/IScene.h"
#include "engine/core/systems/ISystemContext.h"
#include "engine/adapter/systems/SystemContext.h"

namespace niketica::scene
{

    struct ButtonStart {};
    struct ButtonOptions {};
    struct ButtonQuit {};

    class UserInterfaceDemoScene : public IScene
    {
    public:
        UserInterfaceDemoScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        );
        ~UserInterfaceDemoScene() = default;

        void input() override;
        void update(float deltaTime) override;
        void render() override;
        void reset() override;
    
    private:
        entt::registry* registry;
        std::unique_ptr<niketica::systems::ISystemContext> systemContext;
        niketica::engine::EngineServices* engineServices;

        void init();
        void createTestPanel();
        void createTestPanel2();
        entt::entity createTextLabel(const std::string& text);
        entt::entity createButton(const std::string& text);

    };
}