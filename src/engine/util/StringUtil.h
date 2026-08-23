#pragma once

#include <string>
#include <iomanip>
#include <sstream>

namespace niketica::util::string
{

    static inline std::string parseFloat(float f, int precision)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << f;
        return stream.str();
    }

}
