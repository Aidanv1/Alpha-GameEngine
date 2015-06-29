#pragma once
#include "../IGamePhysics.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "../../Actor/Actor.h"
#include "../../Maths/LinearAlgebra.h"
#include "DebugDrawer.h"

struct MeshInfo;
class Resource;
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
enum CollisionMeshType
{
	Hull,
	HeightField
};
struct HeightFieldInfo
{
	int			m_heightStickWidth;
	int			m_heightStickLength;
	btScalar	m_heightScale;
	btScalar	m_minHeight;
	btScalar	m_maxHeight;

	HeightFieldInfo() :
		m_heightStickWidth(256),
		m_heightStickLength(256),
		m_heightScale(1),
		m_minHeight(0),
		m_maxHeight(100)
	{
	}
};
struct PendingCollisionMesh
{
	shared_ptr<Resource>	m_colMeshRes;
	StrongActorPtr			m_actor;
	float					m_density;
	string					m_material;
	Matrix4x4				m_transform;
	bool					m_hasLocalInteria;
	CollisionMeshType		m_meshType;
	HeightFieldInfo			m_heightFieldInfo;
	PendingCollisionMesh() :
		m_colMeshRes(NULL),
		m_actor(NULL),
		m_density(0),
		m_material(""),
		m_transform(0),
		m_hasLocalInteria(false),
		m_meshType(Hull)
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
	BulletPhysics();
	~BulletPhysics();
	virtual bool VInitPhysics() override;
	virtual bool VConfigureXmlData(TiXmlElement* pElement) override;
	virtual void VUpdate(float deltaMs) override;
	//Add shape
	virtual void AddShape(Actor* actor, btCollisionShape* shape, float mass, string material, Matrix4x4& transform, bool hasLocalInteria);
	virtual void VAddSphere(float const radius, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria) override;
	virtual void VAddStaticPlane(StrongActorPtr actor, string density, string material, Matrix4x4& transform, vec3 normal, float planeConstant, bool hasLocalInteria) override;
	virtual void VAddBox(vec3 dimensions, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria) override;
	virtual void VAddHeightField(string meshName, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria) override;
	virtual void VAddMesh(string meshName, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria) override;
	//Accessors
	virtual Matrix4x4 VGetRigidBodyTransform(ActorID actorID) override;
	//Mutator
	virtual void VSetRigidBodyTransform(ActorID actorID, Matrix4x4& transform) override;
	//Debugging
	virtual void VRenderDiagnostics() override;
	//Load meshes
	virtual bool VLoadCollisionMeshes() override;
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
	// Collision Meshes
	list<PendingCollisionMesh>			m_pendingColMeshQueue;
};
//========================================================================