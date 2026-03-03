#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
    Normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
    TexCoords = aTexCoords;
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(aPos, 1.0f); 
}