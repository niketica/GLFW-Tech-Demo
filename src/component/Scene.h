#pragma once

namespace niketica::component
{
    
    enum class SceneType
    {
        TEST,
        USER_INTERFACE_DEMO
    };

    struct SceneSwitchInstruction
    {
        SceneType nextScene;
    };

}
