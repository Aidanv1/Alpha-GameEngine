attribute vec4 a_Position;	// Vertex in local coordinate system
uniform mat4 mM;		// Model Matrix
uniform mat4 mV;		// View Matrix
uniform mat4 mP;		// Perspective Matrix

uniform vec4 u_Colour;
varying vec4 v_Colour;
void main () 
{	
	v_Colour = u_Colour;
	gl_Position = mP * mV * mM * a_Position;	
	gl_PointSize = 10.0;
}