#include <Lighting.glsl>
varying vec3 fN;
varying vec3 fE;
varying vec3 v_Position;
varying vec2 texCoord;				
uniform sampler2D texture;
varying vec3 v_PositionEye;
void main () 
{
	vec4 texel = texture2D(texture, texCoord);
	//if the pixel is completely transparent, discard it
	if(texel.w < 0.1)
	{		
		discard;	
	}
	vec3 N = normalize(fN);		// Noramlize N
	vec3 E = normalize(-fE);	// Reverse (E)ye vector (or view vector)
	vec4 lightingColour = calcLighting(texel, N, E, v_Position);
	gl_FragColor = applyFogEffect(lightingColour, v_PositionEye);
}