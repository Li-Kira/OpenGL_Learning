
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

void main() {
    vs_out.FragPos = position;
    vs_out.Normal = normal;
    vs_out.TexCoords = texcoord;
    vec4 pos = u_ProjectionMatrix * u_ViewMatrix * vec4(position, 1.0);
    gl_Position = pos;
}

