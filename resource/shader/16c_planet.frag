#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D texture_diffuse1;

struct DirLight {
    vec3 direction;
};

uniform DirLight dirLight;


void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float diffuse = max(dot(norm, lightDir), 0.0);
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    vec4 finalColor = vec4(color * diffuse, 1.0);

    FragColor = finalColor;
}
