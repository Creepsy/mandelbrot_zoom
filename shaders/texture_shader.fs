#version 430

in vec2 tex;

uniform sampler2D albedo;

out vec4 FragColor;

void main() {
    FragColor = texture(albedo, tex);
}