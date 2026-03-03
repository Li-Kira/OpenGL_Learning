
#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec3 TexCoord;

void main() {
    TexCoord = position;

    vec4 pos = u_ProjectionMatrix * u_ViewMatrix * vec4(position, 1.0);
    gl_Position = pos.xyww;    
}

