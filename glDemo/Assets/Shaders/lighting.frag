#version 330 core

in vec3 FragPos;   // Fragment position in world space
in vec3 Normal;    // Normal at the fragment
in vec3 LightPos;  // Light position

out vec4 FragColor; // Final color of the fragment

uniform vec3 lightColor;    // Light color
uniform vec3 objectColor;   // Object color
uniform vec3 viewPos;       // Camera position

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
    vec3 ambient = 0.1 * lightColor;  // Ambient light
    vec3 diffuse = diff * lightColor; // Diffuse light
    vec3 specular = spec * lightColor; // Specular light

    // Final color calculation
    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}