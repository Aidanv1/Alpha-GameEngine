#pragma once
#include "BaseShaderProgram.h"
#include "../../Scene/LightManager.h"
#include "../Data/Material_GL.h"
//========================================================================
struct LightUniformLocations
{
	int directionVectorID;
	int positionVectorID;
	int intensityID;
	int attenuationID;
	int ambientID;
	int thetaID;
	int typeID;
};
struct MaterialUniformLocations
{
	int shininessID;
	int KsID;
	int KdID;
	int KaID;
	int KeID;
};
struct FogUniformLocations
{
	int maxFogDistanceID;
	int minFogDistanceID;
	int	colourFogID;
};

class LightShaderProgram : public BaseShaderProgram
{
	//these constants need to be moved else where
	static const Colour FOG_COLOUR;

public:
	LightShaderProgram();
	~LightShaderProgram();
	virtual bool VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode) override;
	void SetLights(WeakLightArray lights);
	void SetMaterial(Material_GL* material);
	void SetLightUniform(int lightIndex, vec3 dir, vec3 pos, vec3 intensity, float attenuation, vec3 ambient, float theta, int type);
private:
	LightUniformLocations* m_lightLocations;
	MaterialUniformLocations m_materialLocation;
	FogUniformLocations m_fogUniformLocations;
};
//========================================================================