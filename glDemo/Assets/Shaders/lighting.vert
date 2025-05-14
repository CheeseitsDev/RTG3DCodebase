#version 330 core

layout(location = 0) in vec3 aPos;    // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal

out vec3 FragPos;   // Position in world space
out vec3 Normal;    // Normal at the fragment
out vec3 LightPos;  // Light position in world space

uniform mat4 model;       // Model transformation matrix
uniform mat4 view;        // View transformation matrix
uniform mat4 projection;  // Projection matrix
uniform vec3 lightPos;    // Position of the light source

void main()
{
    // Calculate the fragment's position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform normal to world space
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pass light position
    LightPos = lightPos;

    // Calculate the final vertex position in camera space (projection -> view -> model)
    gl_Position = projection * view * vec4(FragPos, 1.0);
}