
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0);
    TexCoord = texcoord;
}

