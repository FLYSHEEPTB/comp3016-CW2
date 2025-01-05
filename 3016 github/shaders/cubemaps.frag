#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

uniform vec3 color = vec3(1.0);

void main()
{    
    vec3 n = normalize(Normal);

    vec3 l1 = -vec3(0.8, -0.9, -1.0);
    vec3 l2 = -vec3(0.8, -0.9, 1.0);

    vec3 diffuse1 = max(dot(n, l1), 0.0) * color;
    vec3 diffuse2 = max(dot(n, l2), 0.0) * color;

    FragColor = vec4((diffuse1 + diffuse2) * 0.5, 1.0);
}