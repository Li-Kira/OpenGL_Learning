
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

void main() {
    mat3 normalMatrix = mat3(transpose(inverse(u_ViewMatrix * u_ModelMatrix)));
    vs_out.normal = vec3(vec4(normalMatrix * aNormal, 0.0));
    vec4 pos = u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0); 
    gl_Position = pos;
}

