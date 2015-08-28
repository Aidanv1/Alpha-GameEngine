#pragma once
#include "../IActorComponent.h"
#include "../../Maths/LinearAlgebra.h"
//========================================================================
class DLLExport TransformComponent : public IActorComponent
{
public:
	TransformComponent();
	~TransformComponent();
	Matrix4x4 GetTransform() const { return m_transform; }
	void SetTransform(Matrix4x4& transform);
	void SetPosition(vec3 position);
	vec3 GetPosition() const;
	void SetRotation(vec3 rotation);
	Matrix4x4 GetRotation() const;
	vec3 GetLookAt() const;
	//IActorComponent functions
	ComponentType VGetType() const override { return "Transform"; }
	void VUpdate(float deltaMs) override;
	bool VInitComponent(TiXmlElement* pElement) override;
	bool VPostInit() override;

private:
	Matrix4x4 m_transform;
};
//========================================================================