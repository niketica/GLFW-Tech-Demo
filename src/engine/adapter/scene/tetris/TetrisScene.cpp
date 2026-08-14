#include "engine/adapter/scene/tetris/TetrisScene.h"

namespace niketica::scene
{
    TetrisScene::TetrisScene
    (
        entt::registry* registry,
        niketica::engine::EngineServices* engineServices
    ) : registry(registry), engineServices(engineServices)
    {
        init();
    }

    void TetrisScene::init()
    {
        systemContext = std::make_unique<niketica::systems::SystemContext>(registry, engineServices);
        systemContext->init();

        gridEntities.clear();
        gameActive = true;
        
        float windowWidth = 1920; // TODO Should be retrieved and updated dynamically
        float totalGridWidth = blockSize * gridWidth;
        float containerX = (windowWidth * 0.5f) - (totalGridWidth * 0.5f);
        auto containerEntity = registry->create();
        niketica::component::Transform containerTransform;
        containerTransform.position.x = containerX;
        registry->emplace<niketica::component::Transform>(containerEntity, containerTransform);
        
        for (int x = 0; x < gridWidth; ++x)
        {
            for (int y = 0; y < gridHeight; ++y)
            {
                auto entity = createRectangleWithBorder
                (
                    { x * blockSize, y * blockSize, 0.0f },
                    { blockSize, blockSize },
                    { 0.1f, 0.1f, 0.1f, 1.0f },
                    { 1.0f, 1.0f, 1.0f, 1.0f },
                    1.0f,
                    1.0f
                );
                niketica::component::LocalTransform local;
                local.position.x = x * blockSize;
                local.position.y = y * blockSize;
                local.size.x = blockSize;
                local.size.y = blockSize;
                registry->emplace<niketica::component::LocalTransform>(entity, niketica::component::LocalTransform{ local });
                registry->emplace<niketica::component::ParentTransform>(entity, niketica::component::ParentTransform{ containerEntity });
                gridEntities.push_back(entity);
            }
        }
    }

    void TetrisScene::input()
    {
        systemContext->input();

        auto inputView = registry->view<niketica::component::InputComponent>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());

        if (input.actions[niketica::component::Action::ESCAPE].pressed)
        {
            auto sceneSwitch = niketica::component::SceneSwitchInstruction{ niketica::component::SceneType::MAIN_MENU };
            registry->emplace<niketica::component::SceneSwitchInstruction>(registry->create(), sceneSwitch);
        }
        if (input.actions[niketica::component::Action::W].pressed)
        {
            currentDirection = Direction::UP;
        }
        if (input.actions[niketica::component::Action::A].pressed)
        {
            currentDirection = Direction::LEFT;
        }
        if (input.actions[niketica::component::Action::S].pressed)
        {
            currentDirection = Direction::DOWN;
        }
        if (input.actions[niketica::component::Action::D].pressed)
        {
            currentDirection = Direction::RIGHT;
        }

    }

    void TetrisScene::update(float dt)
    {
        systemContext->update(dt);

        if (gameActive)
        {
            clearGrid();
        }
    }
    
    void TetrisScene::render()
    {
        // For now just assume there is always exactly 1 active camera.
        auto viewCamera = registry->view<niketica::component::Camera, niketica::component::ActiveCamera>();
        const auto &camera = viewCamera.get<niketica::component::Camera>(viewCamera.front());

        systemContext->render();
        engineServices->getRenderContext()->getRectangleRenderer()->render(camera.projection, camera.view);
    }
    
    void TetrisScene::reset()
    {        
        systemContext.release();
        init();
    }
    
    entt::entity TetrisScene::createRectangleWithBorder(const glm::vec3& position, const glm::vec2& size, const glm::vec4& fillColor, const glm::vec4& borderColor, float borderThickness, float fill)
    {
        auto transform = niketica::component::Transform{ position, {size.x, size.y, 0.0f}, {1.0f, 1.0f, 1.0f} };
        auto fillColorCmpnt = niketica::component::FillColor{ fillColor };
        auto borderColorCmpnt = niketica::component::BorderColor{ borderColor };
        auto borderThicknessCmpnt = niketica::component::BorderThickness{ borderThickness };
        
        auto entity = registry->create();
        registry->emplace<niketica::component::Rectangle>(entity);
        registry->emplace<niketica::component::Transform>(entity, transform);
        registry->emplace<niketica::component::BorderColor>(entity, borderColorCmpnt);
        registry->emplace<niketica::component::BorderThickness>(entity, borderThicknessCmpnt);

        if (fill > 0.0f)
        {
            registry->emplace<niketica::component::FillColor>(entity, fillColorCmpnt);
        }
        
        return entity;
    }

    entt::entity TetrisScene::createRectangleBorderless(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        auto transform = niketica::component::Transform{ position, {size.x, size.y, 0.0f}, {1.0f, 1.0f, 1.0f} };
        auto fillColor = niketica::component::FillColor{ color };
        
        auto entity = registry->create();
        registry->emplace<niketica::component::Rectangle>(entity);
        registry->emplace<niketica::component::Transform>(entity, transform);
        registry->emplace<niketica::component::FillColor>(entity, fillColor);

        return entity;
    }

    entt::entity TetrisScene::getEntityAtGridPosition(int x, int y)
    {
        if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
        {
            std::cerr << "ERROR::TetrisScene::getEntityAtGridPosition - Grid position out of bounds: (" << x << ", " << y << ")" << std::endl;
            return entt::null; // Return null entity if out of bounds
        }

        int index = x * gridHeight + y;
        return gridEntities[index];
    }

    void TetrisScene::clearGrid()
    {
        for (auto entity : gridEntities)
        {
            if (!registry->any_of<niketica::component::FillColor>(entity)) continue; // Skip if the entity doesn't have a FillColor component
            auto& fillColor = registry->get<niketica::component::FillColor>(entity);
            fillColor.color = { 0.1f, 0.1f, 0.1f, 1.0f };
        }
    }

}
