#include "SceneNode.h"
//========================================================================
class RootNode : public SceneNode
{
	friend class Scene;
public:
	~RootNode();
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	virtual void VRender(Scene* pScene) override;
	virtual bool VInitNode() override;
	virtual void VAddChild(StrongSceneNodePtr sceneNode);
private:
	RootNode();
};
//========================================================================
