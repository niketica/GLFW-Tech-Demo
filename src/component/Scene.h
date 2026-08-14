#pragma once

namespace niketica::component
{
    
    enum class SceneType
    {
        TEST,
        MAIN_MENU,
        SNAKE,
        TETRIS
    };

    struct SceneSwitchInstruction
    {
        SceneType nextScene;
    };

}
