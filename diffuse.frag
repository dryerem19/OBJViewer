#version 330 core

uniform vec3 uLightPos      = vec3(0.6f, 0.7f, 1.0f);
uniform vec3 uLightColor    = vec3(1, 1, 1);
uniform vec3 uObjectColor   = vec3(0.8, 0.8, 0.8);

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

void main(void)
{
    // AMBIENT COLOR
    //
    float   ambientStrength = 0.1;
    vec3    ambient         = ambientStrength * uLightColor;

    // DIFFUSE COLOR
    //
    vec3    norm        = normalize(Normal);
    vec3    lightDir    = normalize(uLightPos - FragPos);

    float   diff        = max(dot(norm, lightDir), 0.0);
    vec3    diffuse     = diff * uLightColor;

    // COMBINE COLOR
    //
    vec3    result      = (ambient + diffuse) * uObjectColor;
    FragColor           = vec4(result, 1.0);
}
