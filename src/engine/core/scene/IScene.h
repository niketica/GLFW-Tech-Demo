#pragma once

namespace niketica::scene
{

    class IScene
    {
    public:
        virtual ~IScene() = default;

        virtual void input() = 0;
        virtual void update(float dt) = 0;
        virtual void render() = 0;

    };

}
