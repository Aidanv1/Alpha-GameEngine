#pragma once
#include "../../AlphaStd.h"
#include "../../Maths/LinearAlgebra.h"
#include "bullet/btBulletDynamicsCommon.h"
//========================================================================
class BulletMathsHelper
{
public:
	static btVector3 ConvertFromVec3(const vec3& vector);
	static vec3 ConvertToVec3(const btVector3& vector);
	static btTransform ConvertFromMat4(const Matrix4x4& matrix);
	static Matrix4x4 ConvertToMat4(const btTransform& trans);
};
//========================================================================