#pragma once
#include "../IActorComponent.h"
#include "../../Physics/IGamePhysics.h"
class Matrix4x4;
//========================================================================
enum ShapeType
{
	Sphere,
	Plane,
	Box
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
};
//========================================================================
class PhysicsComponent : public IActorComponent
{
public:
	PhysicsComponent();
	~PhysicsComponent();
	Matrix4x4 GetTransform();
	void SetTransform(Matrix4x4& transform);
	//IActorComponent fucnctions
	virtual ComponentType VGetType() const override{ return "Physics"; }
	virtual bool VPostInit() override;
	virtual void VUpdate(float deltaMs) override;
	virtual bool VInitComponent(TiXmlElement* pElement) override;
private:
	IGamePhysics* m_gamePhysics;
	PhysicsProperties m_properties;
};
//========================================================================