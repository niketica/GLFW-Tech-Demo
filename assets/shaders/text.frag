#version 330 core

in vec2 vUV;
in vec4 vColor;

out vec4 FragColor;

uniform sampler2D uFontAtlas;

void main()
{
    float alpha = texture(uFontAtlas, vUV).r;
    FragColor = vec4(vColor.rgb, vColor.a * alpha);
}
