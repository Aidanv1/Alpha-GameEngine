#version 400
#include <Lighting.glsl>
in vec3 v_fN;
in vec3 v_fE;
in vec2 v_texCoord;
in vec3 v_Position;				
uniform sampler2D texture;
in vec3 v_PositionEye;
uniform int hasTexture;
out vec4 fragColour;
void main () 
{
	vec4 texel;
	//if there is no texture, use default colour (red)
	if(hasTexture == 1.0)
	{
		texel = texture2D(texture, v_texCoord);
	}
	else
	{
		texel = vec4(1.0,0.0,0.0,1.0);
	}
	//if the pixel is completely transparent, discard it
	if(texel.w < 0.1)
	{		
		discard;	
	}
	vec3 N = normalize(v_fN);		// Noramlize N
	vec3 E = normalize(-v_fE);	// Reverse (E)ye vector (or view vector)
	vec4 lightingColour = calcLighting(texel, N, E, v_Position);
	fragColour = applyFogEffect(lightingColour, v_PositionEye);
}
