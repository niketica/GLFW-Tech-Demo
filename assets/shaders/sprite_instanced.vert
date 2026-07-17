#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

layout(location = 2) in vec2 iPos;
layout(location = 3) in vec2 iSize;
layout(location = 4) in vec2 iUVOffset;
layout(location = 5) in vec2 iUVScale;
layout(location = 6) in vec4 iColor;
layout(location = 7) in float iZ;
layout(location = 8) in float iRot;

out vec2 TexCoord;
out vec4 SpriteColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    // Transform quad local coords by size + rotation
    float c = cos(iRot);
    float s = sin(iRot);

    vec2 rotated = vec2(
        aPos.x * iSize.x * c - aPos.y * iSize.y * s,
        aPos.x * iSize.x * s + aPos.y * iSize.y * c
    );

    vec2 worldPos = rotated + iPos;

    gl_Position = projection * view * vec4(worldPos, iZ, 1.0);

    //TexCoord = iUVOffset + aUV * iUVScale; // Does not flip on Y axis
    TexCoord = iUVOffset + vec2(aUV.x, 1.0 - aUV.y) * iUVScale; // Flip on Y axis if needed

    SpriteColor = iColor;
}
