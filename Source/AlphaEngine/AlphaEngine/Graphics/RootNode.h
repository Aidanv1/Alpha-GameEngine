#ifndef ROOTNODE_H
#define ROOTNODE_H
#include "SceneNode.h"
class RootNode : public SceneNode
{
public:
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	virtual void VRender(Scene* pScene) override;
};

#endif