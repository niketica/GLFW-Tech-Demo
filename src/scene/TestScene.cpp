#include "scene/TestScene.h"

namespace niketica::scene
{
    TestScene::TestScene(
            entt::registry* registry,
            niketica::systems::SystemRepository* systemRepository,
            niketica::renderer::RendererRepository* rendererRepository
        ) : registry(registry),
            systemRepository(systemRepository),
            rendererRepository(rendererRepository)
    {
        init();
    }

    void TestScene::init()
    {
        auto texture = rendererRepository->getTextureLoader()->acquire("textures/background/main_menu_background.dds");
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
        auto inputView = registry->view<niketica::component::InputComponent>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());

        if (input.actions[niketica::component::Action::W].pressed)
        {
            std::cout << "INFO::Engine::update - W key pressed." << std::endl;
        }

    }

    void TestScene::update(float deltaTime)
    {
        rendererRepository->getSpriteInstancedRenderer()->clear();

        auto spriteView = registry->view<niketica::component::Sprite, niketica::component::Transform, niketica::component::TextureHandle>();
        for (auto entity : spriteView)
        {
            auto& sprite = spriteView.get<niketica::component::Sprite>(entity);
            auto& transform = spriteView.get<niketica::component::Transform>(entity);
            auto& textureHandle = spriteView.get<niketica::component::TextureHandle>(entity);
            rendererRepository->getSpriteInstancedRenderer()->submit(textureHandle.id, sprite, transform.position, transform.size, 1.0f);
        }
    }

    void TestScene::render()
    {
        auto windowView = registry->view<niketica::component::Window>();
        auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());
        rendererRepository->getSpriteInstancedRenderer()->render(windowComponent.projection, windowComponent.view);
    }
}
