#pragma once

#include "../lib/glad/glad.h"
#include <GLFW/glfw3.h>

#include <istream>

namespace shaders {
    unsigned int load_shader_program(std::istream& fragment_src, std::istream& vertex_src);
    unsigned int load_shader(std::istream& src, const GLenum shader_type);
}