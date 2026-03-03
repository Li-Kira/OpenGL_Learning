#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec2 TexCoord;

void main() {
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0);
}