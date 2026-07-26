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

        niketica::builder::UIPanelBuilder panelBuilder = { registry, engineServices };
        panelBuilder
            .withPosition({600,100})
            .withSize({300,400})
            .withPadding(20.0f)
            .withLayout(niketica::component::UILayoutType::Vertical)
            .addTextLabel("First line")
            .addTextLabel("Second line")
            .addButton("Start", { 300.0f, 100.0f})
            .addButton("Options", { 300.0f, 100.0f})
            .addButton("Quit", { 300.0f, 100.0f})
            .build();
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
