#include "GraphicsComponent.h"
#include "../../Graphics3D/GraphicsSystem.h"
#include "../../Actor/Components/TransformComponent.h"
#include "../../Graphics3D/Scene/Scene.h"
#include "../../Graphics3D/Scene/SceneNode.h"
// -----------------------------------------------------------------------
GraphicsComponent::GraphicsComponent()
{

}
// -----------------------------------------------------------------------
GraphicsComponent::~GraphicsComponent()
{
	m_sceneNode->VDestroyNode();
}
// -----------------------------------------------------------------------
void GraphicsComponent::SetSceneNode(ISceneNode* sceneNode)
{
	ALPHA_ASSERT(sceneNode);
	m_sceneNode = StrongSceneNodePtr(sceneNode);
}
// -----------------------------------------------------------------------
bool GraphicsComponent::VPostInit()
{
	GraphicsSystem::Get().GetScene()->AddChild(m_sceneNode);
	return true;
}
// -----------------------------------------------------------------------
void GraphicsComponent::VUpdate(float deltaMs)
{
	//if the actor has a transform component, apply the transform to the 
	//graphics component
	TransformComponent* transformComponent = dynamic_cast<TransformComponent*>(m_pOwner->GetComponent("Transform"));
	if (transformComponent)
	{
		m_sceneNode->VSetTransform(transformComponent->GetTransform());
	}
}
// -----------------------------------------------------------------------
bool GraphicsComponent::VInitComponent(TiXmlElement* pElement)
{
	return m_sceneNode->VConfigureXmlNodeData(pElement);
}
// -----------------------------------------------------------------------