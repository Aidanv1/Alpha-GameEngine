#pragma once
#include "../../AlphaStd.h"
#include "../Scene/SceneNode.h"
#include "../Scene/DrawableNode.h"
//========================================================================
struct VectorPair
{
	VectorPair() :
		m_from(0.0),
		m_to(0.0)
	{
	}

	VectorPair(vec3& from, vec3& to) :
		m_from(from),
		m_to(to)
	{
	}
	vec3 m_from;
	vec3 m_to;
};
//========================================================================
class IGeometry
{
public:
	virtual bool VInside(vec3 point) const = 0;
	virtual bool VInside(vec3 point, float radius) const = 0;
	virtual void VTransform(Matrix4x4& trans) = 0;
	virtual void VDraw(vec4& colour){};
};
class DrawableGeometry : public IGeometry
{
public:
	virtual void VDraw(vec4& colour);
	void VTransform(Matrix4x4& trans);
protected:
	vector < VectorPair >  m_lines;
	Matrix4x4 m_transform;
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
	void VTransform(Matrix4x4& trans) override;
private:
	string m_name;
	vec3 m_point;
	vec3 m_pointWorld;
	vec3 m_normal;
	vec3 m_normalWorld;
};
//========================================================================
class Frustum : public DrawableGeometry
{
public:
	Frustum(float height, float width, float fov, float near, float far);
	Frustum();
	~Frustum();
	bool VInside(vec3 point) const override;
	bool VInside(vec3 point, float radius) const override;	
	void VTransform(Matrix4x4& trans) override;
private:
	StrongSceneNodePtr m_lineMesh;
	Plane m_planes[6];
};
//========================================================================
class Sphere : public DrawableGeometry
{
public:
	Sphere(vec3 position, float radius, int accuracy = 10);
	Sphere();
	~Sphere();
	bool VInside(vec3 point) const override;
	bool VInside(vec3 point, float radius) const override;
	void VTransform(Matrix4x4& trans) override;
private:
	StrongSceneNodePtr m_lineMesh;
	float m_radius;
	vec3 m_position;
};
//========================================================================
class Cone : public DrawableGeometry
{
public:
	Cone(vec3 position, float radius, float length, int accuracy = 10);
	Cone();
	~Cone();
	bool VInside(vec3 point) const override;
	bool VInside(vec3 point, float radius) const override;
	void VTransform(Matrix4x4& trans) override;
private:
	StrongSceneNodePtr m_lineMesh;
	float m_radius;
	vec3 m_position;
	float m_length;

};
//========================================================================