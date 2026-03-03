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

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 4

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 viewPos;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() 
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 resultColor = CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        resultColor += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

    resultColor += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

    vec4 result = vec4(resultColor, 1.0);
    FragColor = result;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Results
    vec3 ambientColor = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuseColor = light.diffuse * diffuse * vec3(texture(material.diffuse, TexCoord));
    vec3 specularColor = light.specular * specular * vec3(texture(material.specular, TexCoord));
    
    return (ambientColor + diffuseColor + specularColor);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * (d * d));    
    // Results
    vec3 ambientColor = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuseColor = light.diffuse * diffuse * vec3(texture(material.diffuse, TexCoord));
    vec3 specularColor = light.specular * specular * vec3(texture(material.specular, TexCoord));
    ambientColor *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;

    return (ambientColor + diffuseColor + specularColor);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // Results
    vec3 ambientColor = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuseColor = light.diffuse * diffuse * vec3(texture(material.diffuse, TexCoord));
    vec3 specularColor = light.specular * specular * vec3(texture(material.specular, TexCoord));
    ambientColor *= attenuation * intensity;
    diffuseColor *= attenuation * intensity;
    specularColor *= attenuation * intensity;

    return (ambientColor + diffuseColor + specularColor);
}