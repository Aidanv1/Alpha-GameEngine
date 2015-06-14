#pragma once
#include "../IGamePhysics.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "bullet/btBulletCollisionCommon.h"
#include "../../Actor/Actor.h"
#include "../../Maths/LinearAlgebra.h"
#include "DebugDrawer.h"
//========================================================================
struct MaterialData
{
	float m_restitution;
	float m_friction;

	MaterialData() :
		m_restitution(0.0),
		m_friction(0.0)
	{
	}
	MaterialData(float restitution, float friction) :
		m_restitution(restitution),
		m_friction(friction)
	{
	}
	MaterialData(const MaterialData& other) :
		m_restitution(other.m_restitution),
		m_friction(other.m_friction)
	{
	}
};
//========================================================================
typedef map<ActorID, btRigidBody*> RigidBodyActorMap;
typedef map<string, float> DensityTable;
typedef map<string, MaterialData> MaterialTable;

//========================================================================
class BulletPhysics : public IGamePhysics
{
		
public:
	BulletPhysics(bool isGlobal);
	~BulletPhysics();
	virtual bool VInitPhysics() override;
	virtual bool VConfigureXmlData(TiXmlElement* pElement) override;
	virtual void VUpdate(float deltaMs) override;
	//Add shape
	virtual void VAddShape(Actor* actor, btCollisionShape* shape, float mass, string material, Matrix4x4& transform, bool hasLocalInteria) override;
	virtual void VAddSphere(float const radius, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria) override;
	virtual void VAddStaticPlane(StrongActorPtr actor, string density, string material, Matrix4x4& transform, vec3 normal, float planeConstant, bool hasLocalInteria) override;
	virtual void VAddBox(vec3 dimensions, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria) override;
	//Accessors
	virtual Matrix4x4 VGetRigidBodyTransform(ActorID actorID) override;
	//Mutator
	virtual void VSetRigidBodyTransform(ActorID actorID, Matrix4x4& transform) override;
	//Debugging
	virtual void VRenderDiagnostics() override;
private:
	static void InternalTickCallBack(btDynamicsWorld * const world, btScalar const timeStep);
	//conversions
	btVector3 ConvertFromVec3(vec3& vector);
	btTransform ConvertFromMat4(Matrix4x4& matrix);
	Matrix4x4 ConvertToMat4(btTransform& trans);
	//Lookups
	float LookUpDensity(string densityID);
	MaterialData LookUpMaterial(string materialID);

private:
	// these are all of the objects that Bullet uses to do its work.
	btDynamicsWorld*					m_dynamicsWorld;
	btBroadphaseInterface*				m_broadphase;
	btCollisionDispatcher*				m_dispatcher;
	btConstraintSolver*					m_solver;
	btDefaultCollisionConfiguration*	m_collisionConfiguration;
	DebugDrawer*						m_debugDrawer;
	//
	RigidBodyActorMap					m_rigidBodyMap;
	DensityTable						m_densityTable;
	MaterialTable						m_materialTable;
};
//========================================================================