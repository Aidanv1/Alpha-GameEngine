#include "BulletMathsHelper.h"
// -----------------------------------------------------------------------
btVector3 BulletMathsHelper::ConvertFromVec3(vec3& vector)
{
	return btVector3(vector.x, vector.y, vector.z);
}
// -----------------------------------------------------------------------
vec3 BulletMathsHelper::ConvertToVec3(btVector3& vector)
{
	return vec3(vector.x(), vector.y(), vector.z());
}
// -----------------------------------------------------------------------
btTransform BulletMathsHelper::ConvertFromMat4(Matrix4x4& matrix)
{
	btTransform t;
	mat4 glmMat = *matrix.Get();
	btScalar* matrixArray = value_ptr(glmMat);
	t.setFromOpenGLMatrix(matrixArray);
	return t;
}
// ----------------------------------------------------------------------
Matrix4x4 BulletMathsHelper::ConvertToMat4(btTransform& trans)
{
	btScalar matrix[16];
	trans.getOpenGLMatrix(matrix);
	matrix[15] = 1.0f;
	mat4 glmMat;
	glmMat = make_mat4(matrix);
	return glmMat;
}
