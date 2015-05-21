#include "GraphicsComponent.h"
#include "../../Graphics3D/GraphicsSystem.h"
bool GraphicsComponent::VPostInit()
{
	SceneNode* node = dynamic_cast<SceneNode*>(this);
	GraphicsSystem::Get().GetScene()->AddChild(shared_ptr<SceneNode>(node));
	return true;
}