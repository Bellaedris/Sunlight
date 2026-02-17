#version 460 core

in vec2 texcoord;

out vec4 FragColor;

uniform sampler2D ShadedNPR;
uniform sampler2D Outlines;

void main() {
    vec3 shaded = texture(ShadedNPR, texcoord).xyz;
    float outline = texture(Outlines, texcoord).x;
    FragColor = vec4(shaded * vec3(outline), 1.f);
}