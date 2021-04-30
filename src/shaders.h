#pragma once

#include "../lib/glad/glad.h"
#include <GLFW/glfw3.h>

#include <istream>

namespace shaders {
    unsigned int load_shader_program(std::istream& vertex_src, std::istream& fragment_src);
    unsigned int load_shader(std::istream& src, const GLenum shader_type);
}