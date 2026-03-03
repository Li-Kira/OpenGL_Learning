#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D u_InputTexture;

void main() {
    // vec4 color = texture(u_InputTexture, TexCoord);
    vec4 color = vec4(0.5);
    FragColor = vec4(color);
}