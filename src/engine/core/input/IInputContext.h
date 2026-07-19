#pragma once

#include <memory>

#include "engine/core/input/InputState.h"
#include "engine/core/input/InputMap.h"

namespace niketica::input
{

    class IInputContext
    {
    public:
        virtual ~IInputContext() = default;

        virtual void clearState() = 0;

        InputState* getInputState()
        {
            return inputState.get();
        }

        InputMap* getInputMap()
        {
            return inputMap.get();
        }

    protected:
        std::unique_ptr<InputState> inputState;
        std::unique_ptr<InputMap> inputMap;
    };

}
