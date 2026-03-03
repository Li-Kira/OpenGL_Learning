#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main() {
    // ambient
    float ambientIns = 0.1;
    vec3 ambient = lightColor * ambientIns;

    // diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = lightPos - FragPos;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularIns = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularIns * spec * lightColor; 

    vec4 result = vec4((ambient + diffuse + specular) * objectColor, 1.0);
    FragColor = result;
}