#pragma once
#include "LightNode.h"

typedef list <StrongLightPtr> LightArray;
typedef list <LightNode*> WeakLightArray;
//========================================================================
class LightManager
{
	const int MAX_SUNS			= 1;
public:
	LightManager();
	~LightManager();
	void AddLight(StrongLightPtr light);
	void RemoveLight(LightNode* light);
	void GetLights(WeakLightArray& lights) const { lights = m_usedLights; };
	void SortLights();
private:
	//all lights in scene
	LightArray		m_sunLights;
	LightArray		m_pointAndSpotLights;
	//lights that will be used in shaders
	WeakLightArray	m_usedLights;

};
//========================================================================