#include "GraphicsComponentCreator.h"
#include "../../Graphics3D/Scene/DrawableNode.h"
// -----------------------------------------------------------------------
IActorComponent* GraphicsComponentCreator::CreateComponent(TiXmlElement* pElement)
{
	ISceneNode* pSceneNode = NULL;
	TiXmlElement* graphicsElement = pElement;
	string type;
	DrawableNodeFactory nodeFactory;
	type = graphicsElement->Attribute("type");
	if (type == "Model")
	{
		pSceneNode = ALPHA_NEW ModelNode();
	}
	if (type == "Sky")
	{
		pSceneNode = nodeFactory.CreateDrawableNode(Node_Sky);
	}

	if (type == "Camera")
	{
		CameraNode* cameraNode = ALPHA_NEW CameraNode();
		ALPHA_ASSERT(cameraNode);
		GraphicsSystem::Get().GetScene()->SetCameraNode(shared_ptr<CameraNode>(cameraNode));
		pSceneNode = cameraNode;

	}	
	if (type == "Light")
	{
		LightNode* light = ALPHA_NEW LightNode();
		pSceneNode = light;

	}
	GraphicsComponent* component = ALPHA_NEW GraphicsComponent();
	component->SetSceneNode(StrongSceneNodePtr(pSceneNode));
	return component;
}
// -----------------------------------------------------------------------