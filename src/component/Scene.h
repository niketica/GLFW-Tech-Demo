#pragma once

namespace niketica::component
{
    
    enum class SceneType
    {
        TEST,
        MAIN_MENU
    };

    struct SceneSwitchInstruction
    {
        SceneType nextScene;
    };

}
