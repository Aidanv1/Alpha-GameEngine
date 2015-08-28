attribute vec2 s_vPosition;	// Vertex in local coordinate system
attribute vec2 s_vTexCoord;	// Coming FROM the OpenGL program
varying vec2 texCoord;		// Going TO the fragment shader

void main () 
{	
	texCoord = s_vTexCoord;
	gl_Position = vec4(s_vPosition,0,1);
}