#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D u_InputTex;

void main() 
{
    FragColor = texture(u_InputTex, TexCoord);
}
