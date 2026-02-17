#version 460 core

in vec2 texcoord;

out vec4 FragColor;

uniform sampler2D GBufferAlbedo;
uniform sampler2D GBufferNormals;
uniform sampler2D GBufferPositions;
uniform sampler2D GBufferDepth;
uniform sampler2D PencilShadows;

#define MAX_POINT_LIGHTS 50
struct PointLight
{
    vec3 position;
    float intensity;
    vec3 color;
    float radius;
};
layout(std140, binding = 1) uniform PointLights
{
    PointLight[MAX_POINT_LIGHTS] pointLightData;
};
uniform int pointLightCount;

#define MAX_DIR_LIGHTS 50
struct DirectionalLight
{
    vec3 direction;
    float intensity;

    vec3 color;
    float pad;
};
layout(std140, binding = 2) uniform DirectionalLights
{
    DirectionalLight[MAX_DIR_LIGHTS] dirLightData;
};
uniform int dirLightCount;

float Posterize(float cosTheta)
{
    return step(.25f, cosTheta) * .25f +
    step(.5f, cosTheta) * .5f +
    step(.75f, cosTheta) * .75f;
}

float CalculateDirectionalLight(DirectionalLight l, vec3 normal)
{
    float cosTheta = max(dot(normalize(-l.direction), normal), .0f);

    vec3 pencilMask = texture(PencilShadows, texcoord * 15.f).xyz;

    float pencil = max(max(step(cosTheta, .25f) * pencilMask.b,
    step(cosTheta, .5f) * pencilMask.r),
    step(cosTheta, .75f) * pencilMask.g);

    return 1.f - pencil;
}

void main() {
    vec3 gAlbedo = texture(GBufferAlbedo, texcoord).xyz;
    vec3 gNormal = texture(GBufferNormals, texcoord).xyz;
    vec3 gPos = texture(GBufferPositions, texcoord).xyz;
    vec3 gDepth = texture(GBufferDepth, texcoord).xyz;

    if(gDepth.r >= 1.f)
        discard; // we discard the skybox for now

    vec3 color = vec3(0, 0, 0);
    for(int i = 0; i < dirLightCount; i++)
    {
        color += gAlbedo * CalculateDirectionalLight(dirLightData[i], gNormal);
    }

    FragColor = vec4(color, 1.f);
}