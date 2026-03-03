#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D u_InputTexture;

void main() {
    vec4 textColor = texture(u_InputTexture, TexCoord);
    FragColor = vec4(textColor);
}