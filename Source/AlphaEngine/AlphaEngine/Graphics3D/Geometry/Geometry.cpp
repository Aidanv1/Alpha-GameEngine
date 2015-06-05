#include "Geometry.h"
#include "../OpenGL/DrawableNodes/LineMesh_GL.h"
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
	float dotProd = dot(normalize(-vectorToPlane), m_normalWorld);
	return dotProd < 0;
}
// -----------------------------------------------------------------------
bool Plane::VInside(vec3 point, float radius) const
{
	vec3 vectorToPlane = m_pointWorld - point;
	float dotProd = dot(normalize(-vectorToPlane), m_normalWorld);
	if (dotProd < 0 || DistanceToPlane(point) < radius)
	{
		return true;
	}
	return false;
}
// -----------------------------------------------------------------------
float Plane::DistanceToPlane(vec3 point) const
{
	vec3 vectorFromPlane = point - m_pointWorld;
	float hypot = length(vectorFromPlane);
	float cosineTheta = dot(vectorFromPlane, m_normalWorld);
	return abs(cosineTheta*hypot);
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
	float aspectRatio = width / height;
	float focalLength = (height / 2) / tan(fovDegreesY/2);
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

	float height2 = (farClip) * tan(fovDegreesY/2)*2;
	float width2 = (farClip)* tan(fovDegreesX)*2;
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
	DrawableNodeFactory nodeFactory;
	ILineMesh* lineMesh = dynamic_cast<ILineMesh*>(nodeFactory.CreateDrawableNode(Node_LineMesh));
	ALPHA_ASSERT(lineMesh);
	lineMesh->VInitLineMesh();
	lineMesh->VLoadLineMesh(vertexArray, 24);
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
	//mat4 offset = translate(mat4(1.0f), vec3(0, 0, -2));
	///m_lineMesh->VSetTransform(trans * rot *offset);
	m_lineMesh->VSetTransform(trans * rot);
}
//========================================================================
Sphere::Sphere() :
m_radius(0),
m_position(0)
{

}
// -----------------------------------------------------------------------
Sphere::Sphere(vec3 position, float radius, int accuracy) :
m_radius(radius),
m_position(position)
{
	vector <float> vertices;
	float revolution = 2 * pi < float >() ;
	float startingAngle = -pi < float >() / 2;
	vec3 previous;
	vec3 start;
	int count = 0;
	//Horizontal
	for (int i = 1; i < accuracy-1; i++)
	{
		for (int j = 0; j < accuracy; j++)
		{
			float phi = startingAngle + ((revolution / accuracy)* i);
			float theta = startingAngle + ((revolution / accuracy)* j);
			float x = radius * cos(phi) * cos(theta);
			float z = radius * cos(phi) * sin(theta);
			float y = radius * sin(phi);


			if (j !=0)
			{
				vertices.push_back(previous.x);
				vertices.push_back(previous.y);
				vertices.push_back(previous.z);
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
				count++;
			}
			else
			{
				start = vec3(x, y, z);
			}
			count++;
			previous = vec3(x, y, z);
		}
		vertices.push_back(previous.x);
		vertices.push_back(previous.y);
		vertices.push_back(previous.z);
		vertices.push_back(start.x);
		vertices.push_back(start.y);
		vertices.push_back(start.z);
		count++;
	}
	//--
	//vertical
	for (int i = 1; i < accuracy - 1; i++)
	{
		for (int j = 0; j < accuracy; j++)
		{
			float phi = startingAngle + ((revolution / accuracy)* j);
			float theta = startingAngle + ((revolution / accuracy)* i);
			float x = radius * cos(phi) * cos(theta);
			float z = radius * cos(phi) * sin(theta);
			float y = radius * sin(phi);


			if (j != 0)
			{
				vertices.push_back(previous.x);
				vertices.push_back(previous.y);
				vertices.push_back(previous.z);
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
				count++;
			}
			else
			{
				start = vec3(x, y, z);
			}
			count++;
			previous = vec3(x, y, z);
		}
		vertices.push_back(previous.x);
		vertices.push_back(previous.y);
		vertices.push_back(previous.z);
		vertices.push_back(start.x);
		vertices.push_back(start.y);
		vertices.push_back(start.z);
		count++;
	}
	//--
	int numvert = 4 * accuracy *(accuracy - 2);
	DrawableNodeFactory nodeFactory;
	ILineMesh* lineMesh = dynamic_cast<ILineMesh*>(nodeFactory.CreateDrawableNode(Node_LineMesh));
	ALPHA_ASSERT(lineMesh);
	lineMesh->VInitLineMesh();
	float* vertexArray = &vertices[0];
	lineMesh->VLoadLineMesh(vertexArray, numvert);
	m_lineMesh = StrongSceneNodePtr(lineMesh);
	GraphicsSystem::Get().GetScene()->AddChild(m_lineMesh);
}
// -----------------------------------------------------------------------
Sphere::~Sphere()
{

}
// -----------------------------------------------------------------------
bool Sphere::VInside(vec3 point) const
{
	float distance = length(point - m_position);
	if (distance < m_radius)
	{
		return true;
	}
	return false;
}
// -----------------------------------------------------------------------
bool Sphere::VInside(vec3 point, float radius) const
{
	float distance = length(point - m_position) - radius;
	if (distance < m_radius)
	{
		return true;
	}
	return false;
}
// -----------------------------------------------------------------------
void Sphere::VTransform(mat4& trans, mat4& rot)
{
	mat4 rt = trans*rot;
	vec4 point = rt * vec4(1.0);
	m_position = vec3(point);
	m_lineMesh->VSetTransform(trans * rot);
}
//========================================================================
Cone::Cone() :
m_radius(0),
m_position(0),
m_length(0)
{

}
// -----------------------------------------------------------------------
Cone::Cone(vec3 position, float radius, float length, int accuracy) :
m_radius(radius),
m_position(position),
m_length(length)
{
	vector <float> vertices;
	float revolution = 2 * pi < float >();
	float startingAngle = -pi < float >() / 2;
	vec3 previous;
	vec3 point(0.0, 0.0, m_length);
	vec3 start;
	int count = 0;
	//vertical
	for (int j = 0; j < accuracy; j++)
	{
		float phi = startingAngle + ((revolution / accuracy)* j);
		float theta = 0;
		float x = radius * cos(phi) * cos(theta);
		float z = radius * cos(phi) * sin(theta);
		float y = radius * sin(phi);


		if (j != 0)
		{
			//circle
			vertices.push_back(previous.x);
			vertices.push_back(previous.y);
			vertices.push_back(previous.z);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			count++;
		}
		else
		{
			start = vec3(x, y, z);
		}
		//cone side
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		vertices.push_back(point.x);
		vertices.push_back(point.y);
		vertices.push_back(point.z);
		count++;
		previous = vec3(x, y, z);

	}
	vertices.push_back(previous.x);
	vertices.push_back(previous.y);
	vertices.push_back(previous.z);
	vertices.push_back(start.x);
	vertices.push_back(start.y);
	vertices.push_back(start.z);

	
	//--
	int numvert = 4 * (accuracy);
	DrawableNodeFactory nodeFactory;
	ILineMesh* lineMesh = dynamic_cast<ILineMesh*>(nodeFactory.CreateDrawableNode(Node_LineMesh));
	ALPHA_ASSERT(lineMesh);
	lineMesh->VInitLineMesh();
	float* vertexArray = &vertices[0];
	lineMesh->VLoadLineMesh(vertexArray, numvert);
	m_lineMesh = StrongSceneNodePtr(lineMesh);
	GraphicsSystem::Get().GetScene()->AddChild(m_lineMesh);
}
// -----------------------------------------------------------------------
Cone::~Cone()
{

}
// -----------------------------------------------------------------------
bool Cone::VInside(vec3 point) const
{
	//Todo: Add functionality
	return false;
}
// -----------------------------------------------------------------------
bool Cone::VInside(vec3 point, float radius) const
{
	//Todo: Add functionality
	return false;
}
// -----------------------------------------------------------------------
void Cone::VTransform(mat4& trans, mat4& rot)
{
	mat4 rt = trans*rot;
	vec4 point = rt * vec4(1.0);
	m_position = vec3(point);
	m_lineMesh->VSetTransform(trans * rot);
}
//========================================================================