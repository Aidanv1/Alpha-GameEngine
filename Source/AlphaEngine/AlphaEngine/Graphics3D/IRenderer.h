#pragma once
#include "Scene\Scene.h"
#include "..\AlphaStd.h"
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
	virtual void VRender(StrongScenePtr scene) = 0;
	virtual void VOnRestore() = 0;
	virtual void VSetBackGroundColour(vec4& backGroundColour) = 0;
	virtual void VPrintText(string text) = 0;
	virtual void VDepthBuffer(DepthBufferCommand depthMode) = 0;
};
//========================================================================
