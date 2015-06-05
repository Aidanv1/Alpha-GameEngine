#include "GraphicsComponentCreator.h"
#include "../../Graphics3D/Scene/DrawableNode.h"
// -----------------------------------------------------------------------
IActorComponent* GraphicsComponentCreator::CreateComponent(TiXmlElement* pElement)
{
	IActorComponent* component;
	TiXmlElement* graphicsElement = pElement;
	string type;
	DrawableNodeFactory nodeFactory;
	type = graphicsElement->Attribute("type");
	if (type == "Model")
	{
		component = dynamic_cast<IActorComponent*>(ALPHA_NEW ModelNode());
		ALPHA_ASSERT(component);
		return component;
	}
	if (type == "Sky")
	{
		component = dynamic_cast<IActorComponent*>(nodeFactory.CreateDrawableNode(Node_Sky));
		ALPHA_ASSERT(component);
		return component;
	}

	if (type == "Camera")
	{
		CameraNode* cameraNode = ALPHA_NEW CameraNode();
		ALPHA_ASSERT(cameraNode);
		GraphicsSystem::Get().GetScene()->SetCameraNode(shared_ptr<CameraNode>(cameraNode));
		component = dynamic_cast<IActorComponent*>(cameraNode);
		ALPHA_ASSERT(component);
		return component;
	}
	if (type == "HeightMap")
	{
		component = dynamic_cast<IActorComponent*>(nodeFactory.CreateDrawableNode(Node_HeightMap));
		ALPHA_ASSERT(component);
		return component;		
	}
	if (type == "Light")
	{
		LightNode* light = ALPHA_NEW LightNode();
		component = dynamic_cast<IActorComponent*>(light);
		ALPHA_ASSERT(component);
		return component;
	}
	graphicsElement = graphicsElement->NextSiblingElement();

	return NULL;
}
// -----------------------------------------------------------------------