#pragma once
#include "../../AlphaStd.h"
#include "../../Maths/LinearAlgebra.h"
#include "bullet/btBulletDynamicsCommon.h"
//========================================================================
class BulletMathsHelper
{
public:
	static btVector3 ConvertFromVec3(vec3& vector);
	static vec3 ConvertToVec3(btVector3& vector);
	static btTransform ConvertFromMat4(Matrix4x4& matrix);
	static Matrix4x4 ConvertToMat4(btTransform& trans);
};
//========================================================================