#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TextCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TextCoords = aTextCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection* view * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
