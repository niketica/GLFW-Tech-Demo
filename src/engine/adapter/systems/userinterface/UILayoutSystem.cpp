#include "engine/adapter/systems/userinterface/UILayoutSystem.h"

namespace niketica::systems
{

    void UILayoutSystem::update(float dt)
    {
        auto viewPanel = registry->view<niketica::component::UIPanel>();
        for (auto panel : viewPanel)
        {
            const auto& transform = registry->get<niketica::component::Transform>(panel);
            const auto& size = transform.size;

            const auto& padding = registry->get<niketica::component::UIContentPadding>(panel);
            const auto& spacing = registry->get<niketica::component::UISpacing>(panel);
            const auto& alignment = registry->get<niketica::component::UIAlignment>(panel);
            const auto& children = registry->get<niketica::component::UIChildren>(panel);

            for (auto child : children.children)
            {
                auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
                
                switch (alignment.horizontal)
                {
                case niketica::component::AlignmentHorizontal::CENTER:
                {
                    float centerPanel = size.x * 0.5f;
                    float halfSizeChild = childLocal.size.x * 0.5f;
                    childLocal.position.x = centerPanel - halfSizeChild;
                }
                break;
                case niketica::component::AlignmentHorizontal::LEFT:
                {
                    childLocal.position.x = padding.left;
                }
                break;
                case niketica::component::AlignmentHorizontal::RIGHT:
                {
                    childLocal.position.x = size.x - padding.right - childLocal.size.x;                    
                }
                break;
                }
                
            }
        }
    }
    
}
