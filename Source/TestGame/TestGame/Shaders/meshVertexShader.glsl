#version 400
//========================================================================
//Constants
const int MAX_BONES = 10;
//========================================================================
in vec4 i_Position;	// Vertex in local coordinate system
in vec4 i_Normal;		// Normal of this vertex
in vec2 i_TexCoord;	// Coming FROM the OpenGL program
in float i_boneWeights[MAX_BONES]; //bone weights

out vec2 v_texCoord;		// Going TO the fragment shader
out vec3 v_fN;			// Because these are "out", they go to the fragment shader
out vec3 v_fE;			// The vector between the camera and a pixel
out vec3 v_Position;
out vec3 v_PositionEye;

uniform mat4 u_M;		// Model Matrix
uniform mat4 u_V;		// View Matrix
uniform mat4 u_P;		// Perspective Matrix
uniform int u_numBones;
uniform mat4 u_BoneOffsets[MAX_BONES];
//========================================================================
void main () 
{
	mat4 Rotation = u_M;
	Rotation[3][0] = 0.0;
	Rotation[3][1] = 0.0;
	Rotation[3][2] = 0.0;
	v_fN = (Rotation*i_Normal).xyz;				// Rotate the normal	
	v_fE = (u_V * u_M * i_Position).xyz;	// where the pixel is relative to the camera. 	

	v_texCoord = i_TexCoord;
	v_Position = (u_M * i_Position).xyz;
	v_PositionEye = (u_V * u_M *  i_Position).xyz;

	vec4 finalPosition = i_Position;
	vec4 normalTemp = vec4(v_fN, 1.0);
	for(int i = 1; i<u_numBones; i++)	
	{		
		mat4 boneRotation = u_BoneOffsets[i];
		boneRotation[3][0] = 0.0;
		boneRotation[3][1] = 0.0;
		boneRotation[3][2] = 0.0;
		//apply bone offset according to weight
		normalTemp = normalTemp + i_boneWeights[i] * boneRotation * vec4(v_fN, 1.0);
		finalPosition =	finalPosition + i_boneWeights[i] * u_BoneOffsets[i] * i_Position;
	}
	v_fN = normalTemp.xyz;
	finalPosition = u_P * u_V * u_M * finalPosition;
	gl_Position = finalPosition;
}