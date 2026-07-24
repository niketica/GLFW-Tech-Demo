#include "engine/adapter/scene/TestScene.h"

namespace niketica::scene
{
    TestScene::TestScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        ) : registry(registry),
            engineServices(engineServices)
    {
        init();
    }

    void TestScene::init()
    {
        systemContext = std::make_unique<niketica::systems::SystemContext>(registry, engineServices);
        systemContext->init();
        
        auto texture = engineServices->getRenderContext()->getTextureLoader()->acquire("textures/background/main_menu_background.dds");
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
    }

    void TestScene::input()
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
            auto sceneSwitch = niketica::component::SceneSwitchInstruction{ niketica::component::SceneType::USER_INTERFACE_DEMO };
            registry->emplace<niketica::component::SceneSwitchInstruction>(registry->create(), sceneSwitch);
        }

    }

    void TestScene::update(float deltaTime)
    {
        systemContext->update(deltaTime);

        engineServices->getRenderContext()->getSpriteInstancedRenderer()->clear();

        auto spriteView = registry->view<niketica::component::Sprite, niketica::component::Transform, niketica::component::TextureHandle>();
        for (auto entity : spriteView)
        {
            auto& sprite = spriteView.get<niketica::component::Sprite>(entity);
            auto& transform = spriteView.get<niketica::component::Transform>(entity);
            auto& textureHandle = spriteView.get<niketica::component::TextureHandle>(entity);
            engineServices->getRenderContext()->getSpriteInstancedRenderer()->submit(textureHandle.id, sprite, transform.position, transform.size, 1.0f);
        }

    }

    void TestScene::render()
    {
        systemContext->render();

        auto windowView = registry->view<niketica::component::Window>();
        auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());
        engineServices->getRenderContext()->getSpriteInstancedRenderer()->render(windowComponent.projection, windowComponent.view);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto positionTopLeft = glm::vec2(100.0f, windowComponent.height - 100.0f);
        auto colorRed = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        auto textValue = "Hello, World!";
        auto scale = 5.0f;

        engineServices->getRenderContext()->getTextRenderer()->begin(windowComponent.projection, niketica::renderer::FontType::OPEN_SANS_REGULAR);
        engineServices->getRenderContext()->getTextRenderer()->submitText(
            niketica::renderer::FontType::OPEN_SANS_REGULAR,
            textValue,
            positionTopLeft,
            scale,
            colorRed
        );

        engineServices->getRenderContext()->getTextRenderer()->flush();

    }

    void TestScene::reset()
    {
        systemContext.release();
        init();
    }

}
