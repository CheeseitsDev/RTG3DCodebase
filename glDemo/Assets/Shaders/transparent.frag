#version 330 core

in vec2 TexCoords;

uniform sampler2D texture_diffuse;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(texture_diffuse, TexCoords);

    if (texColor.a < 0.1)
        discard;

    FragColor = texColor;
}
