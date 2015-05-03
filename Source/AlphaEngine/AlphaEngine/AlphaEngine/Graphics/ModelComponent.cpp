#include "ModelComponent.h"
//========================================================================
// IGameObjectComponent Functions
//========================================================================
ModelComponent::ModelComponent(string modelFileName) :
SceneNode(),
m_modelFileName(modelFileName)
{
}
// -----------------------------------------------------------------------
ModelComponent::~ModelComponent()
{

}
// -----------------------------------------------------------------------
ComponentType ModelComponent::VGetType()
{
	return 1;
}
// -----------------------------------------------------------------------
void ModelComponent::VUpdate()
{

}
// -----------------------------------------------------------------------
bool ModelComponent::VInit(TiXmlElement* pElement)
{
	return true;
}
// -----------------------------------------------------------------------
bool ModelComponent::VPostInit()
{
	return true;
}
//========================================================================
// Scene Node Functions
//========================================================================
void ModelComponent::VRender()
{
	VRenderChildren();
}
bool ModelComponent::VLoad()
{
	return true;
}

//========================================================================
// Component Creator Functions
//========================================================================
IGameObjectComponent* ModelComponentCreator::CreateComponent(TiXmlElement* pElement)
{
	string modelFileName = pElement->Attribute("ModelFileName ");
	ModelComponent* modelComponent = ALPHA_NEW ModelComponent(modelFileName);
	modelComponent->VInit(pElement);
	return modelComponent;
}