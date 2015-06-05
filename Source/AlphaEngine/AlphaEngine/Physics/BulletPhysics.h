#pragma once
#include "IGamePhysics.h"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

//========================================================================
struct MaterialData
{
	float m_restitution;
	float m_friction;

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
class BulletPhysics : public IGamePhysics
{
	// these are all of the objects that Bullet uses to do its work.
	btDynamicsWorld* m_dynamicsWorld;
	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher;
	btConstraintSolver* m_solver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	// tables read from the XML
	typedef map<string, float> DensityTable;
	typedef map<string, MaterialData> MaterialTable;
	DensityTable m_densityTable;
	MaterialTable m_materialTable;
	void LoadXml();
	float LookupSpecificGravity(const string& densityStr);
	MaterialData LookupMaterialData(const string& materialStr);
	// keep track of the existing rigid bodies: To check them for updates
	// to the actors’ positions, and to remove them when their lives are over.
	typedef map<ActorID, btRigidBody*> ActorIDToBulletRigidBodyMap;
	ActorIDToBulletRigidBodyMap m_ActorIDToRigidBody;
	btRigidBody * FindBulletRigidBody(ActorID id) const;
	// also keep a map to get the actor id from the btRigidBody*
	typedef map<btRigidBody const *, ActorID>BulletRigidBodyToActorIDMap;
	BulletRigidBodyToActorIDMap m_rigidBodyToActorID;
	ActorID FindActorID(btRigidBody const *) const;
	// Data used to store which collision pair (bodies that are touching) need
	// Collision events sent. When a new pair of touching bodies are
	// detected, they are added to m_previousTickCollisionPairs and an event
	// is sent. When the pair is no longer detected, they are removed
	// and another event is sent.
	typedef pair< btRigidBody const *, btRigidBody const * > CollisionPair;
	typedef set< CollisionPair > CollisionPairs;
	CollisionPairs m_previousTickCollisionPairs;
	// helpers for sending events relating to collision pairs
	void SendCollisionPairAddEvent(btPersistentManifold const * manifold, btRigidBody const * body0, btRigidBody const * body1);
	void SendCollisionPairRemoveEvent(btRigidBody const * body0, btRigidBody const * body1);
	// common functionality used by VAddSphere, VAddBox, etc
	void AddShape(StrongActorPtr pGameActor, btCollisionShape* shape, float mass, const string& physicsMaterial);
	// helper for cleaning up objects
	void RemoveCollisionObject(btCollisionObject * removeMe);
	// callback from bullet for each physics time step. set in VInitialize
	static void BulletInternalTickCallback(
		btDynamicsWorld * const world, btScalar const timeStep);
public:
	// Initialization and Maintenance of the Physics World
	virtual bool VInitialize() override;
	virtual void VOnUpdate(float deltaSeconds) override;
	virtual void VSyncVisibleScene() override;
	// Initialization of Physics Objects
	virtual void VAddSphere(float radius, Actor* actor, const mat4& initialTransform, const string& densityStr, const string& physicsMaterial) override;
	virtual void VAddBox(const vec3& dimensions, Actor* gameActor, const mat4& initialTransform, const string& densityStr, const string& physicsMaterial) override;
	virtual void VRemoveActor(ActorID id) override;
	// Debugging
	virtual void VRenderDiagnostics() override;
	// Physics world modifiers
	virtual void VCreateTrigger(Actor* gameActor, const vec3 &pos, const float dim) override;
	virtual void VApplyForce(const vec3 &dir, float newtons, ActorID aid) override;
	virtual void VApplyTorque(const vec3 &dir, float newtons, ActorID aid) override;
	virtual bool VKinematicMove(const mat4 &mat, ActorID aid) override;

	virtual void VRotateY(ActorID ActorID, float angleRadians, float time);
	virtual float VGetOrientationY(ActorID ActorID);
	virtual void VStopActor(ActorID ActorID);
	virtual vec3 VGetVelocity(ActorID ActorID);
	virtual void VSetVelocity(ActorID ActorID, const vec3& vel);
	virtual vec3 VGetAngularVelocity(ActorID ActorID);
	virtual void VSetAngularVelocity(ActorID ActorID, const vec3& vel);
	virtual void VTranslate(ActorID ActorID, const vec3& vec);
	virtual void VSetTransform(const ActorID id, const mat4& mat);
	virtual mat4 VGetTransform(const ActorID id);

};
//========================================================================