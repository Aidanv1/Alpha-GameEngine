#include "Scene.h"
// -----------------------------------------------------------------------
Scene::Scene() :
m_rootNode(NULL)
{

}
// -----------------------------------------------------------------------
Scene::~Scene()
{

}
// -----------------------------------------------------------------------
bool Scene::Init()
{
	m_rootNode = StrongRootNodePtr(new RootNode());
	return true;
}
// -----------------------------------------------------------------------
void Scene::SetCameraNode(StrongCameraNodePtr cameraNode)
{
	m_cameraNode = cameraNode;
	m_rootNode->VAddChild(cameraNode);
}

// -----------------------------------------------------------------------
void Scene::Update(float deltaMs)
{
	m_rootNode->VUpdateNode(this, deltaMs);
}

// -----------------------------------------------------------------------
void Scene::AddChild(StrongNodePtr child)
{
	m_rootNode->VAddChild(child);
}
// -----------------------------------------------------------------------
void Scene::Render()
{
	m_rootNode->VRender(this);
}
// -----------------------------------------------------------------------