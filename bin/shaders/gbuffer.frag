#version 460

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in mat3 tbn;

layout(location = 0) out vec4 gAlbedo;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gPosition;
layout(location = 3) out vec3 gMetalRough;
layout(location = 4) out vec3 gEmissive;

uniform sampler2D AlbedoTexture;
uniform sampler2D NormalsTexture;
uniform sampler2D MetalRoughTexture;
uniform sampler2D EmissiveTexture;

uniform int hasNormals = 0;

void main() {
    gAlbedo = texture(AlbedoTexture, texcoord);
    if(hasNormals == 0)
    {
        vec3 norm = texture(NormalsTexture, texcoord).rgb;// this is in 0..1
        norm = norm * 2.f - 1.f;// remap it to -1-; 1
        gNormal = normalize(tbn * norm);// transform it to tangent space
    }
    else
        gNormal = normal;
    gPosition = vec4(position, 1.f).xyz;
    gMetalRough = texture(MetalRoughTexture, texcoord).xyz;
    gEmissive = texture(EmissiveTexture, texcoord).xyz;
}