#include "engine/adapter/systems/userinterface/UILayoutSystem.h"

namespace niketica::systems
{

    void UILayoutSystem::update(float dt)
    {
        auto viewPanel = registry->view<niketica::component::UIChildren>();
        for (auto panel : viewPanel)
        {
            const auto& children = registry->get<niketica::component::UIChildren>(panel);
            const auto& transform = registry->get<niketica::component::LocalTransform>(panel);
            const auto& size = transform.size;

            const auto& padding = registry->get<niketica::component::UIContentPadding>(panel);
            const auto& spacing = registry->get<niketica::component::UISpacing>(panel);

            float cursorY = size.y - padding.top;

            for (auto child : children.children)
            {
                auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
                const auto& alignment = registry->get<niketica::component::UIAnchor>(child);

                cursorY -= childLocal.size.y;
                childLocal.position.y = cursorY;
                cursorY -= spacing.spacing;

                childLocal.position.z = transform.position.z + 0.01f;
                
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
