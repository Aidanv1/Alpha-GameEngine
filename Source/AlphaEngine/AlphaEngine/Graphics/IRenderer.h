#ifndef IRENDERER_H
#define IRENDERER_H
#include "Scene.h"
#include "..\AlphaStd.h"
//========================================================================
class IRenderer
{
public:
	virtual bool VInit(StrongScenePtr pScene, TiXmlElement* pElement) = 0;
	virtual void VRender() = 0;
	virtual void VOnRestore() = 0;
	virtual void VSetBackGroundColour(vec4& backGroundColour) = 0;
	virtual void VPrintText(string text) = 0;
};
//========================================================================
#endif