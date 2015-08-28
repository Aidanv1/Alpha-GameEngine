#pragma once
#include "../IActorComponent.h"
#include <memory>
class ISceneNode;
class TiXmlElement;
//========================================================================
class GraphicsComponent : public IActorComponent
{
public:
	void SetSceneNode(ISceneNode* sceneNode);
	ISceneNode* GetSceneNode() { return m_sceneNode.get(); }
	//IActorComponent fucnctions
	virtual ComponentType VGetType() const override{ return "Graphics"; }
	virtual bool VPostInit() override;
	virtual void VUpdate(float deltaMs) override;
	virtual bool VInitComponent(TiXmlElement* pElement) override;	
private:
	std::shared_ptr<ISceneNode> m_sceneNode;
};
//========================================================================
