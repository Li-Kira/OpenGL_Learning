
#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 u_ModelMatrix;

layout (std140) uniform Matrices
{
    mat4 u_ProjectionMatrix;
    mat4 u_ViewMatrix;
};

out vec3 TexCoord;

void main() {
    vec4 pos = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0);
    gl_Position = pos;
}

