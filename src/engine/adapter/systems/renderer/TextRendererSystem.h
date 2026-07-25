#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"

namespace niketica::systems
{
    struct TextBatchKey
    {
        niketica::component::FontType font;
        uint32_t size;

        bool operator==(const TextBatchKey& other) const
        {
            return font == other.font &&
                size == other.size;
        }
    };

    struct TextBatchHash
    {
        size_t operator()(const TextBatchKey& k) const
        {
            return std::hash<int>()((int)k.font) ^
                (std::hash<uint32_t>()(k.size) << 1);
        }
    };

    class TextRendererSystem : public ISystem
    {
    public:
        TextRendererSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}

        void input() override {}
        void update(float dt) override {}
        void render() override;

    private:
        const std::unordered_map<niketica::component::Action, niketica::input::ActionBinding>& mapBindings() const
        {
            return engineServices->getInputContext()->getInputMap()->getBindings();
        }

    };

}
