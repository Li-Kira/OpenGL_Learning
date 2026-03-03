#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec3 Normal;
out vec2 TexCoords;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(aInstanceMatrix)));
    Normal = normalMatrix * aNormal;
    TexCoords = aTexCoords;
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * aInstanceMatrix * vec4(aPos, 1.0f); 
}