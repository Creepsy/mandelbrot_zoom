#include "shaders.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>

unsigned int shaders::load_shader_program(std::istream& fragment_src, std::istream& vertex_src) {
    return 0;
}

unsigned int shaders::load_shader(std::istream& src, const GLenum shader_type) {
    const size_t start = src.tellg();
    src.seekg(0, std::ios::end);
    std::cout << src.tellg() << std::endl;
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