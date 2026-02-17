#version 460 core

layout(location = 0) in vec3 iPos;
layout(location = 4) in vec2 iTexcoord;

out vec2 texcoord;

void main() {
    texcoord = iTexcoord;
    gl_Position = vec4(iPos, 1.f);
}
