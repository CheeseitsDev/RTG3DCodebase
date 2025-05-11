#version 330 core

in vec2 TexCoords;

uniform sampler2D texture_diffuse1; // Or just 'texture1' depending on your setup

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);

    // Discard fully transparent pixels to prevent writing to framebuffer
    if (texColor.a < 0.1)
        discard;

    FragColor = texColor;
}
