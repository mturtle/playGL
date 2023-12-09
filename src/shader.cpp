#include "shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);
    std::string line;

    while (std::getline(vertexFile, line))
    {
        vertexCode += line;
        vertexCode.push_back('\n');
    }

    while (std::getline(fragmentFile, line))
    {
        fragmentCode += line;
        fragmentCode.push_back('\n');
    }
    
    renderID = CreateShaderProgram(vertexCode.c_str(), fragmentCode.c_str());
}

Shader::~Shader()
{
}

void Shader::Bind()
{
    glUseProgram(renderID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}
