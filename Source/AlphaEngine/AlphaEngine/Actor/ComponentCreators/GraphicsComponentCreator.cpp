#include "GraphicsComponentCreator.h"
IActorComponent* GraphicsComponentCreator::CreateComponent(TiXmlElement* pElement)
{
	TiXmlElement* graphicsElement = pElement;
	string type;
	type = graphicsElement->Attribute("type");
	if (type == "Model")
	{
		Model* component = ALPHA_NEW Model();
		//add to scene
		GraphicsSystem::Get().GetScene()->AddChild(shared_ptr<SceneNode>(component));
		return component;
	}
	if (type == "Sky")
	{
		SkyBox* component = ALPHA_NEW SkyBox();
		//add to scene
		GraphicsSystem::Get().GetScene()->AddChild(shared_ptr<SceneNode>(component));
		return component;
	}

	if (type == "Camera")
	{
		Camera* component = ALPHA_NEW Camera();
		//add to scene
		GraphicsSystem::Get().GetScene()->AddChild(shared_ptr<SceneNode>(component));
		GraphicsSystem::Get().GetScene()->SetCameraNode(shared_ptr<Camera>(component));
		return component;
	}
	if (type == "HeightMap")
	{
		HeightMap* component = ALPHA_NEW HeightMap();
		//add to scene
		GraphicsSystem::Get().GetScene()->AddChild(shared_ptr<SceneNode>(component));
		return component;
	}
	graphicsElement = graphicsElement->NextSiblingElement();

	return NULL;
}