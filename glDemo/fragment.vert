#version 450 core

layout (location = 0) in vec3               vertexPos_VS;	  	// Position in attribute location 0
layout (location = 1) in vec2               texCoord_VS;
layout (location = 2) in vec3               vertexNormal_VS;

uniform mat4          modelMatrix;
uniform mat4          viewMatrix;
uniform mat4          projMatrix;

out vec2 TexCoord;   //we are sending these over to the pixel shader
out vec3 worldPos;   //could make a custom struct to do it as well
out vec3 normal;

//swiped this from here https://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}


//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{    
    //twist around the y-axis based on how high up the y-axis we are
    vec3 newPos = mat3(rotationMatrix( vec3(0,1,0),vertexPos_VS.y)) * vertexPos_VS;
    vec3 newNorm = mat3(rotationMatrix( vec3(0,1,0),vertexPos_VS.y)) * vertexNormal_VS; //do the same to the normal as well

	//send over my texCoord
    TexCoord    = texCoord_VS;

	//calculate my world positionm
	worldPos = (modelMatrix * vec4(newPos, 1.0)).xyz;

	//rotate my normal to match my rotation
	normal = vec3(mat3(modelMatrix) * newNorm); //this should apply just the rotation and the scale 
	normal = normalize(normal);    

	//apply all my camera transforms
    gl_Position   = (projMatrix * viewMatrix * modelMatrix * vec4(newPos, 1.0));
}
