#pragma once

#include <string>
#include <iomanip>
#include <sstream>
#include <vector>

namespace niketica::util::string
{

    static inline std::string parseFloat(float f, int precision)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << f;
        return stream.str();
    }

    static inline std::vector<std::string> splitLines(const std::string& input)
    {
        std::vector<std::string> lines;
        std::istringstream stream(input);
        std::string line;        
        while (std::getline(stream, line)) {
            lines.push_back(line);
        }
        return lines;
    }

}
