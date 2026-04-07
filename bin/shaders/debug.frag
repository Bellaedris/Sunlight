#version 460

in vec3 viewSpacePosition;
in vec3 color;

out vec4 FragColor;

uniform sampler2D depth;

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

vec3 ViewPositionFromDepth(vec2 uv)
{
    float z = texture(depth, uv).x * 2.f - 1.f;
    float x = uv.x * 2.f - 1.f;
    float y = uv.y * 2.f - 1.f;

    vec4 viewSpacePos = inverseProjectionMatrix * vec4(x, y, z, 1.f);
    return viewSpacePos.xyz / viewSpacePos.w;
}

void main() {
    vec2 uv = vec2(gl_FragCoord) / textureSize(depth, 0);
    float pixelDepth = ViewPositionFromDepth(uv).z;

    if(viewSpacePosition.z + .01 < pixelDepth)
        discard;

    FragColor = vec4(color, 1.0);
}