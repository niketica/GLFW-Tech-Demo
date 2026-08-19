#pragma once

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <glm/glm.hpp>

namespace niketica::util
{


    glm::vec4 colorFromHexRGB(const char* hex);
    glm::vec4 colorFromHexRGB(const char* hex, float alphaOverride);
    glm::vec4 colorFromHexRGB(const char* hex, bool normalize);
    glm::vec4 colorFromHexRGB(const char* hex, float alphaOverride, bool normalize);

    uint32_t colorFromHexOpenGL(const char* hex, float alphaOverride = 1.0f);

    uint32_t mixColors(uint32_t base, uint32_t tint, float t);

    uint32_t colorFromVec4(const glm::vec4& color);

}
