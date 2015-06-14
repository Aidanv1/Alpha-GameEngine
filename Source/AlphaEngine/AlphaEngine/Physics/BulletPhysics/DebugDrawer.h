#pragma once
#include "bullet/btBulletDynamicsCommon.h"
#include "bullet/btBulletCollisionCommon.h"
#include <GL/glew.h>
//========================================================================
class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer();
	~DebugDrawer();
	bool Init();
	// btIDebugDraw interface
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& colour) override;
	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
	virtual void reportErrorWarning(const char* warningString) override;
	virtual void draw3dText(const btVector3& location, const char* textString) override;
	virtual void setDebugMode(int debugMode) override;
	virtual int getDebugMode() const override;
private:
};
//========================================================================