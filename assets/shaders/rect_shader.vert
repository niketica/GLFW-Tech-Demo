#version 330 core

layout(location = 0) in vec2 aVertex;
layout(location = 1) in vec3 iPosition;
layout(location = 2) in vec2 iSize;
layout(location = 3) in vec4 iColor;

uniform mat4 projection;
uniform mat4 view;

out vec4 RectColor;

void main()
{
    vec3 worldPos = iPosition + vec3(aVertex * iSize, 0.0);

    gl_Position = projection
                * view
                * vec4(worldPos, 1.0);

    RectColor = iColor;
}
