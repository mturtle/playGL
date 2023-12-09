#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

enum ShaderType { 
    VERTEX_SHADER = GL_VERTEX_SHADER, 
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER 
};

static int CreateShader(ShaderType Type, const char* ShaderSource)
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

static int CreateShaderProgram(const char* VertexShaderSource, const char* FragmentShaderSource)
{
    const int ShaderProgram = glCreateProgram();

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
    glGetProgramiv(ShaderProgram, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(ShaderProgram, 512, &LogLength, Log);
        std::cout << "Error linking shader program: " << Log << std::endl;
        return 0;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    return ShaderProgram;
}

class Shader
{
    public:
    unsigned int renderID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Bind();
    void Unbind();
};

static const char* vertex_shader_source = {
    "#version 330 core\n"
    "attribute vec2 vPos;\n"
    "attribute vec3 vCol;\n"
    "uniform vec3 prim_pos;\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "vec3 pos = vec3(vPos, 0.0) + prim_pos;\n"
    "gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);\n"
    "color = vCol;\n"
    "}"
};

static const char* frag_shader_source = {
    "#version 330 core\n"
    "varying vec3 color;\n"
    "uniform vec4 overColor;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = vec4(color, 1.0);\n"
    "}"
};