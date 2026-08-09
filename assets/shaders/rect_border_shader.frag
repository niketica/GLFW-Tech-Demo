#version 330 core

in vec2 RectSize;
in vec2 RectUV;
in vec4 FillColor;
in vec4 BorderColor;
in float BorderThickness;
in float FillEnabled;

out vec4 FragColor;

void main()
{
    // Convert the requested border thickness from pixels into UV coordinates.
    vec2 borderUV = BorderThickness / RectSize;

    bool isBorder =
        RectUV.x < borderUV.x ||
        RectUV.x > 1.0 - borderUV.x ||
        RectUV.y < borderUV.y ||
        RectUV.y > 1.0 - borderUV.y;

    if (isBorder)
    {
        FragColor = BorderColor;
    }
    else
    {
        if (FillEnabled > 0.5)
        {
            FragColor = FillColor;
        }
        else
        {
            discard;
        }
    }

    // FragColor = vec4(0.0, 0.0, 1.0, 1.0); debug color
}
