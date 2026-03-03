#version 330 core

in vec3 Position;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() 
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    // vec3 R = reflect(I, normalize(Normal));
    vec3 R = refract(I, normalize(Normal), ratio);
    // R.yz = -R.yz;
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
