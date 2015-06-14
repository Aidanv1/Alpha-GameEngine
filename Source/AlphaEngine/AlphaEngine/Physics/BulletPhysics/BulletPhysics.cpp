#include "BulletPhysics.h"

// -----------------------------------------------------------------------
BulletPhysics::BulletPhysics(bool isGlobal) :
IGamePhysics(isGlobal)
{

}
// -----------------------------------------------------------------------
BulletPhysics::~BulletPhysics()
{
	SAFE_DELETE(m_dynamicsWorld);
	SAFE_DELETE(m_broadphase);
	SAFE_DELETE(m_dispatcher);
	SAFE_DELETE(m_collisionConfiguration);
	SAFE_DELETE(m_debugDrawer);
}
// -----------------------------------------------------------------------
bool BulletPhysics::VInitPhysics()
{
	m_collisionConfiguration = ALPHA_NEW btDefaultCollisionConfiguration();
	m_dispatcher = ALPHA_NEW btCollisionDispatcher(m_collisionConfiguration);
	m_broadphase = ALPHA_NEW btDbvtBroadphase();
	m_solver = ALPHA_NEW btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = ALPHA_NEW btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	m_debugDrawer = ALPHA_NEW DebugDrawer();
	//set gravity
	m_dynamicsWorld->setGravity(btVector3(0.0, -9.8, 0.0));
	//callback for collision events
	m_dynamicsWorld->setInternalTickCallback(InternalTickCallBack);
	m_dynamicsWorld->setDebugDrawer(m_debugDrawer);
	return true;
}
// -----------------------------------------------------------------------
void BulletPhysics::VUpdate(float deltaMs)
{
	//update physics
	m_dynamicsWorld->stepSimulation(deltaMs, 4);
}
// -----------------------------------------------------------------------
void BulletPhysics::InternalTickCallBack(btDynamicsWorld * const world, btScalar const timeStep)
{

}
// -----------------------------------------------------------------------
void BulletPhysics::VAddShape(Actor* actor, btCollisionShape* shape, float mass, string material, Matrix4x4& trans, bool hasLocalInteria)
{
	btTransform transform = ConvertFromMat4(trans);
	btVector3 intertia(0,0,0);
	if (hasLocalInteria)
	{
		shape->calculateLocalInertia(mass, intertia);
	}
	//Motion state
	btMotionState* motionState = ALPHA_NEW btDefaultMotionState(transform);
	//rigid body info
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, intertia);
	//material
	MaterialData mData = LookUpMaterial(material);
	info.m_friction = mData.m_friction;
	info.m_restitution = mData.m_restitution;
	//rigid body
	btRigidBody* rigidBody = ALPHA_NEW btRigidBody(info);
	m_dynamicsWorld->addRigidBody(rigidBody);
	//add to map
	m_rigidBodyMap[actor->GetID()] = rigidBody;
}
// -----------------------------------------------------------------------
void BulletPhysics::VAddSphere(float const radius, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria)
{
	btCollisionShape* shape = ALPHA_NEW btSphereShape(radius);
	float volume = 4 / 3 * pi<float>() * radius * radius;
	float mass = volume * LookUpDensity(density);
	VAddShape(actor.get(), shape, mass, material, transform, hasLocalInteria);
}
// -----------------------------------------------------------------------
void BulletPhysics::VAddStaticPlane(StrongActorPtr actor, string density, string material, Matrix4x4& transform, vec3 normal, float planeConstant, bool hasLocalInteria)
{
	btCollisionShape* shape = ALPHA_NEW btStaticPlaneShape(ConvertFromVec3(normal), planeConstant);
	VAddShape(actor.get(), shape, 0, material, transform, hasLocalInteria);
}
// -----------------------------------------------------------------------
void BulletPhysics::VAddBox(vec3 dimensions, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria)
{
	btCollisionShape* shape = ALPHA_NEW btBoxShape(ConvertFromVec3(dimensions));
	float volume = 8*dimensions.x * dimensions.y * dimensions.z; // times each dimension 2 (2 x 2 x 2 = 8) 
	float mass = volume * LookUpDensity(density);
	VAddShape(actor.get(), shape, mass, material, transform, hasLocalInteria);
}
// -----------------------------------------------------------------------
btVector3 BulletPhysics::ConvertFromVec3(vec3& vector)
{
	return btVector3(vector.x, vector.y, vector.z);
}
// -----------------------------------------------------------------------
btTransform BulletPhysics::ConvertFromMat4(Matrix4x4& matrix)
{
	btTransform t;
	mat4 glmMat = *matrix.Get();
	btScalar* matrixArray = value_ptr(glmMat);
	t.setFromOpenGLMatrix(matrixArray);
	return t;
}
// ----------------------------------------------------------------------
Matrix4x4 BulletPhysics::ConvertToMat4(btTransform& trans)
{
	btScalar matrix[16];
	trans.getOpenGLMatrix(matrix);
	matrix[15] = 1.0f;
	mat4 glmMat;
	glmMat = make_mat4(matrix);
	return glmMat;
}
// ----------------------------------------------------------------------
float BulletPhysics::LookUpDensity(string densityID)
{
	return m_densityTable[densityID];
}
// ----------------------------------------------------------------------
MaterialData BulletPhysics::LookUpMaterial(string materialID)
{
	return m_materialTable[materialID];
}
// ----------------------------------------------------------------------
bool BulletPhysics::VConfigureXmlData(TiXmlElement* pElement)
{
	TiXmlElement* nextElem = pElement->FirstChildElement();

	while (nextElem)
	{
		string val = nextElem->Value();
		if (val == "MaterialList")
		{
			TiXmlElement* materialElem = nextElem->FirstChildElement();
			while (materialElem)
			{
				string materialName = materialElem->Value();
				float friction = 0;
				float restitution = 0;
				materialElem->QueryFloatAttribute("friction", &friction);
				materialElem->QueryFloatAttribute("restitution", &restitution);
				//add material to table
				m_materialTable[materialName].m_friction = friction;
				m_materialTable[materialName].m_restitution = restitution;
				//next material
				materialElem = materialElem->NextSiblingElement();
			}
		}
		if (val == "DensityTable")
		{
			TiXmlElement* densityElem = nextElem->FirstChildElement();
			while (densityElem)
			{
				string densityName = densityElem->Value();
				string densityValueStr = densityElem->GetText();
				float densityValue = stof(densityValueStr);
				m_densityTable[densityName] = densityValue;
				densityElem = densityElem->NextSiblingElement();
			}
		}
		nextElem = nextElem->NextSiblingElement();
	}

	return true;
}
// ----------------------------------------------------------------------
Matrix4x4 BulletPhysics::VGetRigidBodyTransform(ActorID actorID)
{
	btTransform transform = m_rigidBodyMap[actorID]->getWorldTransform();
	return ConvertToMat4(transform);
}
// ----------------------------------------------------------------------
void BulletPhysics::VSetRigidBodyTransform(ActorID actorID, Matrix4x4& transform)
{
	m_rigidBodyMap[actorID]->setWorldTransform(ConvertFromMat4(transform));
}
// ----------------------------------------------------------------------
void BulletPhysics::VRenderDiagnostics()
{
	m_dynamicsWorld->debugDrawWorld();
}