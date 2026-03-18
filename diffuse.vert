#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

void main(void)
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPosition, 1.0);
    FragPos     = vec3(modelMatrix * vec4(aPosition, 1.0));
    Normal      = aNormal;
    UV          = aUV;
}
