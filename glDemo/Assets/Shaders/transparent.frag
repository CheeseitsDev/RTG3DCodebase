#version 330 core

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);

    // If the alpha value is too low (near 0), we discard the fragment
    if (texColor.a < 0.1)
        discard;

    FragColor = texColor; // Pass the color with alpha value intact
}
