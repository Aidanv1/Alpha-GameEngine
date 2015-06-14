#pragma once
#include "../IActorComponent.h"
#include "../../Graphics3D/Scene/SceneNode.h"
class GraphicsComponent : public IActorComponent
{
public:
	void SetSceneNode(StrongSceneNodePtr sceneNode);
	ISceneNode* GetSceneNode() { return m_sceneNode.get(); }
	//IActorComponent fucnctions
	virtual ComponentType VGetType() const override{ return "Graphics"; }
	virtual bool VPostInit() override;
	virtual void VUpdate(float deltaMs) override;
	virtual bool VInitComponent(TiXmlElement* pElement) override;	
private:
	StrongSceneNodePtr m_sceneNode;
};

