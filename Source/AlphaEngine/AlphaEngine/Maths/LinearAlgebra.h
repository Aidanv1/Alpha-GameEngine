#pragma once
#include "../AlphaStd.h"
//========================================================================
//Matrix4x4 serves as a simple wrapper class for glm's mat4, adding
//additional functionality
class Matrix4x4
{
public:
	Matrix4x4(float numVal = 0.0f);
	Matrix4x4(mat4& matrix);
	void Set(mat4& matrix);
	mat4x4* Get();
	void SetPosition(vec3 position);
	vec3 GetPosition() const;
	void SetRotation(vec3 rotation);
	mat4 GetRotation() const;
	vec4 GetQuaternion() const;
	vec3 GetLookAtVector() const;
	//operators
	mat4 operator*(mat4& matrix);
	mat4 operator*(Matrix4x4& matrix);
	vec4 operator*(vec4& vector);
	mat4 operator+(mat4& matrix);
	mat4 operator+(Matrix4x4& matrix);
	mat4 operator-(mat4& matrix);
	mat4 operator-(Matrix4x4& matrix);
	mat4 operator/(mat4& matrix);
	mat4 operator/(Matrix4x4& matrix);
	void operator=(mat4& matrix);
	void operator=(const mat4& matrix);
private:
	mat4 m_matrix;
};
//========================================================================