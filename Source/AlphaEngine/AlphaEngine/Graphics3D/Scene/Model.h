#ifndef MODEL_H
#define MODEL_H
#include "../../Actor/Actor.h" 
#include "SceneNode.h"
#include "../../Actor/IActorComponent.h"
#include "../../Actor/IComponentCreator.h"
#include "../../ResourceManager/Resource.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "../OpenGL/Data/Mesh.h"
 
class Model;
typedef shared_ptr<Resource> StrongModelPtr;
//Model Component
//========================================================================
class Model :public SceneNode, public IActorComponent
{
public:
	Model();
	~Model();
	//IActorComponent functions
	virtual ComponentType VGetType() override;
	virtual void VUpdate() override;
	virtual bool VInitComponent(TiXmlElement* pElement) override;
	virtual bool VPostInit() override;
	//SceneNode functions
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


#endif