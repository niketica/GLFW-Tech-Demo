#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/util/EngineUtils.h"
#include "engine/builder/Builders.h"

namespace niketica::factory::ui
{

    class BasePanelFactory
    {
    public:
        BasePanelFactory(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : registry(registry), engineServices(engineServices) {}
        ~BasePanelFactory() = default;

    protected:
        entt::registry* registry;
        niketica::engine::EngineServices* engineServices;

        entt::entity createContainer(const glm::vec2& size, const float spacing, const niketica::component::UILayoutType layout);
        void makeContainer(entt::entity entity, const glm::vec2& size, const float spacing, const niketica::component::UILayoutType layout);
        void addChildToContainer(entt::entity container, entt::entity child);

        entt::entity createContainerRect(const char* fillColor, const char* borderColor, const glm::vec2 size, float spacing = 0.0f);
        entt::entity createButton(const char* text, const glm::vec2& size, const float padding);
        entt::entity createTextLabel(const char* text, float fontSize);
        void setPadding(entt::entity entity, const float value);

    };
}