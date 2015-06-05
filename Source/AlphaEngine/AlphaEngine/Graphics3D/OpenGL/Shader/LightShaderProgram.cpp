#include "LightShaderProgram.h"
#include "../../GraphicsSettings.h"
const Colour LightShaderProgram::FOG_COLOUR = Colour(0.2, 0.2, 0.2, 1.0);
// -----------------------------------------------------------------------
LightShaderProgram::LightShaderProgram()
{
}
// -----------------------------------------------------------------------
LightShaderProgram::~LightShaderProgram()
{

}
// -----------------------------------------------------------------------
bool LightShaderProgram::VInit(const char* vertexShaderSourceCode, const char* fragmentShaderSourceCode)
{
	if (!BaseShaderProgram::VInit(vertexShaderSourceCode, fragmentShaderSourceCode))
	{
		return false;
	}
	m_lightLocations = ALPHA_NEW LightUniformLocations[GraphicsSettings::MaxLights()];
	string lightArrayStr = "lights[";
	int maxLights = GraphicsSettings::MaxLights();
	for (int i = 0; i < maxLights; i++)
	{
		string str = "";
		str.append("lights[").append(to_string(i)).append( "].directionVector");
		
		m_lightLocations[i].directionVectorID = glGetUniformLocation(m_shaderProgramID, str.c_str());
		str = "";
		//
		str.append("lights[").append(to_string(i)).append("].positionVector");
		m_lightLocations[i].positionVectorID = glGetUniformLocation(m_shaderProgramID, str.c_str());
		str = "";
		//
		str.append("lights[").append(to_string(i)).append("].intensity");
		m_lightLocations[i].intensityID = glGetUniformLocation(m_shaderProgramID, str.c_str());
		str = "";
		//
		str.append("lights[").append(to_string(i)).append("].attenuation");
		m_lightLocations[i].attenuationID = glGetUniformLocation(m_shaderProgramID, str.c_str());
		str = "";
		//
		str.append("lights[").append(to_string(i)).append("].ambient");
		m_lightLocations[i].ambientID = glGetUniformLocation(m_shaderProgramID, str.c_str());
		str = "";
		//
		str.append("lights[").append(to_string(i)).append("].cosTheta");
		m_lightLocations[i].thetaID = glGetUniformLocation(m_shaderProgramID, str.c_str());
		str = "";
		//
		str.append("lights[").append(to_string(i)).append("].type");
		m_lightLocations[i].typeID = glGetUniformLocation(m_shaderProgramID, str.c_str());
		str = "";

		//check if all locations were found successfully
		if (m_lightLocations[i].directionVectorID == -1 ||
			m_lightLocations[i].positionVectorID == -1 ||
			m_lightLocations[i].intensityID == -1 ||
			m_lightLocations[i].attenuationID == -1 ||
			m_lightLocations[i].ambientID == -1 ||
			m_lightLocations[i].thetaID == -1 ||
			m_lightLocations[i].typeID == -1
			)
		{
			ALPHA_ASSERT(false);
			return false;
		}
	}

	m_materialLocation.shininessID = glGetUniformLocation(m_shaderProgramID, "material.shininess");
	m_materialLocation.KsID = glGetUniformLocation(m_shaderProgramID, "material.Ks");
	m_materialLocation.KdID = glGetUniformLocation(m_shaderProgramID, "material.Kd");
	m_materialLocation.KaID = glGetUniformLocation(m_shaderProgramID, "material.Ka");
	m_materialLocation.KeID = glGetUniformLocation(m_shaderProgramID, "material.Ke");

	m_fogUniformLocations.colourFogID = glGetUniformLocation(m_shaderProgramID, "fog.colourFog");
	m_fogUniformLocations.maxFogDistanceID = glGetUniformLocation(m_shaderProgramID, "fog.distanceMax");
	m_fogUniformLocations.minFogDistanceID = glGetUniformLocation(m_shaderProgramID, "fog.distanceMin");
	
	if (m_materialLocation.shininessID == -1 ||
		m_materialLocation.KsID == -1 ||
		m_materialLocation.KdID == -1 ||
		m_materialLocation.KaID == -1 ||
		m_materialLocation.KeID == -1 ||
		m_fogUniformLocations.colourFogID == -1 ||
		m_fogUniformLocations.maxFogDistanceID == -1 ||
		m_fogUniformLocations.minFogDistanceID == -1
		)		
	{
		ALPHA_ASSERT(false);
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------
void LightShaderProgram::SetLightUniform(int lightIndex, vec3 dir, vec3 pos, vec3 intensity, float attinuation, vec3 ambient, float theta, int type)
{
	glUniform3fv(m_lightLocations[lightIndex].directionVectorID, 1, value_ptr(dir));
	glUniform3fv(m_lightLocations[lightIndex].positionVectorID, 1, value_ptr(pos));
	glUniform3fv(m_lightLocations[lightIndex].intensityID, 1, value_ptr(intensity));
	glUniform1fv(m_lightLocations[lightIndex].attenuationID, 1, &attinuation);
	glUniform3fv(m_lightLocations[lightIndex].ambientID, 1, value_ptr(ambient));
	float cosTheta = cos(theta);
	glUniform1fv(m_lightLocations[lightIndex].thetaID, 1, &cosTheta);
	glUniform1iv(m_lightLocations[lightIndex].typeID,1, &type);
}
// -----------------------------------------------------------------------
void LightShaderProgram::SetLights(WeakLightArray lights)
{
	int i = 0;
	for (auto it = lights.begin(); it != lights.end(); it++, i++)
	{
		LightNodeProperties prop;
		(*it)->GetLightProperties(prop);
		SetLightUniform(		i, 
								prop.m_directionVector, 
								prop.m_positionVector, 
								prop.m_intensity, 
								prop.m_attinuation,
								prop.m_ambient,
								prop.m_theta,
								prop.m_lightType);
	}
	float fogFar = GraphicsSettings::FogFar();
	float fogClose = GraphicsSettings::FogClose();
	glUniform1fv(m_fogUniformLocations.maxFogDistanceID, 1, &fogFar);
	glUniform1fv(m_fogUniformLocations.minFogDistanceID, 1, &fogClose);
	glUniform4fv(m_fogUniformLocations.colourFogID, 1, value_ptr(FOG_COLOUR));

}
// -----------------------------------------------------------------------
void LightShaderProgram::SetMaterial(Material_GL* material)
{
	float shininess = 0;
	Colour Ks(0);
	Colour Kd = material->GetDiffuse();
	Colour Ka = material->GetAmbient();
	Colour Ke = material->GetEmissive();
	material->GetSpecular(Ks, shininess);
	glUniform1fv(m_materialLocation.shininessID, 1, &shininess);
	glUniform3fv(m_materialLocation.KsID, 1, value_ptr(Ks));
	glUniform3fv(m_materialLocation.KaID, 1, value_ptr(Ka));
	glUniform3fv(m_materialLocation.KdID, 1, value_ptr(Kd));
	glUniform3fv(m_materialLocation.KeID, 1, value_ptr(Ke));
}
// -----------------------------------------------------------------------