#version 450 core

layout(binding = 0) uniform sampler2D ColTexture0; //these are where the four material textures would be mapped to
layout(binding = 1) uniform sampler2D ColTexture1;
layout(binding = 2) uniform sampler2D ColTexture2;
layout(binding = 3) uniform sampler2D ColTexture3;
layout(binding = 4) uniform samplerCube cubemapTexture0;//these are where the four global cubemaps would be mapped to
layout(binding = 5) uniform samplerCube cubemapTexture1;
layout(binding = 6) uniform samplerCube cubemapTexture2;
layout(binding = 7) uniform samplerCube cubemapTexture3;// 8 and above is for you to play with

in vec2 TexCoord; //we are picking these up from the vertex shader
in vec3 worldPos;
in vec3 normal;

uniform vec3 bluelightPos; //pull in the data for the light called blue
uniform vec3 bluelightCol;
uniform vec3 bluelightAmb;

out vec4 FragColor; // Color that will be used for the fragment

/////////////////////////////////////////// ///////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
//calculate the light vector that conects this point to the light in world space
	vec3 lv = bluelightPos - worldPos; 
	float d = length(lv); //distance
	lv = normalize(lv);

	vec3 n = normalize(normal);//should be close to normal but just in case
	
	float Intensity = clamp(dot(n,lv), 0.0, 1.0);

	FragColor = (vec4(bluelightAmb,1.0) + Intensity * vec4(bluelightCol,1.0)) * texture(ColTexture0, TexCoord.xy);
}