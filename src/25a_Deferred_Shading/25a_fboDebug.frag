// fragment shader
#version 330 core
out vec4 FragColor;
in  vec2 TexCoords;
  
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D sceneTex;
uniform int debugMode;

void main()
{
    vec2 texCoord = TexCoords;
    
    if (debugMode == 0) {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        return;
    }
    else if (debugMode == 1) {
        vec3 position = texture(gPosition, texCoord).rgb;
        FragColor = vec4(position * 0.1, 1.0);
    }
    else if (debugMode == 2) {
        vec3 normal = texture(gNormal, texCoord).rgb;
        FragColor = vec4(normal * 0.5 + 0.5, 1.0);
    }
    else if (debugMode == 3) {
        vec4 albedoSpec = texture(gAlbedoSpec, texCoord);
        FragColor = albedoSpec;
    }
    else if (debugMode == 4) {
        vec2 quadrantCoord = TexCoords * 2.0;
        vec4 color;

        if (TexCoords.x < 0.5 && TexCoords.y >= 0.5) {
            // Top-left: gPosition
            quadrantCoord = (TexCoords - vec2(0.0, 0.5)) * 2.0;
            vec3 position = texture(gPosition, quadrantCoord).rgb;
            color = vec4(position * 0.1, 1.0);
        }
        else if (TexCoords.x >= 0.5 && TexCoords.y >= 0.5) {
            // Top-right: gNormal
            quadrantCoord = (TexCoords - vec2(0.5, 0.5)) * 2.0;
            vec3 normal = texture(gNormal, quadrantCoord).rgb;
            color = vec4(normal, 1.0);
        }
        else if (TexCoords.x < 0.5 && TexCoords.y < 0.5) {
            // Bottom-left: gAlbedoSpec RGB
            quadrantCoord = TexCoords * 2.0;
            vec4 albedoSpec = texture(gAlbedoSpec, quadrantCoord);
            color = vec4(albedoSpec.rgb, 1.0);
        }
        else {
            // Bottom-right: Combine
            quadrantCoord = (TexCoords - vec2(0.5, 0.0)) * 2.0;
            vec3 position = texture(gPosition, quadrantCoord).rgb;
            vec3 normal = texture(gNormal, quadrantCoord).rgb;
            vec4 albedoSpec = texture(gAlbedoSpec, quadrantCoord);
            color = vec4(albedoSpec.rgb * 0.5 + normal * 0.25 + position * 0.1, 1.0);
        }

        float lineWidth = 0.001;
        vec4 lineColor = vec4(1.0, 1.0, 1.0, 1.0);
        
        if (abs(TexCoords.x - 0.5) < lineWidth) {
            FragColor = lineColor;
        }
        else if (abs(TexCoords.y - 0.5) < lineWidth) {
            FragColor = lineColor;
        }
        else if ((TexCoords.x < 0.5 && abs(TexCoords.y - 0.5) < lineWidth * 2.0) ||
                 (TexCoords.x >= 0.5 && abs(TexCoords.y - 0.5) < lineWidth * 2.0) ||
                 (TexCoords.y < 0.5 && abs(TexCoords.x - 0.5) < lineWidth * 2.0) ||
                 (TexCoords.y >= 0.5 && abs(TexCoords.x - 0.5) < lineWidth * 2.0)) {
            FragColor = lineColor;
        }
        else {
            FragColor = color;
        }
    }
} 