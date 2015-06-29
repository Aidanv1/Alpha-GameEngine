#include "DebugDrawer.h"
#include "../../Graphics3D/GraphicsSystem.h"
/*
DBG_NoDebug = 0,
DBG_DrawWireframe = 1,
DBG_DrawAabb = 2,
DBG_DrawFeaturesText = 4,
DBG_DrawContactPoints = 8,
DBG_NoDeactivation = 16,
DBG_NoHelpText = 32,
DBG_DrawText = 64,
DBG_ProfileTimings = 128,
DBG_EnableSatComparison = 256,
DBG_DisableBulletLCP = 512,
DBG_EnableCCD = 1024,
DBG_DrawConstraints = (1 << 11),
DBG_DrawConstraintLimits = (1 << 12),
DBG_FastWireframe = (1 << 13),
DBG_DrawNormals = (1 << 14),
DBG_DrawFrames = (1 << 15),
DBG_MAX_DEBUG_DRAW_MODE */
// -----------------------------------------------------------------------
DebugDrawer::DebugDrawer() :
m_debugMode(DBG_NoDebug) //DBG_DrawWireframe
{	
}
// -----------------------------------------------------------------------
bool DebugDrawer::Init()
{
	return true;
}
// -----------------------------------------------------------------------
DebugDrawer::~DebugDrawer()
{

}
// -----------------------------------------------------------------------
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& colour)
{
	vec3 from1(from.x(), from.y(), from.z());
	vec3 to1(to.x(), to.y(), to.z());
	vec4 colour1(colour.x(), colour.y(), colour.z(), 1.0);
	GraphicsSystem::Get().GetRenderer()->VDrawLine(from1, to1, colour1);
}
// -----------------------------------------------------------------------
void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& colour)
{
	vec3 point(PointOnB.x(), PointOnB.y(), PointOnB.z());
	vec4 colour1(colour.x(), colour.y(), colour.z(), 1.0);
	vec3 normal(normalOnB.x(), normalOnB.y(), normalOnB.z());
	GraphicsSystem::Get().GetRenderer()->VDrawPoint(point, distance, colour1);
	GraphicsSystem::Get().GetRenderer()->VDrawLine(point, point + normal, colour1);
}
// -----------------------------------------------------------------------
void DebugDrawer::reportErrorWarning(const char* warningString)
{
	ALPHA_WARNING(warningString);
}
// -----------------------------------------------------------------------
void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{

}
// -----------------------------------------------------------------------
void DebugDrawer::setDebugMode(int debugMode)
{
	switch (debugMode)
	{
	case 0:
		m_debugMode = DBG_NoDebug;
		break;
	case 1:
		m_debugMode = DBG_DrawWireframe;
		break;
	case 2:
		m_debugMode = DBG_DrawContactPoints;
		break;
	case 3:
		m_debugMode = DBG_MAX_DEBUG_DRAW_MODE;
		break;
	}
}
// -----------------------------------------------------------------------
int DebugDrawer::getDebugMode() const
{
	return m_debugMode;
}
// -----------------------------------------------------------------------