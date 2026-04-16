#version 460

layout(location = 0) in vec3 iPos;
layout(location = 4) in vec2 iTexcoord;

out vec3 position;
out vec2 texcoord;

layout(std140, binding = 0) uniform CameraData
{
    mat4 viewMatrix;
    mat4 inverseViewMatrix;
    mat4 projectionMatrix;
    mat4 inverseProjectionMatrix;
    vec3 cameraPosition;
    float znear;
    float zfar;
};

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

void main() {
    texcoord = iTexcoord;

    vec4 positionCS = projectionMatrix * viewMatrix * modelMatrix * vec4(iPos, 1.f);
    gl_Position = positionCS;
}
