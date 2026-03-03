
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec3 Position;
out vec3 Normal;

void main() {
    Normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
    Position = vec3(u_ModelMatrix * vec4(position, 1.0));
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0);
}

