#include "Geometry.h"
#include "../OpenGL/Data/LineMesh.h"
//========================================================================
Plane::Plane(vec3 point1, vec3 point2, vec3 point3, string name) :
m_name(name)
{
	m_point = point1;
	m_pointWorld = m_point;
	vec3 vector1 = point2 - point1;
	vec3 vector2 = point3 - point1;
	m_normal = normalize(cross(vector1, vector2));
	m_normalWorld = m_normal;
}
// -----------------------------------------------------------------------
Plane::Plane(vec3 point, vec3 normal, string name) :
m_name(name)
{
	m_point = point;
	normal = m_normal;
	m_pointWorld = m_point;
	m_normalWorld = m_normal;
}
// -----------------------------------------------------------------------
Plane::Plane() :
m_normal(0),
m_point(0),
m_pointWorld(0),
m_normalWorld(0)
{
}
// -----------------------------------------------------------------------
Plane::~Plane()
{

}
// -----------------------------------------------------------------------
bool Plane::VInside(vec3 point) const
{
	vec3 vectorToPlane = m_pointWorld - point;
	float dotProd = dot(-vectorToPlane, m_normalWorld);
	return dotProd < 1;
}
// -----------------------------------------------------------------------
bool Plane::VInside(vec3 point, float radius) const
{
	vec3 vectorToPlane = m_pointWorld - point;
	float distance = DistanceToPlane(point);
	return	dot(normalize(vectorToPlane), m_normalWorld) < 1 &&
			distance >= radius;
}
// -----------------------------------------------------------------------
float Plane::DistanceToPlane(vec3 point) const
{
	vec3 vectorFromPlane = point - m_pointWorld;
	float hypot = length(vectorFromPlane);
	float cosineTheta = dot(vectorFromPlane, m_normalWorld);
	return cosineTheta*hypot;
}
// -----------------------------------------------------------------------
void Plane::VTransform(mat4& trans, mat4& rot)
{
	m_normalWorld = vec3(rot * vec4(m_normal, 1.0f));
	mat4 rt = trans*rot;
	vec4 point = rt * vec4(m_point, 1.0f);
	m_pointWorld = vec3(point);
}
//========================================================================
Frustum::Frustum(float height, float width, float fovDegreesY, float nearClip, float farClip)
{
	float focalLength = (height / 2) / tan(fovDegreesY);
	float fovDegreesX = atan((width / 2) / focalLength);
	//close points
	//top left
	vec3 p1(-width / 2, height / 2, nearClip);
	//top right
	vec3 p2(width / 2, height / 2, nearClip);
	//bottom left
	vec3 p3(-width / 2, -height / 2, nearClip);
	//bottom left
	vec3 p4(width / 2, -height / 2, nearClip);

	float height2 = farClip * tan(fovDegreesY);
	float width2 = farClip * tan(fovDegreesX);
	//farClip points
	//top left
	vec3 p1_2(-width2 / 2, height2 / 2, -farClip);
	//top right
	vec3 p2_2(width2 / 2, height2 / 2, -farClip);
	//bottom left
	vec3 p3_2(-width2 / 2, -height2 / 2, -farClip);
	//bottom left
	vec3 p4_2(width2 / 2, -height2 / 2, -farClip);
	//construct planes
	//front plane
	m_planes[0] = Plane(p1, p3, p4, "front");
	//farClip plane
	m_planes[1] = Plane(p2_2, p4_2, p3_2, "farClip");
	//left plane
	m_planes[2] = Plane(p1, p1_2, p3_2, "left");
	//right plane
	m_planes[3] = Plane(p2, p4, p4_2, "right");
	//bottom plane
	m_planes[4] = Plane(p3, p3_2, p4_2, "bottom");
	//top plane
	m_planes[5] = Plane(p1, p2, p2_2, "top");

	float vertexArray[] =
	{
		//front face
		p1.x, p1.y, p1.z,
		p2.x, p2.y, p2.z,
		//
		p2.x, p2.y, p2.z,
		p4.x, p4.y, p4.z,
		//
		p4.x, p4.y, p4.z,
		p3.x, p3.y, p3.z,
		//
		p3.x, p3.y, p3.z,
		p1.x, p1.y, p1.z,
		//farClip face
		p1_2.x, p1_2.y, p1_2.z,
		p2_2.x, p2_2.y, p2_2.z,
		//
		p2_2.x, p2_2.y, p2_2.z,
		p4_2.x, p4_2.y, p4_2.z,
		//
		p4_2.x, p4_2.y, p4_2.z,
		p3_2.x, p3_2.y, p3_2.z,
		//
		p3_2.x, p3_2.y, p3_2.z,
		p1_2.x, p1_2.y, p1_2.z,
		//connecting lines
		p1_2.x, p1_2.y, p1_2.z,
		p1.x, p1.y, p1.z,
		//
		p2_2.x, p2_2.y, p2_2.z,
		p2.x, p2.y, p2.z,
		//
		p4_2.x, p4_2.y, p4_2.z,
		p4.x, p4.y, p4.z,
		//
		p3_2.x, p3_2.y, p3_2.z,
		p3.x, p3.y, p3.z


	};

	LineMesh* lineMesh = ALPHA_NEW LineMesh();
	lineMesh->Init();
	lineMesh->LoadLineMesh(vertexArray, 24);
	m_lineMesh = StrongSceneNodePtr(lineMesh);
	GraphicsSystem::Get().GetScene()->AddChild(m_lineMesh);
}
// -----------------------------------------------------------------------
Frustum::Frustum()
{
}
// -----------------------------------------------------------------------
Frustum::~Frustum()
{

}
// -----------------------------------------------------------------------
bool Frustum::VInside(vec3 point) const
{
	for (int i = 0; i<6; i ++)
	{
		Plane p = m_planes[i];
		if (!m_planes[i].VInside(point))
		{
			return false;
		}
	}
	return true;
}
// -----------------------------------------------------------------------
bool Frustum::VInside(vec3 point, float radius) const
{
	for (int i = 0; i<6; i++)
	{
		if (!m_planes[i].VInside(point, radius))
		{
			return false;
		}
	}
	return true;
}
// -----------------------------------------------------------------------
void Frustum::VTransform(mat4& trans, mat4& rot)
{
	for (int i = 0; i < 6; i++)
	{
		m_planes[i].VTransform(trans, rot);
	}
	//mat4 offset = translate(mat4(1.0f), vec3(0, 0, -1));
	//m_lineMesh->VSetTransform(trans * rot *offset);
	m_lineMesh->VSetTransform(trans * rot);
}
//========================================================================