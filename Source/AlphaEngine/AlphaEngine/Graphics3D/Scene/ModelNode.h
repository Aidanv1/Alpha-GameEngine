#pragma once
#include "../../Actor/Actor.h" 
#include "SceneNode.h"
#include "../GraphicsSystem.h"
#include "../../Actor/Components/GraphicsComponent.h"
#include "../../Actor/IComponentCreator.h"
#include "../../ResourceManager/Resource.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "DrawableNode.h"
class ModelNode;
typedef shared_ptr<Resource> StrongModelPtr;
//Model Component
//========================================================================
class ModelNode :public SceneNode
{
public:
	ModelNode();
	~ModelNode();
	//SceneNode functions
	virtual bool VConfigureXmlNodeData(TiXmlElement* pElement) override;
	virtual bool VInitNode() override;
	virtual void VRender(Scene* pScene) override;
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	//ModelComponent functions
	string GetModelFileName() { return m_modelFileName; }
	virtual bool Load();
	bool RequestLoadResource();	
private:
	virtual void VRenderChildren(Scene* pScene) override;
	bool ValidateBuffers();
private:
	int m_modelID;	
	string m_modelFileName;
	StrongResourceManagerPtr m_modelResourceManager;
	StrongModelPtr m_modelResource;
	SceneNodeList m_meshChildren;
};
