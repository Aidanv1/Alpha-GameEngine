#version 130

in vec4 s_vPosition;	// Vertex in local coordinate system
in vec4 s_vNormal;		// Normal of this vertex


in vec2 s_vTexCoord;	// Coming FROM the OpenGL program
out vec2 texCoord;		// Going TO the fragment shader

uniform mat4 mM;		// Model Matrix
uniform mat4 mV;		// View Matrix
uniform mat4 mP;		// Perspective Matrix
uniform mat4 mRotations;


uniform vec4 vLight;	// The Light position

out vec3 fN;			// Because these are "out", they go to the fragment shader
out vec3 fL;			// as interpolated values (i.e. the pixel will have an interpolated normal)

out vec3 fE;			// The vector between the camera and a pixel

void main () {
	
	fN = (mRotations*s_vNormal).xyz;				// Rotate the normal
	fL = (vLight).xyz;					// Rotate light direction
	fE = (mV * mM * s_vPosition).xyz;	// where the pixel is relative to the camera. 
	
	// NEW!! Just pass off the UVs to the fragment shader
	texCoord = s_vTexCoord;

	// New way using matrix multiplication.  From local, to world, to camera, to NDCs
	gl_Position = mP * mV * mM * s_vPosition;


}