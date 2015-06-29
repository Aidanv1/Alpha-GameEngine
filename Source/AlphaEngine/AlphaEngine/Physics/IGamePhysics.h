#pragma once
#include "../AlphaStd.h"
#include "../Actor/Actor.h"
#include "../Maths/LinearAlgebra.h"
//========================================================================
class IGamePhysics
{
public:
	virtual bool VInitPhysics() = 0;
	virtual bool VConfigureXmlData(TiXmlElement* pElement) = 0;
	virtual void VUpdate(float deltaMs) = 0;
	virtual void VAddSphere(float const radius, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria = false) = 0;
	virtual void VAddBox(vec3 dimensions, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria = false) = 0;
	virtual void VAddMesh(string meshName, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria = false) = 0;
	virtual void VAddHeightField(string meshName, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria = false) = 0;
	virtual void VAddStaticPlane(StrongActorPtr actor, string density, string material, Matrix4x4& transform, vec3 normal, float planeConstant, bool hasLocalInteria = false) = 0;
	//Accessor
	virtual Matrix4x4 VGetRigidBodyTransform(ActorID actorID) = 0;
	//Mutator
	virtual void VSetRigidBodyTransform(ActorID actorID, Matrix4x4& transform) = 0;
	//Debugging
	virtual void VRenderDiagnostics() = 0;
	//Loading
	virtual bool VLoadCollisionMeshes() = 0;
};
//========================================================================