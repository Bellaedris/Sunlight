#version 460

layout(location = 0) in vec3 iPos;

uniform mat4 lightspaceMatrix;
uniform mat4 modelMatrix;

void main() {
    gl_Position = lightspaceMatrix * modelMatrix * vec4(iPos, 1.0);
}