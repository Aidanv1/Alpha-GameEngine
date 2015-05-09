#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H
#include "../GameObject/GameObject.h" 
#include "SceneNode.h"
#include "../GameObject/IGameObjectComponent.h"
#include "../GameObject/IComponentCreator.h"
#include "../ResourceManager/Resource.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "OpenGL/Data/Mesh.h"
 
typedef shared_ptr<Resource> StrongModelPtr;
//Model Component
//========================================================================
class ModelComponent :public SceneNode, public IGameObjectComponent
{
public:
	ModelComponent(string modelFileName, vec3 pos, vec3 rot);
	~ModelComponent();
	//IGameObjectComponent functions
	virtual ComponentType VGetType() override;
	virtual void VUpdate() override;
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override;
	virtual bool VInit(TiXmlElement* pElement) override;
	virtual bool VPostInit() override;
	//SceneNode functions
	virtual void VRender(Scene* pScene) override;
	virtual bool Load();
	bool LoadResource();
	//16 byte alignment
	//void* operator new(std::size_t sz)
	//{
	//	return _aligned_malloc(sz, 16);
	//}
private:
	int m_modelID;	
	string m_modelFileName;
	StrongResourceManagerPtr m_modelResourceManager;
	StrongModelPtr m_modelResource;
};
//========================================================================
//Model Component Creator
//========================================================================
class ModelComponentCreator : public IComponentCreator
{
public:
	virtual IGameObjectComponent* CreateComponent(TiXmlElement* pElement) override;
};
//========================================================================
#endif