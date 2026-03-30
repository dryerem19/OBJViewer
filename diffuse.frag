#version 330 core

struct Material
{
    float   shiness;
    vec3    diffuseColor;
    vec3    ambienceColor;
    vec3    specularColor;
};

uniform Material    u_material;
uniform sampler2D   u_diffuseMap;
uniform bool        u_useDiffuseMap;

uniform vec3 uLightPos      = vec3(0.6f, 0.7f, 1.0f);
uniform vec3 uLightColor    = vec3(1, 1, 1);

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

void main(void)
{
    vec4 diffuseObjectColor = texture(u_diffuseMap, UV);
    diffuseObjectColor *= vec4(u_material.diffuseColor, 1.0);
    if (u_useDiffuseMap == false) { diffuseObjectColor = vec4(u_material.diffuseColor, 1.0); }

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

    // SPECULAR COLOR
    //


    // COMBINE COLOR
    //
    vec4    result      = vec4((ambient + diffuse), 1.0) * diffuseObjectColor;
    FragColor           = result;
}
