#version 330 core
out vec4 FragColor;

in vec3 position;
in vec2 TexCoords;
in vec3 normal;

// uniform vec3 ambient;
uniform vec3 materialColor = vec3(1.0, 1.0, 1.0);

uniform sampler2D albedoMap;

uniform vec3 lightPosition;

uniform bool useTexture;

uniform float opacity;

void main()
{   
    vec3 n = normalize(normal);

    vec3 albedo = vec3(1.0);
    
    if (useTexture)
    {
        albedo = texture(albedoMap, TexCoords).rgb;
    }

    vec3 ambient = albedo * 0.3;

    vec3 l1 = normalize(-vec3(-1.0, -1.0, 1.0));

    vec3 l2 = normalize(lightPosition - position);

    float diffuse1 = max(dot(n, l1), 0.0);
    float diffuse2 = max(dot(n, l2), 0.0);

    vec3 finalColor = ambient + (diffuse1 + diffuse2) * albedo * materialColor;
    // vec3 finalColor = ambient + (diffuse1 + diffuse2) * materialColor;

    // vec3 finalColor = ambient + albedo * diffuse;

    FragColor = vec4(finalColor, opacity);
    // FragColor = vec4(n, 1.0);
}