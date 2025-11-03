#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec4 fragColor;

out vec4 finalColor;

// Point light properties
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    vec3 lightDir = lightPosition - fragPosition;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    
    // Attenuation (light intensity decreases with distance)
    float a = 0.002;
    float b = 0.00004;
    float attenuation = 1.0 / (1.0 + a * distance + b * distance * distance);
    
    // Diffuse lighting
    vec3 normal = normalize(fragNormal);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular lighting (optional)
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * lightColor * 0.13;
    
    // Ambient lighting
    vec3 ambient = vec3(0.1, 0.1, 0.1);
    
    // Combine all lighting components
    vec3 lighting = ambient + (diffuse + specular) * attenuation;
    
    // Apply lighting to the vertex color
    finalColor = vec4(lighting * fragColor.rgb, fragColor.a);
}