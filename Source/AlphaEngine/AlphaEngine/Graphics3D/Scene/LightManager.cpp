#include "LightManager.h"
#include "../GraphicsSettings.h"
// -----------------------------------------------------------------------
LightManager::LightManager()
{
}
// -----------------------------------------------------------------------
LightManager::~LightManager()
{

}
// -----------------------------------------------------------------------
void LightManager::AddLight(StrongLightPtr light)
{
	LightNodeProperties lightProp ;
	light->GetLightProperties(lightProp);
	switch (lightProp.m_lightType)
	{
	case LightType_Sun:
		m_sunLights.push_back(light);
		break;
	case LightType_Point:
		m_pointAndSpotLights.push_back(light);
		break;
	case LightType_SpotLight:
		m_pointAndSpotLights.push_back(light);
		break;
	}
}
// -----------------------------------------------------------------------
void LightManager::RemoveLight(LightNode* light)
{
	LightNodeProperties lightProp;
	light->GetLightProperties(lightProp);
	LightType type = lightProp.m_lightType;

	LightArray* lightArray = NULL;
	switch (lightProp.m_lightType)
	{
	case LightType_Sun:
		*lightArray = m_sunLights;
		break;
	case LightType_Point:
		*lightArray = m_pointAndSpotLights;
		break;
	case LightType_SpotLight:
		*lightArray = m_pointAndSpotLights;
		break;
	}
	//remove from appropriate list
	for (auto it = lightArray->begin(); it != lightArray->end(); it++)
	{
		if ((*it)->GetID() == light->GetID())
		{
			lightArray->erase(it);
		}
	}
}
// -----------------------------------------------------------------------
void LightManager::SortLights()
{
	int maxLights = GraphicsSettings::MaxLights();
	//sun lights are not sorted
	m_pointAndSpotLights.sort([](StrongLightPtr a, StrongLightPtr b)
	{
		float aDist = a->GetDistanceToCamera();
		float bDist = b->GetDistanceToCamera();
		return aDist < bDist;
	});
		
	m_usedLights.clear();
	int numSuns = MAX_SUNS;
	if (m_sunLights.size() != 0)
	{
		m_usedLights.push_back(m_sunLights.front().get());
	}
	int maxPointsAndSpots = maxLights - numSuns;
	//point and spot lights

	int numPointandSpotLights = glm::min((int)m_pointAndSpotLights.size(), maxPointsAndSpots);
	int count = 0;
	auto it = m_pointAndSpotLights.begin();
	while (it != m_pointAndSpotLights.end() &&
		count++ != numPointandSpotLights)
	{
		m_usedLights.push_back((*it).get());
		it++;
	}
}
// -----------------------------------------------------------------------