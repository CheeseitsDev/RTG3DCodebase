#version 330 core

in vec3 FragPos;   // Fragment position in world space
in vec3 Normal;    // Normal at the fragment
in vec3 LightPos;  // Light position

out vec4 FragColor; // Final color of the fragment

uniform vec3 lightColour;    // Light color (set in C++)
uniform vec3 objectColour;   // Object color (set in C++)
uniform vec3 viewPos;       // Camera position (set in C++)

void main()
{
    // Normalize the normal and light direction vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Diffuse reflection (Lambert's cosine law)
    float diff = max(dot(norm, lightDir), 0.0);

    // Specular reflection (Blinn-Phong)
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Shininess exponent

    // Lighting components
    vec3 ambient = 0.1 * lightColour;  // Ambient light
    vec3 diffuse = diff * lightColour; // Diffuse light
    vec3 specular = spec * lightColour; // Specular light

    // Final color calculation
    vec3 result = (ambient + diffuse + specular) * objectColour;

    FragColor = vec4(result, 1.0);
}
