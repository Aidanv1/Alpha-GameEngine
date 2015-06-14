attribute vec4 a_Position;
attribute vec4 a_Normal;
attribute vec2 a_TexCoord;	
uniform mat4 mM;		// Model Matrix
uniform mat4 mV;		// View Matrix
uniform mat4 mP;		// Perspective Matrix
varying vec2 texCoord;
varying vec3 v_Color;
uniform vec4 vLight;

varying vec3 fN;
varying vec3 fL;
varying vec3 fE;	
varying vec3 v_Position;
varying vec3 v_PositionEye;
void main()                    
{	
	//get rotation only
	mat4 Rotation = mM;
	Rotation[3][0] = 0.0;
	Rotation[3][1] = 0.0;
	Rotation[3][2] = 0.0;
	fN = (Rotation*a_Normal).xyz;		// Rotate the normal
	fL = (vLight).xyz;					// Rotate light direction
	fE = (mV * mM * a_Position).xyz;	// where the pixel is relative to the camera. 
	texCoord = a_TexCoord;
	v_Position = (mM * a_Position).xyz;
   	v_PositionEye = (mV * mM *  a_Position).xyz;
    gl_Position = mP * mV * mM *  a_Position;    
}