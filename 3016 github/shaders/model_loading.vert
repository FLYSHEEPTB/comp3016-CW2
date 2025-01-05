#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aColor;

out vec3 position;
out vec2 TexCoords;
out vec3 normal;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    position = (model * vec4(aPos, 1.0)).xyz;
    TexCoords = aTexCoords;
    normal = (model * vec4(aNormal, 0.0)).xyz;
    color = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}