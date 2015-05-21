#ifndef ROOTNODE_H
#define ROOTNODE_H
#include "SceneNode.h"
//========================================================================
class RootNode : public SceneNode
{
public:
	RootNode();
	~RootNode();
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	virtual void VRender(Scene* pScene) override;
	virtual bool VInitNode() override;
	virtual void VAddChild(StrongSceneNodePtr sceneNode);
private:
	
};
//========================================================================
#endif