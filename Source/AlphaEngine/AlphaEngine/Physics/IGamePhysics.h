#pragma once
#include "../AlphaStd.h"
#include "../Actor/Actor.h"
#include "../Maths/LinearAlgebra.h"
class btCollisionShape;
//========================================================================
class IGamePhysics
{
	static IGamePhysics* globalGamePhysics;
public:
	IGamePhysics(bool isGlobal);
	~IGamePhysics();
	static IGamePhysics* Get();
	virtual bool VInitPhysics() = 0;
	virtual bool VConfigureXmlData(TiXmlElement* pElement) = 0;
	virtual void VUpdate(float deltaMs) = 0;
	virtual void VAddShape(Actor* actor, btCollisionShape* shape, float mass, string material, Matrix4x4& transform, bool hasLocalInteria) = 0;
	virtual void VAddSphere(float const radius, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria = false) = 0;
	virtual void VAddBox(vec3 dimensions, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria = false) = 0;
	virtual void VAddStaticPlane(StrongActorPtr actor, string density, string material, Matrix4x4& transform, vec3 normal, float planeConstant, bool hasLocalInteria = false) = 0;
	//Accessor
	virtual Matrix4x4 VGetRigidBodyTransform(ActorID actorID) = 0;
	//Mutator
	virtual void VSetRigidBodyTransform(ActorID actorID, Matrix4x4& transform) = 0;
	//Debugging
	virtual void VRenderDiagnostics() = 0;
};
//========================================================================