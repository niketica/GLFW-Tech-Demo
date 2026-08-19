#include "engine/util/ColorUtil.h"

namespace niketica::util
{

    uint32_t colorFromHexOpenGL(const char* hex, float alphaOverride)
    {
        auto color = colorFromHexRGB(hex, alphaOverride, false);

        // Pack as ABGR for OpenGL
        return (uint32_t(color.w) << 24) |
            (uint32_t(color.z) << 16) |
            (uint32_t(color.y) << 8) |
            uint32_t(color.x);
    }

    glm::vec4 colorFromHexRGB(const char* hex)
    {
        return colorFromHexRGB(hex, 1.0f, true);
    }

    glm::vec4 colorFromHexRGB(const char* hex, float alphaOverride)
    {
        return colorFromHexRGB(hex, alphaOverride, true);

    }
    
    glm::vec4 colorFromHexRGB(const char* hex, bool normalize)
    {
        return colorFromHexRGB(hex, 1.0f, normalize);
    }
    
    glm::vec4 colorFromHexRGB(const char* hex, float alphaOverride, bool normalize)
    {
        // Skip leading '#'
        if (hex[0] == '#')
        {
            ++hex;
        }

        size_t len = std::strlen(hex);

        uint32_t value = std::strtoul(hex, nullptr, 16);

        uint8_t r, g, b, a;

        if (len == 6) // RRGGBB
        {
            r = (value >> 16) & 0xFF;
            g = (value >> 8) & 0xFF;
            b = value & 0xFF;
            a = static_cast<uint8_t>(
                std::clamp(alphaOverride, 0.0f, 1.0f) * 255.0f
                );
        }
        else if (len == 8) // RRGGBBAA
        {
            r = (value >> 24) & 0xFF;
            g = (value >> 16) & 0xFF;
            b = (value >> 8) & 0xFF;
            a = value & 0xFF;

            a = static_cast<uint8_t>(
                a * std::clamp(alphaOverride, 0.0f, 1.0f)
                );
        }
        else
        {
            // Invalid input -> magenta debug color
            r = 255; g = 0; b = 255; a = 255;
        }

        if (normalize)
        {
            return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
        }
        else
        {
            return {r, g, b, a};
        }
    }

    uint32_t mixColors(uint32_t base, uint32_t tint, float t)
    {
        auto unpack = [](uint32_t c, int shift) {
            return float((c >> shift) & 0xFF);
            };

        float br = unpack(base, 24);
        float bg = unpack(base, 16);
        float bb = unpack(base, 8);
        float ba = unpack(base, 0);

        float tr = unpack(tint, 24);
        float tg = unpack(tint, 16);
        float tb = unpack(tint, 8);

        uint8_t r = uint8_t(glm::mix(br, tr, t));
        uint8_t g = uint8_t(glm::mix(bg, tg, t));
        uint8_t b = uint8_t(glm::mix(bb, tb, t));
        uint8_t a = uint8_t(ba);

        return (r << 24) | (g << 16) | (b << 8) | a;
    }
    
    uint32_t colorFromVec4(const glm::vec4& color)
    {
        auto clampToByte = [](float v) -> uint8_t
            {
                return static_cast<uint8_t>(
                    std::clamp(v, 0.0f, 1.0f) * 255.0f + 0.5f // +0.5 for proper rounding
                    );
            };

        uint8_t r = clampToByte(color.r);
        uint8_t g = clampToByte(color.g);
        uint8_t b = clampToByte(color.b);
        uint8_t a = clampToByte(color.a);

        return (uint32_t(a) << 24) |
            (uint32_t(b) << 16) |
            (uint32_t(g) << 8) |
            uint32_t(r);
    }

}
