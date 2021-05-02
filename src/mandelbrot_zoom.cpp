#include "../lib/glad/glad.h"
#include <GLFW/glfw3.h>

#include <cstddef>
#include <string>
#include <fstream>
#include <vector>

#include "shaders.h"

GLFWwindow* create_window(const size_t width, const size_t height, const std::string& title);
void load_vertices(const std::vector<float>& vertices, unsigned int& vbo, unsigned int& vao);
unsigned int create_empty_texture(const size_t width, const size_t height, const GLint texture_wrap, const GLint texture_filter);

int main() {
    const std::vector<float> rect_vertices = {
        -1.0, -1.0, 0.0,
        -1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,

        -1.0, -1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, -1.0, 0.0
    };

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

    unsigned int rect_vbo, rect_vao;
    glGenBuffers(1, &rect_vbo);
    glGenVertexArrays(1, &rect_vao);
    load_vertices(rect_vertices, rect_vbo, rect_vao);

    glBindVertexArray(rect_vao);

    unsigned int texture_shader = shaders::load_shader_program(texture_vs_shader, texture_fs_shader);
    texture_vs_shader.close();
    texture_fs_shader.close();

    unsigned int render_texture = create_empty_texture(width, height, GL_CLAMP_TO_BORDER, GL_LINEAR);

    glUseProgram(texture_shader);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(texture_shader);
    glDeleteVertexArrays(1, &rect_vao);
    glDeleteBuffers(1, &rect_vbo);
    glDeleteTextures(1, &render_texture);

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

void load_vertices(const std::vector<float>& vertices, unsigned int& vbo, unsigned int& vao) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int create_empty_texture(const size_t width, const size_t height, const GLint texture_wrap, const GLint texture_filter) {
    unsigned int texture;
    glGenTextures(1, &texture);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_filter);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    return texture;
}