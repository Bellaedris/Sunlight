#version 460

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNorm;
layout(location = 2) in vec2 iTexcoord;

out vec3 position;
out vec3 normal;
out vec2 texcoord;

layout(std140, binding = 0) uniform CameraData
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

void main() {
    normal = iNorm;
    texcoord = iTexcoord;
    vec4 positionCS = projectionMatrix * viewMatrix * vec4(iPos, 1.f);
    position = iPos; // multioply by model matrix when we have one
    gl_Position = positionCS;
}
