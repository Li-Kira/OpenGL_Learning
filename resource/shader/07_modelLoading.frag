#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;

out vec4 FragColor;

void main() 
{
    vec4 color = texture(texture_diffuse1, TexCoord);
    FragColor = vec4(color);
}
