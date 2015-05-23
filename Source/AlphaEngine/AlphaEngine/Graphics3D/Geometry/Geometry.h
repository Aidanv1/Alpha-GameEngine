#pragma once
#include "../../AlphaStd.h"
#include "../Scene/SceneNode.h"
//========================================================================
class IGeometry
{
public:
	virtual bool VInside(vec3 point) const = 0;
	virtual bool VInside(vec3 point, float radius) const = 0;
	virtual void VTransform(mat4& trans, mat4& rot) = 0;
};
//========================================================================
class Plane : public IGeometry
{
public:
	Plane();
	Plane(vec3 point1, vec3 point2, vec3 point3, string name = "");
	Plane(vec3 point, vec3 normal, string name = "");
	~Plane();
	bool VInside(vec3 point) const override;
	bool VInside(vec3 point, float radius) const override;
	float DistanceToPlane(vec3 point) const;
	void VTransform(mat4& trans, mat4& rot) override;
private:
	string m_name;
	vec3 m_point;
	vec3 m_pointWorld;
	vec3 m_normal;
	vec3 m_normalWorld;
};
//========================================================================
class Frustum : public IGeometry
{
public:
	Frustum(float height, float width, float fov, float near, float far);
	Frustum();
	~Frustum();
	bool VInside(vec3 point) const override;
	bool VInside(vec3 point, float radius) const override;	
	void VTransform(mat4& trans, mat4& rot) override;
private:
	StrongSceneNodePtr m_lineMesh;
	Plane m_planes[6];
};
//========================================================================