#pragma once

#include <unordered_map>

namespace niketica::component
{
    enum class CharacterType
    {
        HUMAN_MALE,
        HUMAN_FEMALE
    };

    enum class AnimationType
    {
        IDLE,
        RUN,
        WALK,
        ATTACK,
        ATTACK_BOW,
        IMPACT,
        DEATH
    };

    enum class SpriteType
    {
        UNDEFINED,
        SKIN,
        ARMOR,
        HAIR,
        FACIAL_HAIR,
        PROP_R,
        PROP_L,
        MAP
    };

    enum class Direction
    {
        W, NW, N, NE, E, SE, S, SW
    };

    struct AnimationLayerSet
    {
        uint32_t firstLayer;
        uint32_t layerCount;
    };

    struct AnimationClip
    {
        AnimationType type;
        uint32_t framesPerDirection;
        float frameDuration; // seconds per frame

        // spritesheet layout
        uint32_t sheetWidth;
        uint32_t sheetHeight;
        uint32_t frameWidth;
        uint32_t frameHeight;
    };

    struct SpriteAppearance
    {
        bool enabled = true;
        uint32_t variant = 0;
        uint32_t color = 0xFFFFFFFF;
    };

    struct CharacterAppearance
    {
        CharacterType characterType = CharacterType::HUMAN_MALE;
        std::unordered_map<SpriteType, SpriteAppearance> spriteAppearanceMap;
    };

    struct CharacterAnimationState
    {
        AnimationType currentAnim = AnimationType::RUN;
        Direction direction = Direction::S;

        float timeAccumulator = 0.0f;
        uint32_t currentFrame = 0;

        CharacterAppearance appearance;

        bool loop = true;
        bool done = false;
    };

    struct ClipRepository
    {
        std::unordered_map<AnimationType, AnimationClip> clipMap;
    };

}
