#pragma once

#include <entt/entt.hpp>

namespace niketica::scene
{

    class ISceneContext
    {
    public:
        virtual ~ISceneContext() = default;

        virtual void input() = 0;
        virtual void update(float dt) = 0;
        virtual void render() = 0;

        void setRegistry(entt::registry* registry)
        {
            this->registry = registry;
        }

        virtual void initScenes() = 0;

    protected:
        entt::registry* registry;

    };

}
