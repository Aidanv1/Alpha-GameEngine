#pragma once
#include "../Maths/GameMaths.h"
//========================================================================
//Forward declarations
class Scene;
class TiXmlElement;
enum DepthBufferCommand
{
	DepthBufferDisabled,
	DepthBufferEnabled,
	DepthBufferLess,
	DepthBufferEqual,
	DepthBufferAlways
};
//========================================================================
class IRenderer
{
public:
	virtual bool VInit(TiXmlElement* pElement) = 0;
	virtual void VRender(Scene* scene) = 0;
	virtual void VOnRestore() = 0;
	virtual void VSetBackGroundColour(vec4& backGroundColour) = 0;
	virtual void VDepthBuffer(DepthBufferCommand depthMode) = 0;
	virtual void VDrawLine(vec3& from, vec3& to, vec4& colour) = 0;
	virtual void VDrawPoint(vec3& point, float size, vec4& colour) = 0;
	virtual void VDrawText(const char* text) = 0;
};
//========================================================================
