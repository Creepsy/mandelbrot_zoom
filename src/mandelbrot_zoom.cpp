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

    std::ifstream texture_vs_shader("shaders/texture_shader.vs");
    std::ifstream texture_fs_shader("shaders/texture_shader.fs");

    if(!texture_vs_shader.is_open() || !texture_fs_shader.is_open()) {
        glfwTerminate();
        return -1;
    }
    unsigned int texture_shader = shaders::load_shader_program(texture_vs_shader, texture_fs_shader);
    texture_vs_shader.close();
    texture_fs_shader.close();

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