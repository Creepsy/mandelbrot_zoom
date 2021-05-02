#version 430 core

layout (location = 0) in vec3 pos;

out vec2 tex;

void main() {
    gl_Position = vec4(pos, 1.0);
    tex = vec2((pos.x + 1) / 2, (pos.y + 1) / 2);
}