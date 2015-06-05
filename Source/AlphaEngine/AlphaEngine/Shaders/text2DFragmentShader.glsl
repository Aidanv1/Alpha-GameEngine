varying vec2 texCoord;				
uniform sampler2D texture;

void main () 
{
	vec4 color = vec4(texture2D(texture, texCoord));	
	gl_FragColor  = color;	
}
