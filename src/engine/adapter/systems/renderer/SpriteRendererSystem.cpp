#include "engine/adapter/systems/renderer/SpriteRendererSystem.h"

namespace niketica::systems
{
    void SpriteRendererSystem::update(float dt)
    {
        engineServices->getRenderContext()->getSpriteInstancedRenderer()->clear();
        auto spriteView = registry->view<niketica::component::Sprite, niketica::component::Transform, niketica::component::TextureHandle>();
        for (auto entity : spriteView)
        {
            const auto& sprite = spriteView.get<niketica::component::Sprite>(entity);
            const auto& transform = spriteView.get<niketica::component::Transform>(entity);
            const auto& textureHandle = spriteView.get<niketica::component::TextureHandle>(entity);
            engineServices->getRenderContext()->getSpriteInstancedRenderer()->submit(textureHandle.id, sprite, transform.position, transform.size, 1.0f);
        }
        
        engineServices->getRenderContext()->getNineSliceInstancedRenderer()->clear();
        auto nineSliceView = registry->view<niketica::component::NineSlice, niketica::component::Transform>();
        for (auto entity : nineSliceView)
        {
            const auto& uiNineSlice = registry->get<niketica::component::NineSlice>(entity);
            const auto& transform = registry->get<niketica::component::Transform>(entity);
            const auto& ui_sheet = engineServices->getRenderContext()->getTextureLoader()->acquire(uiNineSlice.texture);

            niketica::component::NineSliceTexture nineSliceTex =
            {
                ui_sheet,
                2048.0f,
                2048.0f
            };
            engineServices->getRenderContext()->getNineSliceInstancedRenderer()->submit(transform, uiNineSlice, nineSliceTex, 1.0f);
        }
    }

    void SpriteRendererSystem::render()
    {
        auto windowView = registry->view<niketica::component::Window>();
        const auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());

        engineServices->getRenderContext()->getSpriteInstancedRenderer()->render(windowComponent.projection, windowComponent.view);
        engineServices->getRenderContext()->getNineSliceInstancedRenderer()->render(windowComponent.projection, windowComponent.view);
    }

}
