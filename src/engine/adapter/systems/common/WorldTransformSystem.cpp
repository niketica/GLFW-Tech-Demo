#include "engine/adapter/systems/common/WorldTransformSystem.h"

namespace niketica::systems
{

    void WorldTransformSystem::update(float dt)
    {
        auto viewLocalTransfom = registry->view<niketica::component::LocalTransform, niketica::component::Transform>();
        for (auto entity : viewLocalTransfom)
        {
            const auto& local = registry->get<niketica::component::LocalTransform>(entity);
            auto& world = registry->get<niketica::component::Transform>(entity);

            world.position = local.position;
            world.size = local.size;
            world.rotation = local.rotation;
            world.scale = local.scale;

            if (registry->any_of<niketica::component::ParentTransform>(entity))
            {
                const auto& parent = registry->get<niketica::component::ParentTransform>(entity); 
                const auto& parentWorld = registry->get<niketica::component::Transform>(parent.parent); 

                world.position = parentWorld.position + local.position;
            }
        }
    }

}
