#include "BulletMathsHelper.h"
// -----------------------------------------------------------------------
btVector3 BulletMathsHelper::ConvertFromVec3(const vec3& vector)
{
	return btVector3(vector.x, vector.y, vector.z);
}
// -----------------------------------------------------------------------
vec3 BulletMathsHelper::ConvertToVec3(const btVector3& vector)
{
	return vec3(vector.x(), vector.y(), vector.z());
}
// -----------------------------------------------------------------------
btTransform BulletMathsHelper::ConvertFromMat4(const Matrix4x4& matrix)
{
	btTransform t;
	Matrix4x4 temp = matrix;
	mat4 glmMat = *temp.Get();
	btScalar* matrixArray = value_ptr(glmMat);
	t.setFromOpenGLMatrix(matrixArray);
	return t;
}
// ----------------------------------------------------------------------
Matrix4x4 BulletMathsHelper::ConvertToMat4(const btTransform& trans)
{
	btScalar matrix[16];
	trans.getOpenGLMatrix(matrix);
	matrix[15] = 1.0f;
	mat4 glmMat;
	glmMat = make_mat4(matrix);
	return glmMat;
}
