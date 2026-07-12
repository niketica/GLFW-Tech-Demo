#include "renderer/Shader.h"

namespace niketica::renderer
{
    Shader::Shader(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexCode = getShaderCode(vertexPath);
        std::string fragmentCode = getShaderCode(fragmentPath);

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex;
        unsigned int fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::checkCompileErrors(unsigned int shader, const char* type) const
    {
        int success;
        char infoLog[1024];

        if (std::strcmp(type, "PROGRAM") != 0)
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::Shader::checkCompileErrors::SHADER_COMPILATION_ERROR of type: " << type
                    << "\n" << infoLog
                    << "\n -- --------------------------------------------------- -- "
                    << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "ERROR::Shader::checkCompileErrors::PROGRAM_LINKING_ERROR of type: " << type
                    << "\n" << infoLog
                    << "\n -- --------------------------------------------------- -- "
                    << std::endl;
            }
        }
    }

    std::string Shader::getShaderCode(const char* path) const
    {
        auto loadedFile = niketica::asset::AssetManager::Get().Load<niketica::asset::File>(path);
        niketica::asset::File* file = loadedFile.get();
        auto& fileData = file->fileData;
        std::string shaderCode(fileData.begin(), fileData.end());
        return shaderCode;
    }

    unsigned int Shader::getUniformLocation(const std::string& name)
    {
        for (const auto& it : uniformLocations)
        {
            if (it.first == name) return it.second;
        }

        auto location = glGetUniformLocation(ID, name.c_str());
        uniformLocations.try_emplace(name, location);
        return location;
    }
}
