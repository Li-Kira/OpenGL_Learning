
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out VS_OUT {
    vec2 texCoords;
} vs_out;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

void main() {
    vs_out.texCoords = texcoord;
    vec4 pos = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0);
    gl_Position = pos;
}

