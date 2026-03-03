
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aColor;

out VS_OUT {
    vec3 color;
} vs_out;

void main() {
    vs_out.color = aColor;
    vec4 pos = vec4(position.x, position.y, 0.0, 1.0);
    gl_Position = pos;
}

