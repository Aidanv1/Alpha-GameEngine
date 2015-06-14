#include "DebugDrawer.h"
#include "../../Graphics3D/GraphicsSystem.h"
// -----------------------------------------------------------------------
DebugDrawer::DebugDrawer()
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

}
// -----------------------------------------------------------------------
void DebugDrawer::reportErrorWarning(const char* warningString)
{

}
// -----------------------------------------------------------------------
void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{

}
// -----------------------------------------------------------------------
void DebugDrawer::setDebugMode(int debugMode)
{


}
// -----------------------------------------------------------------------
int DebugDrawer::getDebugMode() const
{
	return DBG_DrawWireframe;
}
// -----------------------------------------------------------------------