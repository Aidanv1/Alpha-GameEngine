#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H
#include "../GameObject/GameObject.h" 
#include "SceneNode.h"
#include "../GameObject/IGameObjectComponent.h"
#include "../GameObject/IComponentCreator.h"
//Model Component
//========================================================================
class ModelComponent :public SceneNode, public IGameObjectComponent
{
public:
	ModelComponent(string modelFileName);
	~ModelComponent();
	//IGameObjectComponent functions
	virtual ComponentType VGetType() override;
	virtual void VUpdate() override;
	virtual bool VInit(TiXmlElement* pElement) override;
	virtual bool VPostInit() override;
	//SceneNode functions
	virtual void VRender() override;
	virtual bool VLoad() override;
private:
	string m_modelFileName;
};
//========================================================================
//Model Component Creator
//========================================================================
class ModelComponentCreator : public IComponentCreator
{
	virtual IGameObjectComponent* CreateComponent(TiXmlElement* pElement) override;
};
//========================================================================
#endif