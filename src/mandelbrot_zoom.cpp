#include "../lib/glad/glad.h"
#include <GLFW/glfw3.h>

#include <cstddef>
#include <string>
#include <fstream>

#include "shaders.h"

GLFWwindow* create_window(const size_t width, const size_t height, const std::string& title);

int main() {
    const size_t width = 800, height = 400;

    GLFWwindow* window = create_window(width, height, "Mandelbrot Zoom");

    if(!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glViewport(0, 0, width, height);
    glClearColor(0.2, 0.3, 0.3, 1.0);

    std::ifstream vs_shader;
    vs_shader.open("shaders/texture_shader.vs");
    if(vs_shader.is_open()) shaders::load_shader(vs_shader, GL_VERTEX_SHADER);
    vs_shader.close();

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

GLFWwindow* create_window(const size_t width, const size_t height, const std::string& title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    return window;
}