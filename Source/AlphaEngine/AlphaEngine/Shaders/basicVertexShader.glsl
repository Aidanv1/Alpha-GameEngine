#version 130

attribute vec4 a_Position;	// Vertex in local coordinate system
uniform mat4 mM;		// Model Matrix
uniform mat4 mV;		// View Matrix
uniform mat4 mP;		// Perspective Matrix

void main () 
{	
	gl_Position = mP * mV * mM * a_Position;	
	gl_PointSize = 10.0;
}