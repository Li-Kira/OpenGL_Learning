#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    // diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

    // attenuation
    float d = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * (d * d));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec4 result = vec4(ambient + diffuse + specular, 1.0);
    FragColor = result;
}