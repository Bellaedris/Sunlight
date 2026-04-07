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

uniform float sketchTextureTiling;

float Posterize(float cosTheta)
{
    return step(.25f, cosTheta) * .25f +
    step(.5f, cosTheta) * .5f +
    step(.75f, cosTheta) * .75f;
}

float CalculateSketchingFactor(float luminance)
{
    vec3 pencilMask = texture(PencilShadows, texcoord * sketchTextureTiling).xyz;

    float pencil = max(max(step(luminance, .25f) * pencilMask.b,
    step(luminance, .5f) * pencilMask.r),
    step(luminance, .75f) * pencilMask.g);

    return (1.f - pencil);
}

vec3 CalculateDirectionalLight(vec3 normal, inout float luminance)
{
    vec3 col = vec3(.0f);
    for(int i = 0; i < dirLightCount; i++)
    {
        DirectionalLight l = dirLightData[i];
        vec3 radiance = l.color * l.intensity;
        float cosTheta = max(dot(normalize(-l.direction), normal), .0f);
        luminance += cosTheta * l.intensity;
        col += cosTheta * radiance;
    }

    return col;
}

vec3 CalculatePointLight(vec3 pos, vec3 normal, inout float luminance)
{
    vec3 col = vec3(.0f);
    for(int i = 0; i < pointLightCount; i++)
    {
        PointLight l = pointLightData[i];
        vec3 lightDir = normalize(l.position - pos);
        // attenuation uses https://www.cemyuksel.com/research/pointlightattenuation/
        float dist = distance(pos, l.position);
        float d2 = dist * dist;
        float r2 = l.radius * l.radius;
        float attenuation = l.intensity * (2.f / (d2 + r2 + dist * sqrt(d2 + r2)));
        vec3 radiance = l.color * attenuation;

        float cosTheta = max(dot(normalize(lightDir), normal), .0f);
        luminance += cosTheta * attenuation;
        col += cosTheta * radiance;
    }

    return col;
}

void main() {
    vec3 gAlbedo = texture(GBufferAlbedo, texcoord).xyz;
    vec3 gNormal = texture(GBufferNormals, texcoord).xyz;
    vec3 gPos = texture(GBufferPositions, texcoord).xyz;
    vec3 gDepth = texture(GBufferDepth, texcoord).xyz;

    if(gDepth.r >= 1.f)
        discard; // we discard the skybox for now

    vec3 color = vec3(0, 0, 0);
    float luminance = .0f;
    color += gAlbedo * CalculateDirectionalLight(gNormal, luminance);
    color += gAlbedo * CalculatePointLight(gPos, gNormal, luminance);
    color = color * CalculateSketchingFactor(luminance);

    FragColor = vec4(color, 1.f);
}