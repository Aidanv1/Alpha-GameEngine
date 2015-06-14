#include "PhysicsComponent.h"
#include "TransformComponent.h"

// -----------------------------------------------------------------------
PhysicsComponent::PhysicsComponent() :
m_gamePhysics(NULL)
{
	m_gamePhysics = IGamePhysics::Get();
}
// -----------------------------------------------------------------------
PhysicsComponent::~PhysicsComponent()
{

}
// -----------------------------------------------------------------------
bool PhysicsComponent::VPostInit()
{
	Matrix4x4 transform(1.0f);
	TransformComponent* trans = dynamic_cast<TransformComponent*>(m_pOwner->GetComponent("Transform"));
	if (trans)
	{
		transform = trans->GetTransform();
	}
	switch (m_properties.m_shape)
	{
	case Sphere:
		m_gamePhysics->VAddSphere(m_properties.m_radius, m_pOwner, m_properties.m_density, m_properties.m_material, transform, m_properties.m_hasLocalInteria);
		break;
	case Plane:
		m_gamePhysics->VAddStaticPlane(m_pOwner, m_properties.m_density, m_properties.m_material, transform, m_properties.m_normal, 0, m_properties.m_hasLocalInteria);
		break;
	case Box:
		m_gamePhysics->VAddBox(m_properties.m_dimensions, m_pOwner, m_properties.m_density, m_properties.m_material, transform, m_properties.m_hasLocalInteria);
		break;
	}
	return true;
}
// -----------------------------------------------------------------------
void PhysicsComponent::VUpdate(float deltaMs)
{
	Matrix4x4 physicsTransform = GetTransform();
	TransformComponent* trans = dynamic_cast<TransformComponent*>(m_pOwner->GetComponent("Transform"));
	if (trans)
	{
		trans->SetTransform(physicsTransform);
	}
}
// -----------------------------------------------------------------------
bool PhysicsComponent::VInitComponent(TiXmlElement* pElement)
{
	TiXmlElement* physicsElement = pElement->FirstChildElement();
	while (physicsElement)
	{
		string value = physicsElement->Value();
		if (value == "PhysicsMaterial")
		{
			m_properties.m_material = physicsElement->GetText();
		}
		if (value == "Density")
		{
			m_properties.m_density = physicsElement->GetText();
		}
		if (value == "Shape")
		{
			string shape = physicsElement->Attribute("type");
			TiXmlElement* shapeElem = physicsElement->FirstChildElement();
			string shapeElemVal = shapeElem->Value();
			if (shapeElemVal == "Properties")
			{
				if (shape == "sphere")
				{
					m_properties.m_shape = Sphere;
					shapeElem->QueryFloatAttribute("radius", &m_properties.m_radius);
				}
				if (shape == "plane")
				{
					m_properties.m_shape = Plane;
					shapeElem->QueryFloatAttribute("nX", &m_properties.m_normal.x);
					shapeElem->QueryFloatAttribute("nY", &m_properties.m_normal.y);
					shapeElem->QueryFloatAttribute("nZ", &m_properties.m_normal.z);
					m_properties.m_normal = normalize(m_properties.m_normal);
				}
				if (shape == "box")
				{
					m_properties.m_shape = Box;
					shapeElem->QueryFloatAttribute("halfX", &m_properties.m_dimensions.x);
					shapeElem->QueryFloatAttribute("halfY", &m_properties.m_dimensions.y);
					shapeElem->QueryFloatAttribute("halfZ", &m_properties.m_dimensions.z);
				}
				shapeElem->QueryBoolAttribute("hasLocalInertia", &m_properties.m_hasLocalInteria);
			}
			else
			{
				ALPHA_ERROR("Physics shape properties not found.");
			}
			
		}
		physicsElement = physicsElement->NextSiblingElement();
	}
	return true;
}
// -----------------------------------------------------------------------
Matrix4x4 PhysicsComponent::GetTransform()
{
	return m_gamePhysics->VGetRigidBodyTransform(m_pOwner->GetID());
}
// -----------------------------------------------------------------------
void PhysicsComponent::SetTransform(Matrix4x4& transform)
{
	m_gamePhysics->VSetRigidBodyTransform(m_pOwner->GetID(), transform);
}