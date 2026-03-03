#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D u_InputTexture0;
uniform sampler2D u_InputTexture1;

void main() {
    FragColor = texture(u_InputTexture1, TexCoord);
    // FragColor = vec4(1.0);
}