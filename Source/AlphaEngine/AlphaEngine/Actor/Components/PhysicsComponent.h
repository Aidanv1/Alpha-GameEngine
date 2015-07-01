#pragma once
#include "../IActorComponent.h"
#include "../../Physics/IGamePhysics.h"
class Matrix4x4;
//========================================================================
enum ShapeType
{
	Sphere,
	Plane,
	Box,
	Mesh,
	Height
};
struct PhysicsProperties
{
	bool m_hasLocalInteria;
	ShapeType m_shape;
	const char* m_material;
	const char* m_density;
	//sphere
	float m_radius;
	//box
	vec3 m_dimensions;
	//plane
	vec3 m_normal;
	//mesh
	string m_meshName;
};
//========================================================================
class PhysicsComponent : public IActorComponent
{
public:
	PhysicsComponent();
	~PhysicsComponent();
	Matrix4x4 GetTransform();
	void SetTransform(Matrix4x4& transform);
	//Mechanics
	void SetVelocity(vec3& velocity);
	vec3 GetVelocity() const;
	void SetAngularVelocity(vec3& velocity);
	vec3 GetAngularVelocity() const;
	void ApplyImpulseForce(vec3 force);
	void ApplyImpulseTorque(vec3 force);

	//IActorComponent fucnctions
	ComponentType VGetType() const override{ return "Physics"; }
	bool VPostInit() override;
	void VUpdate(float deltaMs) override;
	bool VInitComponent(TiXmlElement* pElement) override;
private:
	PhysicsProperties m_properties;
};
//========================================================================