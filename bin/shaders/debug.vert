#version 460

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iColor;

out vec3 viewSpacePosition;
out vec3 color;

layout(std140, binding = 0) uniform CameraData
{
    mat4 viewMatrix;
    mat4 inverseViewMatrix;
    mat4 projectionMatrix;
    mat4 inverseProjectionMatrix;
    vec3 position;
    float znear;
    float zfar;
};

void main() {
    color = iColor;

    viewSpacePosition = (viewMatrix * vec4(iPos, 1.f)).xyz;
    gl_Position = projectionMatrix * viewMatrix * vec4(iPos, 1.f);
}
