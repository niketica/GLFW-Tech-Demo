#include "systems/sound/SoundSystem.h"

namespace niketica::systems
{
    void SoundSystem::update(float deltaTime)
    {
        auto viewSFX = registry->view<component::SoundEffect>();
        for (auto entity : viewSFX)
        {
            const auto& sound = viewSFX.get<component::SoundEffect>(entity);
            engineServices->getSoundContext()->playSoundAssetFile(sound.path.c_str());
            registry->destroy(entity);
        }

        auto viewMusic = registry->view<component::Music>();
        for (auto entity : viewMusic)
        {
            const auto& music = viewMusic.get<component::Music>(entity);
            engineServices->getSoundContext()->playSoundAssetFile(music.path.c_str(), true);
            registry->destroy(entity);
        }
    }
}
