#pragma once

#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "asset/File.h"
#include "asset/AssetManager.h"

namespace niketica::renderer
{
    class Shader
    {
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader() = default;

        void use() const
        {
            glUseProgram(ID);
        }

        void setBool(const std::string& name, bool value)
        {
            glUniform1i(getUniformLocation(name), (int)value);
        }

        void setInt(const std::string& name, int value)
        {
            glUniform1i(getUniformLocation(name), value);
        }

        void setFloat(const std::string& name, float value)
        {
            glUniform1f(getUniformLocation(name), value);
        }

        void setFloat3(const std::string& name, float v0, float v1, float v2)
        {
            glUniform3f(getUniformLocation(name), v0, v1, v2);
        }

        void setFloat4(const std::string& name, float v0, float v1, float v2, float v3)
        {
            glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
        }

        void setVec2(const std::string& name, const glm::vec2& value)
        {
            glUniform2fv(getUniformLocation(name), 1, &value[0]);
        }

        void setVec3(const std::string& name, const glm::vec3& value)
        {
            glUniform3fv(getUniformLocation(name), 1, &value[0]);
        }

        void setVec4(const std::string& name, const glm::vec4& value)
        {
            glUniform4fv(getUniformLocation(name), 1, &value[0]);
        }

        void setMat4(const std::string& name, const glm::mat4& value)
        {
            glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
        }

    private:

        std::unordered_map<std::string, unsigned int> uniformLocations;

        void checkCompileErrors(unsigned int shader, const char* type) const;
        
        std::string getShaderCode(const char* path) const;

        unsigned int getUniformLocation(const std::string& name);
    };
}

