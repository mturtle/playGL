#define GLFW_INCLUDE_NONE
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <dependencies/glm/vec3.hpp>

#include "shader.h"

#include <iostream>
#include <cmath>

static void error_callback(int error, const char* desc)
{
    std::cout << "Error: " << desc;
}

static void keypress_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

static void framesize_callback(GLFWwindow* window, int width, int height)
{
    std::cout << "Resizing: " << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}

static void update_framerate(GLFWwindow* window)
{
    static double previous_seconds = glfwGetTime();
    static int frame_count;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;

    if (elapsed_seconds > 0.25)
    {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf(tmp, "fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }

    frame_count++;
}

// -------------------------------------------------------------------

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1200, 1024, "glPlay", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keypress_callback);
    glfwSetFramebufferSizeCallback(window, framesize_callback);

    // opengl extension loader
    if (!gladLoadGL())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, 1200, 1024);

    //const int shaderProgram = CreateShaderProgram(vertex_shader_source, frag_shader_source);
    Shader basicShader(std::string("res/shaders/basic.vert"), std::string("res/shaders/basic.frag"));

    if (!basicShader.renderID)
    {
        std::cout << "Missing shader program, exiting...";
    }

    //vertex buffer stuff
    float triangle_verts[15] =
    {
        -0.6f, -0.4f, 1.0f, 0.0f, 0.0f,
         0.6f, -0.4f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 1.0f, 1.0f, 1.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verts), triangle_verts, GL_STATIC_DRAW);

    // describe vertex position attribute
    GLint vPosLocation = glGetAttribLocation(basicShader.renderID, "vPos");
    glEnableVertexAttribArray(vPosLocation);
    glVertexAttribPointer(vPosLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

    // describe vertex color attribute
    GLint vColLocation = glGetAttribLocation(basicShader.renderID, "vCol");
    glEnableVertexAttribArray(vColLocation);
    glVertexAttribPointer(vColLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 2));

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        update_framerate(window);

        glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        const float xFreq = 1.0f;
        const float yFreq = 0.5f;
        const glm::vec3 prim_pos (sin(glfwGetTime() * xFreq), sin(glfwGetTime() * yFreq), 0.0f); 
        glUniform3f(glGetUniformLocation(basicShader.renderID, "prim_pos"), prim_pos.x, prim_pos.y, prim_pos.z);

        basicShader.Bind();
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}