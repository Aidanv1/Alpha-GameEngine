#include "LinearAlgebra.h"
// -----------------------------------------------------------------------
Matrix4x4::Matrix4x4(float numVal)
{
	m_matrix = numVal * mat4(1.0f);
}
// -----------------------------------------------------------------------
Matrix4x4::Matrix4x4(mat4& matrix) :
m_matrix(matrix)
{
}
// -----------------------------------------------------------------------
void Matrix4x4::Set(mat4& matrix)
{
	m_matrix = matrix;
}
// -----------------------------------------------------------------------
mat4x4* Matrix4x4::Get()
{
	return &m_matrix;
}
// -----------------------------------------------------------------------
void Matrix4x4::SetPosition(vec3 position)
{
	m_matrix[3].x = position.x;
	m_matrix[3].y = position.y;
	m_matrix[3].z = position.z;
	m_matrix[3].w = 1;
}
// -----------------------------------------------------------------------
vec3 Matrix4x4::GetPosition() const
{
	vec3 position;
	position.x = m_matrix[3].x;
	position.y = m_matrix[3].y;
	position.z = m_matrix[3].z;
	return position;
}
// -----------------------------------------------------------------------
void Matrix4x4::SetRotation(vec3 rotation)
{
	//rotate identity
	mat4 rot = rotate(mat4(1.0f), rotation.x, vec3(1.0f, 0, 0));
	rot = rotate(rot, rotation.y, vec3(0, 1.0f, 0));
	rot = rotate(rot, rotation.z, vec3(0, 0, 1.0f));
	m_matrix[0] = rot[0];
	m_matrix[1] = rot[1];
	m_matrix[2] = rot[2];
}
// -----------------------------------------------------------------------
mat4 Matrix4x4::GetRotation() const
{
	mat4 rotation(1.0f);
	rotation[0] = m_matrix[0];
	rotation[1] = m_matrix[1];
	rotation[2] = m_matrix[2];
	return rotation;
}
// -----------------------------------------------------------------------
vec3 Matrix4x4::GetLookAtVector()
{
	vec4 vectorPos = GetRotation() * vec4(0, 0, 1.0f, 0);
	return vec3(vectorPos);
}
// -----------------------------------------------------------------------
vec4 Matrix4x4::GetQuaternion()
{
	quat q = quat_cast(GetRotation());

	return vec4(q.x, q.y, q.z, q.w);
}
// -----------------------------------------------------------------------
mat4 Matrix4x4::operator*(mat4& matrix)
{
	return m_matrix * matrix;
}
// -----------------------------------------------------------------------
mat4 Matrix4x4::operator*(Matrix4x4& matrix)
{
	return m_matrix * matrix.m_matrix;
}
// -----------------------------------------------------------------------
vec4 Matrix4x4::operator*(vec4& vector)
{
	return m_matrix*vector;
}
// -----------------------------------------------------------------------
mat4 Matrix4x4::operator+(mat4& matrix)
{
	return m_matrix + matrix;
}
// -----------------------------------------------------------------------
mat4 Matrix4x4::operator+(Matrix4x4& matrix)
{
	return m_matrix + matrix.m_matrix;
}
// -----------------------------------------------------------------------
mat4 Matrix4x4::operator-(mat4& matrix)
{
	return m_matrix - matrix;
}
// -----------------------------------------------------------------------
mat4 Matrix4x4::operator-(Matrix4x4& matrix)
{
	return m_matrix - matrix.m_matrix;
}
// -----------------------------------------------------------------------
mat4 Matrix4x4::operator/(mat4& matrix)
{
	return m_matrix / matrix;
}
// -----------------------------------------------------------------------
mat4 Matrix4x4::operator/(Matrix4x4& matrix)
{
	return m_matrix / matrix.m_matrix;
}
// -----------------------------------------------------------------------
void Matrix4x4::operator=(mat4& matrix)
{
	m_matrix = matrix;
}
// -----------------------------------------------------------------------
void Matrix4x4::operator=(const mat4& matrix)
{
	m_matrix = matrix;
}
// -----------------------------------------------------------------------