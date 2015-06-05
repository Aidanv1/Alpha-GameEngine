#pragma once
#include "../AlphaStd.h"
#include "../Actor/Actor.h"
//========================================================================
class IGamePhysics
{
public:
	// Initialization and Maintenance of the Physics World
	virtual bool VInitialize() = 0;
	virtual void VOnUpdate(float deltaSeconds) = 0;
	virtual void VSyncVisibleScene() = 0;
	// Initialization of Physics Objects
	virtual void VAddSphere(float radius, Actor* actor, const mat4& initialTransform, const string& densityStr, const string& physicsMaterial) = 0;
	virtual void VAddBox(const vec3& dimensions, Actor* gameActor, const mat4& initialTransform, const std::string& densityStr, const std::string& physicsMaterial) = 0;
	virtual void VRemoveActor(ActorID id) = 0;
	// Debugging
	virtual void VRenderDiagnostics() = 0;
	// Physics world modifiers
	virtual void VCreateTrigger(Actor* gameActor, const vec3 &pos, const float dim) = 0;
	virtual void VApplyForce(const vec3 &dir, float newtons, ActorID aid) = 0;
	virtual void VApplyTorque(const vec3 &dir, float newtons, ActorID aid) = 0;
	virtual bool VKinematicMove(const mat4 &mat, ActorID aid) = 0;
};
//========================================================================