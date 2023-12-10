#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <dependencies/glm/vec3.hpp>

#include <iostream>

enum ShaderType { 
    VERTEX_SHADER = GL_VERTEX_SHADER, 
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER 
};

static const char* sample_vert_shader_source = {
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

static const char* sample_frag_shader_source = {
    "#version 330 core\n"
    "varying vec3 color;\n"
    "uniform vec4 overColor;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = vec4(color, 1.0);\n"
    "}"
};

namespace ShaderHelpers
{
    static int CreateShader(ShaderType Type, const char* ShaderSource);
    static int CreateShaderProgram(const char* VertexShaderSource, const char* FragmentShaderSource);
}

class Shader
{
    public:
    unsigned int renderID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Bind();
    void Unbind();
    void SetUniformFloat3(const std::string& name, const glm::vec3 value);
};