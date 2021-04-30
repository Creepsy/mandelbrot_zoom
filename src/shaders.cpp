#include "shaders.h"

#include <cstddef>
#include <stdexcept>

unsigned int shaders::load_shader_program(std::istream& vertex_src, std::istream& fragment_src) {
    unsigned int vertex_shader = load_shader(vertex_src, GL_VERTEX_SHADER);
    unsigned int fragment_shader = load_shader(fragment_src, GL_FRAGMENT_SHADER);

    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    
    if(!success) {
        char info[512];
        glGetProgramInfoLog(shader_program, 512, nullptr, info);
        throw std::runtime_error("Shader program compilation error: " + std::string(info));
    }

    return shader_program;
}

unsigned int shaders::load_shader(std::istream& src, const GLenum shader_type) {
    const size_t start = src.tellg();
    src.seekg(0, std::ios::end);
    const size_t size = (size_t)src.tellg() - start;
    src.seekg(start, std::ios::beg);

    std::string shader_str;
    shader_str.resize(size);
    src.read(&shader_str[0], size);
    unsigned int shader = glCreateShader(shader_type);
    const char* shader_ptr = shader_str.c_str();
    glShaderSource(shader, 1, &shader_ptr, nullptr);
    glCompileShader(shader);
    
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if(!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        throw std::runtime_error("Shader compilation error: " + std::string(info));
    }

    return shader;
}