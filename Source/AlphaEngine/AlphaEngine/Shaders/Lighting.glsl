//LIGHTING
//==============================================================
//Constants-----------------------------------------------------
const int MAX_LIGHTS = 10;
const int TYPE_SUN_LIGHT = 1;
const int TYPE_POINT_LIGHT = 2;
const int TYPE_SPOT_LIGHT = 3;
const float MAX_BRIGHTNESS_RATIO = 2.0;
//Global Uniforms-----------------------------------------------
struct FogInfo
{
	float	distanceMax;
	float	distanceMin;
	vec4	colourFog;
};

struct LightInfo
{
	vec3 directionVector;
	vec3 positionVector;
	vec3 intensity;
	float attenuation;
	vec3 ambient;
	float cosTheta;
	int type;
};

struct MaterialData
{
	float shininess;
	vec3 Ks;
	vec3 Kd;
	vec3 Ka;
	vec3 Ke;
};
//Global Light Array
uniform LightInfo lights[MAX_LIGHTS];
uniform FogInfo fog;
uniform MaterialData material;
//==============================================================	
//FUNCTIONS
//==============================================================
vec3 specularLight(vec3 N, vec3 E, vec3 Dir, int lightIndex)
{
	vec3 H = normalize(Dir + E);
	float spec = pow(max(dot(N, H), 0.0), material.shininess);
	return spec * material.Ks * lights[lightIndex].intensity;
}
//-------------------------------------------------------------
vec3 diffuseLight(vec3 N, vec3 Dir, int lightIndex)
{
	float diff = max(dot(N, Dir), 0.0);
	return diff * material.Kd * lights[lightIndex].intensity;
}
//-------------------------------------------------------------
vec3 ambientLight(int lightIndex)
{
	return material.Ka * lights[lightIndex].ambient;
}
//-------------------------------------------------------------
vec3 calcSunLighting(vec3 N, vec3 E, int lightIndex)
{
	return	specularLight(N, E, lights[lightIndex].directionVector, lightIndex) +
		diffuseLight(N, lights[lightIndex].directionVector, lightIndex) +
		ambientLight(lightIndex) +
		material.Ke;
}
//-------------------------------------------------------------
vec3 calcPointLighting(vec3 N, vec3 E, vec3 pos, int lightIndex)
{
	vec3 dirVector = normalize(pos - lights[lightIndex].positionVector);
	float distanceCoeff = min(lights[lightIndex].attenuation / (length(pos - lights[lightIndex].positionVector)), MAX_BRIGHTNESS_RATIO);
	return	distanceCoeff * (specularLight(N, E, dirVector, lightIndex) +
		diffuseLight(N, dirVector, lightIndex) +
		ambientLight(lightIndex)) +
		material.Ke;
}
//-------------------------------------------------------------
vec3 calcSpotLighting(vec3 N, vec3 E, vec3 pos, int lightIndex)
{
	vec3 dirVector = normalize(pos - lights[lightIndex].positionVector);
	//check if point lies in cone
	float spotTest = dot(dirVector, lights[lightIndex].directionVector);
	if (spotTest < lights[lightIndex].cosTheta)
	{
		return vec3(0);
	}
	float spotIntensity = spotTest - lights[lightIndex].cosTheta;

	float distanceCoeff = min(lights[lightIndex].attenuation / (length(pos - lights[lightIndex].positionVector)), MAX_BRIGHTNESS_RATIO);
	return	distanceCoeff * spotIntensity * (specularLight(N, E, dirVector, lightIndex) +
		diffuseLight(N, dirVector, lightIndex) +
		ambientLight(lightIndex)) +
		material.Ke;
}
//-------------------------------------------------------------
vec4 calcLighting(vec4 pixel, vec3 N, vec3 E, vec3 pos)
{
	vec3 colour = vec3(0);
	vec4 result;
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		switch (lights[i].type)
		{
		case TYPE_SUN_LIGHT:
			colour += calcSunLighting(-N, -E, i);
			break;
		case TYPE_POINT_LIGHT:
			colour += calcPointLighting(-N, -E, pos, i);
			break;
		case TYPE_SPOT_LIGHT:
			colour += calcSpotLighting(-N, -E, pos, i);
			break;
		}
	}
	result = vec4(colour.x*pixel.x, colour.y*pixel.y, colour.z*pixel.z, pixel.w);

	return result;
}
//-------------------------------------------------------------
vec4 applyFogEffect(vec4 colour, vec3 pos)
{
	float distance = length(pos);
	float fogFactor = (fog.distanceMax - distance) / (fog.distanceMax - fog.distanceMin);
	float clampedFactor = clamp(fogFactor, 0.0, 1.0);
	return mix(fog.colourFog, colour, clampedFactor);
}