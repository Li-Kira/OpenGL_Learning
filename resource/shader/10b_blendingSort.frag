#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D u_InputTex;

void main() 
{
    vec4 texColor = texture(u_InputTex, TexCoord);
    FragColor = texColor;
}
