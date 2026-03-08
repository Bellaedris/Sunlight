#version 460 core

layout(location = 0) in vec3 iPos;

layout(std140, binding = 0) uniform CameraData
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
    vec3 viewPosition;
};

uniform mat4 modelMatrix;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(iPos, 1.f);
}
