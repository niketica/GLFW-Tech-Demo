#version 330 core

layout(location = 0) in vec2 aVertex;
layout(location = 1) in vec3 iPosition;
layout(location = 2) in vec2 iSize;
layout(location = 3) in vec4 iFillColor;
layout(location = 4) in vec4 iBorderColor;
layout(location = 5) in float iBorderThickness;
layout(location = 6) in float iFillEnabled;

uniform mat4 projection;
uniform mat4 view;

out vec2 RectSize;
out vec2 RectUV;
out vec4 FillColor;
out vec4 BorderColor;
out float BorderThickness;
out float FillEnabled;

void main()
{
    vec3 worldPos =
        iPosition +
        vec3(aVertex * iSize, 0.0);

    gl_Position =
        projection *
        view *
        vec4(worldPos, 1.0);

    RectSize = iSize;
    RectUV = aVertex;

    FillColor = iFillColor;
    BorderColor = iBorderColor;
    BorderThickness = iBorderThickness;
    FillEnabled = iFillEnabled;
}
