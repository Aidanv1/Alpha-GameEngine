#include "Scene.h"
#include "../IRenderer.h"
// -----------------------------------------------------------------------
Scene::Scene() :
m_rootNode(NULL),
m_isAlphaPass(false)
{

}
// -----------------------------------------------------------------------
Scene::~Scene()
{

}
// -----------------------------------------------------------------------
bool Scene::Init(IRenderer* renderer)
{
	m_renderer = renderer;
	m_rootNode = StrongRootNodePtr(ALPHA_NEW RootNode());
	return true;
}
// -----------------------------------------------------------------------
void Scene::SetCameraNode(StrongCameraNodePtr cameraNode)
{
	m_cameraNode = cameraNode;	
}

// -----------------------------------------------------------------------
void Scene::Update(float deltaMs)
{
	m_rootNode->VUpdateNode(this, deltaMs);
}

// -----------------------------------------------------------------------
void Scene::AddChild(StrongSceneNodePtr child)
{
	if (auto light = dynamic_pointer_cast<LightNode>(child))
	{
		AddLightToScene(light);
	}
	m_rootNode->VAddChild(child);
	
}
// -----------------------------------------------------------------------
void Scene::Render()
{
	//sort lights
	m_lightManager.SortLights();
	//render opaque objects
	m_rootNode->VRender(this);
	//sort transparent objects in ascending order of depth
	SortTransparentNodes();
	//render transparent objects
	m_isAlphaPass = true;
	RenderTransparentNodes();
	m_isAlphaPass = false;
	//clear list of transparent nodes
	m_transparentSceneNodes.clear();
}
// -----------------------------------------------------------------------
bool Scene::Load()
{	
	return m_rootNode->VInitNode();
}
// -----------------------------------------------------------------------
void Scene::AddTransparentNode(ISceneNode* node)
{
	//add to list
	m_transparentSceneNodes.push_back(node);
}
// -----------------------------------------------------------------------
void Scene::RenderTransparentNodes()
{	
	for (auto it = m_transparentSceneNodes.begin(); it != m_transparentSceneNodes.end(); it++)
	{
		(*it)->VRender(this);
	}	
}
// -----------------------------------------------------------------------
void Scene::SortTransparentNodes()
{
	m_transparentSceneNodes.sort([](ISceneNode* a, ISceneNode* b)
	{
		float aZ = a->VGetScreenZ();
		float bZ = b->VGetScreenZ();
		return aZ < bZ;
	});
}
// -----------------------------------------------------------------------
void Scene::AddLightToScene(StrongLightPtr light)
{
	m_lightManager.AddLight(light);
}
// -----------------------------------------------------------------------
void Scene::RemoveLighFromScene(LightNode* light)
{
	m_lightManager.RemoveLight(light);
}