#include "engine/adapter/scene/UserInterfaceDemoScene.h"

namespace niketica::scene
{
    UserInterfaceDemoScene::UserInterfaceDemoScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        ) : registry(registry),
            engineServices(engineServices)
    {
        init();
    }

    void UserInterfaceDemoScene::init()
    {
        systemContext = std::make_unique<niketica::systems::SystemContext>(registry, engineServices);
        systemContext->init();
        
        auto texture = engineServices->getRenderContext()->getTextureLoader()->acquire("textures/background/forest_001.dds");
        component::Sprite sprite;

        component::Transform transform;
        transform.position = { 0.0f, 0.0f, 0.0f };
        transform.scale = { 1.0f, 1.0f, 1.0f };
        transform.size = { 1920.0f, 1080.0f, 1.0f };

        component::Color color = { { 1.0f,1.0f,1.0f,1.0f } };

        auto entity = registry->create();
        registry->emplace<component::Sprite>(entity, sprite);
        registry->emplace<component::Transform>(entity, transform);
        registry->emplace<component::Color>(entity, color);
        registry->emplace<component::TextureHandle>(entity, texture);
        registry->emplace<component::RenderSprite>(entity);

        auto windowView = registry->view<niketica::component::Window>();
        const auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());

        niketica::component::Text text;
        text.fontSize = 48;
        text.value = "User Interface Demo";
        text.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        text.scale = 1.0f;

        niketica::component::Transform textTransform;
        textTransform.position = { 100.0f, windowComponent.height - 100.0f, 0.0f };

        auto textEntity = registry->create();
        registry->emplace<niketica::component::Text>(textEntity, text);
        registry->emplace<niketica::component::Transform>(textEntity, textTransform);

        niketica::component::UINineSlice uiNineSlice;
        uiNineSlice.texture = engineServices->getRenderContext()->getTextureLoader()->acquire("textures/ui/ui_sheet.dds");
        uiNineSlice.spriteOffset = { 814.0f, 0.0f };
        uiNineSlice.spriteSize = { 206.0f, 209.0f };
        uiNineSlice.left = 25.0f;
        uiNineSlice.right = 25.0f;
        uiNineSlice.top = 28.0f;
        uiNineSlice.bottom = 25.0f;

        niketica::component::Transform transformUI;
        transformUI.position = { 100.0f, 100.0f, 1.0f };
        transformUI.size = { 200.0f, 400.0f, 1.0f };

        auto entityUI = registry->create();
        registry->emplace<niketica::component::UINineSlice>(entityUI, uiNineSlice);
        registry->emplace<niketica::component::Transform>(entityUI, transformUI);

        niketica::component::Text textUI;
        textUI.fontSize = 20;
        textUI.value = "This is a text box.";
        textUI.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        textUI.scale = 1.0f;
        textUI.fontType = niketica::component::FontType::COURIER_PRIME_CODE;

        niketica::component::ParentTransform textUIParent = { entityUI };

        auto entityTextUI = registry->create();
        registry->emplace<niketica::component::Text>(entityTextUI, textUI);
        registry->emplace<niketica::component::Transform>(entityTextUI);
        registry->emplace<niketica::component::LocalTransform>(entityTextUI);
        registry->emplace<niketica::component::ParentTransform>(entityTextUI, textUIParent);
    }

    void UserInterfaceDemoScene::input()
    {
        systemContext->input();

        auto inputView = registry->view<niketica::component::InputComponent>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());

        if (input.actions[niketica::component::Action::W].pressed)
        {
            std::cout << "INFO::Engine::update - W key pressed." << std::endl;
            auto sound = niketica::component::SoundEffect{ "sound/sfx/menu/23910__nightflame__menuui-sounds/397604__nightflame__menu-fx-01.wav" };
            registry->emplace<niketica::component::SoundEffect>(registry->create(), sound);
        }
        if (input.actions[niketica::component::Action::A].pressed)
        {
            std::cout << "INFO::Engine::update - A key pressed." << std::endl;
            auto sound = niketica::component::SoundEffect{ "sound/sfx/menu/23910__nightflame__menuui-sounds/397599__nightflame__menu-fx-02.wav" };
            registry->emplace<niketica::component::SoundEffect>(registry->create(), sound);
        }
        if (input.actions[niketica::component::Action::S].pressed)
        {
            std::cout << "INFO::Engine::update - S key pressed." << std::endl;
            auto sound = niketica::component::SoundEffect{ "sound/sfx/menu/23910__nightflame__menuui-sounds/422514__nightflame__menu-fx-03-normal.wav" };
            registry->emplace<niketica::component::SoundEffect>(registry->create(), sound);
        }
        if (input.actions[niketica::component::Action::D].pressed)
        {
            std::cout << "INFO::Engine::update - D key pressed." << std::endl;
            auto sound = niketica::component::SoundEffect{ "sound/sfx/menu/23910__nightflame__menuui-sounds/422515__nightflame__menu-fx-03-descending.wav" };
            registry->emplace<niketica::component::SoundEffect>(registry->create(), sound);
        }
        if (input.actions[niketica::component::Action::MINUS].pressed)
        {
            auto sceneSwitch = niketica::component::SceneSwitchInstruction{ niketica::component::SceneType::TEST };
            registry->emplace<niketica::component::SceneSwitchInstruction>(registry->create(), sceneSwitch);
        }

    }

    void UserInterfaceDemoScene::update(float deltaTime)
    {
        systemContext->update(deltaTime);
    }

    void UserInterfaceDemoScene::render()
    {
        systemContext->render();
    }

    void UserInterfaceDemoScene::reset()
    {
        systemContext.release();
        init();
    }

}
