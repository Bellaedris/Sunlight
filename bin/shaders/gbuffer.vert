#version 460

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNorm;
layout(location = 2) in vec3 iTangent;
layout(location = 3) in vec3 iBitangent;
layout(location = 4) in vec2 iTexcoord;

out vec3 position;
out vec3 normal;
out vec2 texcoord;
out mat3 tbn;

layout(std140, binding = 0) uniform CameraData
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
    vec3 camPosition;
};

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

void main() {
    normal = iNorm;
    texcoord = iTexcoord;
    position = (modelMatrix * vec4(iPos, 1.f)).xyz; // multiply by model matrix when we have one

    vec3 T = normalize(vec3(normalMatrix * vec4(iTangent, .0f)));
    vec3 B = normalize(vec3(normalMatrix * vec4(iBitangent, .0f)));
    vec3 N = normalize(vec3(normalMatrix * vec4(iNorm, .0f)));
    tbn = mat3(T, B, N);

    vec4 positionCS = projectionMatrix * viewMatrix * modelMatrix * vec4(iPos, 1.f);
    gl_Position = positionCS;
}
