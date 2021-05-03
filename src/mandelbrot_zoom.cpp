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

    const size_t width = 800, height = 600;
    double zoom = 1.0, off_x = 0.0, off_y = 0.0;

    GLFWwindow* window = create_window(width, height, "Mandelbrot Zoom");

    if(!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glViewport(0, 0, width, height);
    glClearColor(0.2, 0.3, 0.3, 1.0);

    unsigned int rect_vbo, rect_vao;
    glGenBuffers(1, &rect_vbo);
    glGenVertexArrays(1, &rect_vao);
    load_vertices(rect_vertices, rect_vbo, rect_vao);

    unsigned int render_texture = create_empty_texture(width, height, GL_CLAMP_TO_BORDER, GL_LINEAR);
    glBindImageTexture(0, render_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    unsigned int texture_shader; 
    {
        std::ifstream texture_vs_shader_file("shaders/texture_shader.vs");
        std::ifstream texture_fs_shader_file("shaders/texture_shader.fs");

        if(!texture_vs_shader_file.is_open() || !texture_fs_shader_file.is_open()) {
            glfwTerminate();
            return -1;
        }
        texture_shader = shaders::load_shader_program(texture_vs_shader_file, texture_fs_shader_file);
        texture_vs_shader_file.close();
        texture_fs_shader_file.close();
    }

    unsigned int mandelbrot_shader = glCreateProgram();
    {
        std::ifstream mandelbrot_shader_file("shaders/mandelbrot.cmp");

        if(!mandelbrot_shader_file.is_open()) {
            glfwTerminate();
            return -1;
        }

        unsigned int comp_shader = shaders::load_shader(mandelbrot_shader_file, GL_COMPUTE_SHADER);
        mandelbrot_shader_file.close();

        glAttachShader(mandelbrot_shader, comp_shader);
        glLinkProgram(mandelbrot_shader);

        glDeleteShader(comp_shader);

        int success;
        glGetProgramiv(mandelbrot_shader, GL_LINK_STATUS, &success);

        if(!success) {
            char info[512];
            glGetProgramInfoLog(mandelbrot_shader, 512, nullptr, info);
            throw std::runtime_error("Shader program compilation error: " + std::string(info));
        }
    }

    int zoom_location = glGetUniformLocation(mandelbrot_shader, "zoom");
    int offset_location = glGetUniformLocation(mandelbrot_shader, "offset");

    glBindVertexArray(rect_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render_texture);

    while(!glfwWindowShouldClose(window)) {
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) off_x -= zoom * 0.1;
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) off_x += zoom * 0.1;
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) off_y += zoom * 0.1;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) off_y -= zoom * 0.1;
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) zoom *= 1.25;
        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) zoom /= 1.25;

        glUseProgram(mandelbrot_shader);
        glUniform2d(offset_location, off_x, off_y);
        glUniform1d(zoom_location, zoom);
        glDispatchCompute(width / 32, height / 32, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(texture_shader);
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}