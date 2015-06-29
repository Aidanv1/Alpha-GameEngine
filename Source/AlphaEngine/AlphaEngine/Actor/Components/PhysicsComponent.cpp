#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "../../Physics/PhysicsSystem.h"

// -----------------------------------------------------------------------
PhysicsComponent::PhysicsComponent()
{
	
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
		PhysicsSystem::Get().RigidBodyPhysics()->VAddSphere(m_properties.m_radius, m_pOwner, m_properties.m_density, m_properties.m_material, transform, m_properties.m_hasLocalInteria);
		break;
	case Plane:
		PhysicsSystem::Get().RigidBodyPhysics()->VAddStaticPlane(m_pOwner, m_properties.m_density, m_properties.m_material, transform, m_properties.m_normal, 0, m_properties.m_hasLocalInteria);
		break;
	case Box:
		PhysicsSystem::Get().RigidBodyPhysics()->VAddBox(m_properties.m_dimensions, m_pOwner, m_properties.m_density, m_properties.m_material, transform, m_properties.m_hasLocalInteria);
		break;
	case Mesh:
		PhysicsSystem::Get().RigidBodyPhysics()->VAddMesh(m_properties.m_meshName, m_pOwner, m_properties.m_density, m_properties.m_material, transform, m_properties.m_hasLocalInteria);
		break;
	case Height:
		PhysicsSystem::Get().RigidBodyPhysics()->VAddHeightField(m_properties.m_meshName, m_pOwner, m_properties.m_density, m_properties.m_material, transform, m_properties.m_hasLocalInteria);
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
				if (shape == "mesh")
				{
					m_properties.m_shape = Mesh;
					m_properties.m_meshName = shapeElem->Attribute("MeshName");
				}
				if (shape == "height")
				{
					m_properties.m_shape = Height;
					m_properties.m_meshName = shapeElem->Attribute("MeshName");
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
	return PhysicsSystem::Get().RigidBodyPhysics()->VGetRigidBodyTransform(m_pOwner->GetID());
}
// -----------------------------------------------------------------------
void PhysicsComponent::SetTransform(Matrix4x4& transform)
{
	PhysicsSystem::Get().RigidBodyPhysics()->VSetRigidBodyTransform(m_pOwner->GetID(), transform);
}