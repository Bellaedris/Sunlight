#version 460

in vec3 position;
in vec2 texcoord;

out vec4 FragColor;

uniform sampler2D AlbedoTexture;
uniform sampler2D EmissiveTexture;

uniform vec3 albedoColor;
uniform vec3 emissionColor;
uniform float emissionStrength;

void main() {
    vec4 color = texture(AlbedoTexture, texcoord);
    if(color.a < .1f)
        discard;

    FragColor = color * vec4(albedoColor, 1.f) + texture(EmissiveTexture, texcoord) * vec4(emissionColor, 1.f) * emissionStrength;
}