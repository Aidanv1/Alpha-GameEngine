#ifndef IDRAWABLE_H
#define IDRAWABLE_H
#include "SceneNode.h"
class SceneNode;
//========================================================================
class IDrawable
{
public:
	virtual bool VInit(SceneNode* pSceneNode) = 0;
	virtual void VDraw() = 0;
	virtual bool VLoad() = 0;
};
//========================================================================
#endif