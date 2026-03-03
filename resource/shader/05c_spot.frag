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
    vec3 direction;
    float cutOff;

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

    vec3 lightDir = normalize(light.position - FragPos);
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction)); 

    vec4 result = vec4(0.0);

    if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

        // diffuse
        vec3 normal = normalize(Normal);
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

        // ambient *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branch
        diffuse *= attenuation;
        specular *= attenuation;

        result = vec4(ambient + diffuse + specular, 1.0);
    }
    else
    {
        // else, use ambient light so scene isn't completely dark outside the spotlight.
        result = vec4(light.ambient * texture(material.diffuse, TexCoord).rgb, 1.0);
    }

    
    FragColor = result;
}