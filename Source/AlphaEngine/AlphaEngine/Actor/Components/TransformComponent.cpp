#include "../../AlphaStd.h"
#include "TransformComponent.h"
#include "../../Maths/LinearAlgebra.h"
// -----------------------------------------------------------------------
TransformComponent::TransformComponent()
{

}
// -----------------------------------------------------------------------
TransformComponent::~TransformComponent()
{

}
// -----------------------------------------------------------------------
void TransformComponent::SetTransform(Matrix4x4& transform)
{
	m_transform = transform;
}
// -----------------------------------------------------------------------
void TransformComponent::SetPosition(vec3 position)
{
	m_transform.SetPosition(position);
}
// -----------------------------------------------------------------------
vec3 TransformComponent::GetPosition() const
{
	return m_transform.GetPosition();
}
// -----------------------------------------------------------------------
void TransformComponent::SetRotation(vec3 rotation)
{
	m_transform.SetRotation(rotation);
}
// -----------------------------------------------------------------------
Matrix4x4 TransformComponent::GetRotation() const
{
	return m_transform.GetRotation();
}
// -----------------------------------------------------------------------
vec3 TransformComponent::GetLookAt() const
{
	return m_transform.GetLookAtVector();
}
// -----------------------------------------------------------------------
//IActorComponent functions
// -----------------------------------------------------------------------
void TransformComponent::VUpdate(float deltaMs)
{
}
// -----------------------------------------------------------------------
bool TransformComponent::VInitComponent(TiXmlElement* pElement)
{
	pElement = pElement->FirstChildElement();
	//loop through elements
	while (pElement)
	{
		string val = pElement->Value();
		if (val == "Position")
		{
			vec3 position;
			pElement->QueryFloatAttribute("x", &position.x);
			pElement->QueryFloatAttribute("y", &position.y);
			pElement->QueryFloatAttribute("z", &position.z);
			SetPosition(position);
		}
		if (val == "RotationInDegrees")
		{
			vec3 rotation;
			pElement->QueryFloatAttribute("xAxis", &rotation.x);
			pElement->QueryFloatAttribute("yAxis", &rotation.y);
			pElement->QueryFloatAttribute("zAxis", &rotation.z);
			//convert to radians			
			rotation.x = radians<float>(rotation.x);
			rotation.y = radians<float>(rotation.y);
			rotation.z = radians<float>(rotation.z);
			SetRotation(rotation);
		}
		pElement = pElement->NextSiblingElement();
	}
	return true;
}
// -----------------------------------------------------------------------
bool TransformComponent::VPostInit()
{
	return true;
}
// -----------------------------------------------------------------------

