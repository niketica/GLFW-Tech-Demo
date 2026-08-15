#include "scene/tetris/TetrisScene.h"

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

        auto gridSystem = std::make_unique<niketica::tetris::GridSystem>(registry, engineServices);
        systemContext->addSystem(std::move(gridSystem));
        auto rotationSystem = std::make_unique<niketica::tetris::RotationSystem>(registry, engineServices);
        systemContext->addSystem(std::move(rotationSystem));
        auto tetrominoCreationSystem = std::make_unique<niketica::tetris::TetrominoCreationSystem>(registry, engineServices);
        systemContext->addSystem(std::move(tetrominoCreationSystem));
        auto blockPositionSystem = std::make_unique<niketica::tetris::BlockPositionSystem>(registry, engineServices);
        systemContext->addSystem(std::move(blockPositionSystem));
        auto verticalMovementSystem = std::make_unique<niketica::tetris::VerticalMovementSystem>(registry, engineServices);
        systemContext->addSystem(std::move(verticalMovementSystem));
        auto horizontalMovementSystem = std::make_unique<niketica::tetris::HorizontalMovementSystem>(registry, engineServices);
        systemContext->addSystem(std::move(horizontalMovementSystem));

        niketica::tetris::GameState gameState;
        gameState.gameActive = true;
        registry->emplace<niketica::tetris::GameState>(registry->create(), gameState);
        registry->emplace<niketica::tetris::CreateGrid>(registry->create());
        registry->emplace<niketica::tetris::CreateRandomTetromino>(registry->create());
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
        
        auto viewTetromino = registry->view<niketica::tetris::Tetromino>();
        auto& tetromino = viewTetromino.get<niketica::tetris::Tetromino>(viewTetromino.front());
        tetromino.direction = niketica::tetris::Direction::UNDEFINED;
        if (input.actions[niketica::component::Action::A].pressed)
        {
            tetromino.direction = niketica::tetris::Direction::LEFT;
        }
        if (input.actions[niketica::component::Action::D].pressed)
        {
            tetromino.direction = niketica::tetris::Direction::RIGHT;
        }
        if (input.actions[niketica::component::Action::S].pressed)
        {
            tetromino.direction = niketica::tetris::Direction::DOWN;
        }
        if (input.actions[niketica::component::Action::E].pressed || input.actions[niketica::component::Action::R].pressed)
        {
            registry->emplace<niketica::tetris::RotateTetromino>(registry->create());
        }

    }

    void TetrisScene::update(float dt)
    {
        systemContext->update(dt);

        auto& gameState = niketica::tetris::getGameState(registry);

        if (gameState.verticalHitDetected)
        {
            gameState.verticalHitDetected = false;
            registry->emplace<niketica::tetris::MoveTetrominoBlocksToGrid>(registry->create());
            registry->emplace<niketica::tetris::DestroyTetromino>(registry->create());
            registry->emplace<niketica::tetris::CreateRandomTetromino>(registry->create());
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

}
