attribute vec4 a_Position;	// Vertex in local coordinate system
attribute vec4 a_Normal;		// Normal of this vertex


attribute vec2 a_TexCoord;	// Coming FROM the OpenGL program
varying vec2 texCoord;		// Going TO the fragment shader

uniform mat4 mM;		// Model Matrix
uniform mat4 mV;		// View Matrix
uniform mat4 mP;		// Perspective Matrix
uniform mat4 mRotations;
varying vec3 fN;			// Because these are "out", they go to the fragment shader
varying vec3 fE;			// The vector between the camera and a pixel
varying vec3 v_Position;
varying vec3 v_PositionEye;
void main () 
{
	
	fN = (mRotations*a_Normal).xyz;				// Rotate the normal	
	fE = (mV * mM * a_Position).xyz;	// where the pixel is relative to the camera. 	

	texCoord = a_TexCoord;
	v_Position = (mM * a_Position).xyz;
	v_PositionEye = (mV * mM *  a_Position).xyz;
	// New way using matrix multiplication.  From local, to world, to camera, to NDCs
	gl_Position = mP * mV * mM * a_Position;


}