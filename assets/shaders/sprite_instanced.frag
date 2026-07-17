#version 330 core

in vec2 TexCoord;
in vec4 SpriteColor;

out vec4 FragColor;

uniform sampler2D spriteTex;

void main()
{
    vec4 texColor = texture(spriteTex, TexCoord);
    FragColor = texColor * SpriteColor;

    if (FragColor.a < 0.01)
        discard;
}
