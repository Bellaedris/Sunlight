#version 460 core

out vec4 FragColor;

#define PI 3.141592653589793
#define ONE_OVER_PI 0.3183098861837907

layout(binding = 0) uniform sampler2D GBufferAlbedo;
layout(binding = 1) uniform sampler2D GBufferNormals;
layout(binding = 2) uniform sampler2D GBufferPositions;
layout(binding = 3) uniform sampler2D GBufferMetalRough;
layout(binding = 5) uniform sampler2D GBufferDepth;

layout(std140, binding = 0) uniform CameraData
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
    vec3 viewPosition;
};

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
uniform int lightIndex;

vec3 SchlickFresnel(float cosTheta, vec3 R0)
{
    return R0 + (1.f - R0) * pow((1.f - cosTheta), 5.f);
}

// Trowbridge-Reitz GGX
float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2 = a * a;
    float NdotH = max(dot(N, H), .0f);
    float denom = ((NdotH * NdotH) * (a2 - 1.f) + 1.f);
    denom = PI * denom * denom;

    return a2 / denom;
}

float SchlickGGX(float NdotX, float a)
{
    float k = a * .5f;
    return NdotX / (NdotX * (1.f - k) + k + 0.0001);
}

float SmithGGX(float NdotL, float NdotV, float a)
{
    return SchlickGGX(NdotL, a) * SchlickGGX(NdotV, a);
}

vec3 PointLightContribution(vec3 position, vec3 albedo, vec3 normal, vec3 viewDir, float alpha, float metalness)
{
    vec3 F0 = vec3(.04f);
    F0 = mix(F0, albedo, metalness);

    PointLight l = pointLightData[lightIndex];
    // attenuation uses https://www.cemyuksel.com/research/pointlightattenuation/
    float dist = distance(position, l.position);
    float d2 = dist * dist;
    float r2 = l.radius * l.radius;
    vec3 radiance = l.color * l.intensity * (2.f / (d2 + r2 + dist * sqrt(d2 + r2)));

    vec3 lightDir = normalize(l.position - position);
    vec3 halfway = normalize(viewDir + lightDir);
    float VdotH = max(dot(halfway, viewDir), .0f);
    float NdotL = max(dot(normal, lightDir), .0f);
    float NdotV = max(dot(normal, viewDir), .0f);

    vec3 F = SchlickFresnel(VdotH, F0);

    vec3 kS = F;
    vec3 kD = 1.f - kS;
    kD *= 1. - metalness;

    float D = DistributionGGX(normal, halfway, alpha);
    float G = SmithGGX(NdotL, NdotV, alpha);

    vec3 num = D * F * G;
    float denom = 4.f * NdotL * NdotV;
    vec3 specular = num / (denom + .00001); // avoid division by 0 if a dot is null

    return (kD * albedo * ONE_OVER_PI + specular) * radiance * NdotL;
}

void main() {
    vec2 texcoord = vec2(gl_FragCoord.xy) / vec2(textureSize(GBufferAlbedo, 0));

    vec3 gDepth = texture(GBufferDepth, texcoord).xyz;

    if(gDepth.r >= 1.f)
        discard; // we discard the skybox for now

    vec3 gAlbedo = texture(GBufferAlbedo, texcoord).xyz;
    vec3 gNormal = texture(GBufferNormals, texcoord).xyz;
    vec3 gPos = texture(GBufferPositions, texcoord).xyz;
    vec3 gMetalRough = texture(GBufferMetalRough, texcoord).xyz;

    vec3 viewDir = normalize(viewPosition - gPos);
    float roughness = gMetalRough.g;
    float metalness = gMetalRough.b;
    float alpha = roughness * roughness;

    vec3 color = PointLightContribution(gPos, gAlbedo, gNormal, viewDir, alpha, metalness);


    FragColor = vec4(color, 1.f);
}