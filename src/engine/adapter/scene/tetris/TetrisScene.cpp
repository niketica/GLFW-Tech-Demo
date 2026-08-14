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

        createTetrominoL();
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
        
        auto viewTetromino = registry->view<Tetromino>();
        auto& tetromino = viewTetromino.get<Tetromino>(viewTetromino.front());
        tetromino.direction = Direction::UNDEFINED;
        if (input.actions[niketica::component::Action::A].pressed)
        {
            tetromino.direction = Direction::LEFT;
        }
        if (input.actions[niketica::component::Action::D].pressed)
        {
            tetromino.direction = Direction::RIGHT;
        }
        if (input.actions[niketica::component::Action::S].pressed)
        {
            tetromino.direction = Direction::DOWN;
        }
        if (input.actions[niketica::component::Action::E].pressed)
        {
            rotateTetromino();
        }

    }

    void TetrisScene::update(float dt)
    {
        systemContext->update(dt);

        if (gameActive)
        {
            clearGrid();
        }

        moveTetromino();
        colorTetrominoOnGrid();
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

    void TetrisScene::setBlockPositions()
    {
        auto viewTetromino = registry->view<Tetromino, BlockPositions>();
        auto entity = viewTetromino.front();

       auto& blockPositions = viewTetromino.get<BlockPositions>(entity).blockPositions;

        if (registry->any_of<Matrices2X2>(entity))
        {
            auto& matrix = getMatrix2x2(entity);
            blockPositions.clear();
            for (int y=0; y<2; y++)
            {
                for (int x=0; x<2; x++)
                {
                    if (matrix[y][x] == 'X')
                    {
                        blockPositions.emplace_back(glm::ivec2{x,3-y});
                    }
                }
            }
        }
        else if (registry->any_of<Matrices3X3>(entity))
        {
            auto& matrix = getMatrix3x3(entity);
            blockPositions.clear();
            for (int y=0; y<3; y++)
            {
                for (int x=0; x<3; x++)
                {
                    if (matrix[y][x] == 'X')
                    {
                        blockPositions.emplace_back(glm::ivec2{x,3-y});
                    }
                }
            }
        }
        else if (registry->any_of<Matrices4X4>(entity))
        {
            auto& matrix = getMatrix4x4(entity);
            blockPositions.clear();
            for (int y=0; y<4; y++)
            {
                for (int x=0; x<4; x++)
                {
                    if (matrix[y][x] == 'X')
                    {
                        blockPositions.emplace_back(glm::ivec2{x,3-y});
                    }
                }
            }
        }
    }

    TetrisScene::MATRIX_2X2& TetrisScene::getMatrix2x2(entt::entity entity)
    {
        auto matrices = registry->get<Matrices2X2>(entity);
        return matrices.matrix;
    }

    TetrisScene::MATRIX_3X3& TetrisScene::getMatrix3x3(entt::entity entity)
    {
        auto viewTetromino = registry->view<Tetromino, BlockPositions>();
        const auto& tetromino = viewTetromino.get<Tetromino>(entity);
        auto matrices = registry->get<Matrices3X3>(entity);

        switch (tetromino.rotation)
        {
        case Rotation::_1:
            return matrices.matrix1;
        case Rotation::_2:
            return matrices.matrix2;
        case Rotation::_3:
            return matrices.matrix3;
        case Rotation::_4:
            return matrices.matrix4;
        }
        throw std::invalid_argument("TetrisScene::getMatrix3x3 - No valid rotation for tetromino.");
    }

    TetrisScene::MATRIX_4X4& TetrisScene::getMatrix4x4(entt::entity entity)
    {
        auto viewTetromino = registry->view<Tetromino, BlockPositions>();
        const auto& tetromino = viewTetromino.get<Tetromino>(entity);
        auto matrices = registry->get<Matrices4X4>(entity);

        switch (tetromino.rotation)
        {
        case Rotation::_1:
            return matrices.matrix1;
        case Rotation::_2:
            return matrices.matrix2;
        case Rotation::_3:
            return matrices.matrix3;
        case Rotation::_4:
            return matrices.matrix4;
        }
        throw std::invalid_argument("TetrisScene::getMatrix4x4 - No valid rotation for tetromino.");
    }

    void TetrisScene::rotateTetromino()
    {
        auto viewTetromino = registry->view<Tetromino, GridPosition>();
        auto& tetromino = viewTetromino.get<Tetromino>(viewTetromino.front());
        auto& gridPosition = viewTetromino.get<GridPosition>(viewTetromino.front()).position;

        switch (tetromino.rotation)
        {
        case Rotation::_1:
            tetromino.rotation = Rotation::_2;
            break;
        case Rotation::_2:
            tetromino.rotation = Rotation::_3;
            break;
        case Rotation::_3:
            tetromino.rotation = Rotation::_4;
            break;
        case Rotation::_4:
            tetromino.rotation = Rotation::_1;
            break;
        }

        setBlockPositions();

        while (shouldPushLeft())
        {
            gridPosition.x--;
        }

        while (shouldPushRight())
        {
            gridPosition.x++;
        }
    }
    
    void TetrisScene::createTetrominoI()
    {
        auto entity = createTetrominoBase();

        auto matrices = Matrices4X4
        {
            niketica::tetris::TETROMINO_I_1,            
            niketica::tetris::TETROMINO_I_2,            
            niketica::tetris::TETROMINO_I_3,
            niketica::tetris::TETROMINO_I_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_I);
        registry->emplace<Matrices4X4>(entity, matrices);

        setBlockPositions();
    }

    void TetrisScene::createTetrominoJ()
    {
        auto entity = createTetrominoBase();

        auto matrices = Matrices3X3
        {
            niketica::tetris::TETROMINO_J_1,            
            niketica::tetris::TETROMINO_J_2,            
            niketica::tetris::TETROMINO_J_3,
            niketica::tetris::TETROMINO_J_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_J);
        registry->emplace<Matrices3X3>(entity, matrices);

        setBlockPositions();
    }
    
    void TetrisScene::createTetrominoL()
    {
        auto entity = createTetrominoBase();

        auto matrices = Matrices3X3
        {
            niketica::tetris::TETROMINO_L_1,            
            niketica::tetris::TETROMINO_L_2,            
            niketica::tetris::TETROMINO_L_3,
            niketica::tetris::TETROMINO_L_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_L);
        registry->emplace<Matrices3X3>(entity, matrices);

        setBlockPositions();        
    }
    
    void TetrisScene::createTetrominoO()
    {
        auto entity = createTetrominoBase();

        auto matrices = Matrices2X2
        {
            niketica::tetris::TETROMINO_O_1
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_O);
        registry->emplace<Matrices2X2>(entity, matrices);

        setBlockPositions();        
    }
    
    void TetrisScene::createTetrominoS()
    {
        auto entity = createTetrominoBase();

        auto matrices = Matrices3X3
        {
            niketica::tetris::TETROMINO_S_1,
            niketica::tetris::TETROMINO_S_2,
            niketica::tetris::TETROMINO_S_3,
            niketica::tetris::TETROMINO_S_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_S);
        registry->emplace<Matrices3X3>(entity, matrices);

        setBlockPositions();
    }
    
    void TetrisScene::createTetrominoT()
    {
        auto entity = createTetrominoBase();

        auto matrices = Matrices3X3
        {
            niketica::tetris::TETROMINO_T_1,
            niketica::tetris::TETROMINO_T_2,
            niketica::tetris::TETROMINO_T_3,
            niketica::tetris::TETROMINO_T_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_T);
        registry->emplace<Matrices3X3>(entity, matrices);

        setBlockPositions();
    }

    void TetrisScene::createTetrominoZ()
    {
        auto entity = createTetrominoBase();

        auto matrices = Matrices3X3
        {
            niketica::tetris::TETROMINO_Z_1,
            niketica::tetris::TETROMINO_Z_2,
            niketica::tetris::TETROMINO_Z_3,
            niketica::tetris::TETROMINO_Z_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_Z);
        registry->emplace<Matrices3X3>(entity, matrices);

        setBlockPositions();
    }
    
    entt::entity TetrisScene::createTetrominoBase()
    {
        auto entity = registry->create();
        registry->emplace<Tetromino>(entity);
        registry->emplace<BlockPositions>(entity);
        registry->emplace<GridPosition>(entity);
        return entity;
    }

    void TetrisScene::colorTetrominoOnGrid()
    {
        auto viewTetromino = registry->view<Tetromino, BlockPositions, GridPosition>();
        auto entity = viewTetromino.front();
        const auto& blockPositions = viewTetromino.get<BlockPositions>(entity).blockPositions;
        const auto& gridPosition = viewTetromino.get<GridPosition>(entity).position;
        for (const auto& blockPosition : blockPositions)
        {
            auto gridEntity = getEntityAtGridPosition(gridPosition.x + blockPosition.x, gridPosition.y + blockPosition.y);
            if (!registry->any_of<niketica::component::FillColor>(gridEntity)) continue; // Skip if the entity doesn't have a FillColor component
            auto& fillColor = registry->get<niketica::component::FillColor>(gridEntity);
            fillColor.color = { 0.8f, 0.1f, 0.1f, 1.0f };
        }
    }
    
    void TetrisScene::moveTetromino()
    {
        auto viewTetromino = registry->view<Tetromino, GridPosition>();
        const auto& tetromino = viewTetromino.get<Tetromino>(viewTetromino.front());

        switch (tetromino.direction)
        {
        case Direction::LEFT:
        {
            if (canMoveLeft())
            {
                auto& position = viewTetromino.get<GridPosition>(viewTetromino.front()).position;
                position.x--;
            }
        }
        break;
        case Direction::RIGHT:
        {
            if (canMoveRight())
            {
                auto& position = viewTetromino.get<GridPosition>(viewTetromino.front()).position;
                position.x++;
            }
        }
        break;
        }
    }

    bool TetrisScene::canMoveLeft()
    {
        auto viewTetromino = registry->view<Tetromino, GridPosition, BlockPositions>();
        const auto& gridPosition = viewTetromino.get<GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<BlockPositions>(viewTetromino.front()).blockPositions;

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x - 1;
            if (x < 0)
            {
                return false;
            }
        }

        return true;
    }

    bool TetrisScene::canMoveRight()
    {
        auto viewTetromino = registry->view<Tetromino, GridPosition, BlockPositions>();
        const auto& gridPosition = viewTetromino.get<GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<BlockPositions>(viewTetromino.front()).blockPositions;

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x + 1;
            if (x >= gridWidth)
            {
                return false;
            }
        }

        return true;
    }

    bool TetrisScene::shouldPushLeft()
    {
        auto viewTetromino = registry->view<Tetromino, GridPosition, BlockPositions>();
        const auto& gridPosition = viewTetromino.get<GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<BlockPositions>(viewTetromino.front()).blockPositions;

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x;
            if (x >= gridWidth)
            {
                return true;
            }
        }

        return false;
    }

    bool TetrisScene::shouldPushRight()
    {
        auto viewTetromino = registry->view<Tetromino, GridPosition, BlockPositions>();
        const auto& gridPosition = viewTetromino.get<GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<BlockPositions>(viewTetromino.front()).blockPositions;

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x;
            if (x < 0)
            {
                return true;
            }
        }

        return false;
    }

}
