#include "shader.h"
#include <iostream>
#include <fstream>

static int ShaderHelpers::CreateShader(ShaderType Type, const char* ShaderSource)
{
    const unsigned int Shader = glCreateShader(Type);
    glShaderSource(Shader, 1, &ShaderSource, NULL);
    glCompileShader(Shader);

    int Success, LogLength;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        char Log[512];
        glGetShaderInfoLog(Shader, 512, &LogLength, Log);
        std::cout << "Error compiling " << Type << " shader: " << Log << std::endl;
        return 0;
    }

    return Shader;
}

static int ShaderHelpers::CreateShaderProgram(const char* VertexShaderSource, const char* FragmentShaderSource)
{
    const int ShaderProgram = glCreateProgram();
    
    //std::cout << "Creating shader program with vert: \n" << VertexShaderSource << std::endl;
    //std::cout << "Creating shader program with frag: \n" << FragmentShaderSource << std::endl;

    int VertexShader = CreateShader(VERTEX_SHADER, VertexShaderSource);
    int FragmentShader = CreateShader(FRAGMENT_SHADER, FragmentShaderSource);

    if (!VertexShader || !FragmentShader)
    {
        return 0;
    }

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    int Success, LogLength;
    char Log[512];
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(ShaderProgram, 512, NULL, Log);
        std::cout << "Error linking shader program: \n" << Log << std::endl;
        return 0;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    return ShaderProgram;
}

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
    
    renderID = ShaderHelpers::CreateShaderProgram(vertexCode.c_str(), fragmentCode.c_str());
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

void Shader::SetUniformFloat3(const std::string &name, const glm::vec3 value)
{
    glUniform3f(glGetUniformLocation(renderID, name.c_str()), value.x, value.y, value.z);
}
