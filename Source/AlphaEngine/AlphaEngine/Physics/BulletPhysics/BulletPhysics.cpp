#include "BulletPhysics.h"
#include "../PhysicsSystem.h"
#include "../../ResourceManager/Resources/Model.h"
#include "../../ResourceManager/Resources/Bitmap.h"
#include "BulletMathsHelper.h"
#include "../PhysicsEvents.h"
#include "../../EventManager/EventManager.h"
// -----------------------------------------------------------------------
BulletPhysics::BulletPhysics()
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
	m_dynamicsWorld->setWorldUserInfo(this);
	return true;
}
// -----------------------------------------------------------------------
void BulletPhysics::VUpdate(float deltaMs)
{
	//update physics
	//fixed time step accomodates for up to 240 fps
	//THIS MUST BE CHANGED TO MATCH THE FPS LOCK!!!
	m_dynamicsWorld->stepSimulation(deltaMs / MS_PER_SECOND, 4, 1.0/240.0);
}
// -----------------------------------------------------------------------
void BulletPhysics::InternalTickCallBack(btDynamicsWorld * const world, btScalar const timeStep)
{
	BulletPhysics * const bulletPhysics = static_cast<BulletPhysics*>(world->getWorldUserInfo());
	btDispatcher * const dispatcher = world->getDispatcher();
	CollisionPairs currentTickCollisionPairs;
	//Check for NEW Collisions
	for (int manifoldIdx = 0; manifoldIdx < dispatcher->getNumManifolds(); ++manifoldIdx)
	{
		btPersistentManifold const * const manifold = dispatcher->getManifoldByIndexInternal(manifoldIdx);
		int numContacts = manifold->getNumContacts();
		if (numContacts > 0)
		{
			const btRigidBody* body0 = dynamic_cast< const btRigidBody* >(manifold->getBody0());
			const btRigidBody* body1 = dynamic_cast< const btRigidBody* >(manifold->getBody1());
			//ensure consistent order
			if (body0 > body1)
			{
				const btRigidBody* temp = body1;
				body1 = body0;
				body0 = temp;
			}
			//find average contact information
			//---
			vec3 contactPoint0(0.0);
			vec3 contactNormal0(0.0);
			vec3 contactPoint1(0.0);
			vec3 contactNormal1(0.0);
			//iterate through all contact points
			for (int i = 0; i < numContacts; i++)
			{
				contactPoint0 += BulletMathsHelper::ConvertToVec3(manifold->getContactPoint(i).getPositionWorldOnA());
				contactNormal0 += BulletMathsHelper::ConvertToVec3(-manifold->getContactPoint(i).m_normalWorldOnB);

				contactPoint1 += BulletMathsHelper::ConvertToVec3(manifold->getContactPoint(i).getPositionWorldOnB());
				contactNormal1 += BulletMathsHelper::ConvertToVec3(manifold->getContactPoint(i).m_normalWorldOnB);
			}
			contactPoint0 = contactPoint0 / (float)numContacts;
			contactPoint1 = contactPoint1 / (float)numContacts;
			contactNormal0 = contactNormal0 / (float)numContacts;
			contactNormal1 = contactNormal1 / (float)numContacts;
			//---
			//add pair to current set of collisions
			CollisionPair colPair = make_pair(body0, body1);
			currentTickCollisionPairs.insert(colPair);
			if (bulletPhysics->m_previousTickCollisionPairs.find(colPair) ==
				bulletPhysics->m_previousTickCollisionPairs.end())
			{
				CollisionObjectInfo info0(	bulletPhysics->m_actorMap[colPair.first],
											contactPoint0,
											contactNormal0);
				CollisionObjectInfo info1(	bulletPhysics->m_actorMap[colPair.second],
											contactPoint1,
											contactNormal1);
				Queue_Event(ALPHA_NEW NewCollisionEvent(info0, info1));
			}
		}
	}
	//Check for REMOVED Collisions
	CollisionPairs removedPairs;
	set_difference(	currentTickCollisionPairs.begin(),
					currentTickCollisionPairs.end(), 
					bulletPhysics->m_previousTickCollisionPairs.begin(), 
					bulletPhysics->m_previousTickCollisionPairs.end(), 
					inserter(removedPairs, removedPairs.begin()));

	for (auto it = removedPairs.begin(); it != removedPairs.end(); it++)
	{
		Queue_Event(ALPHA_NEW RemovedCollisionEvent(	bulletPhysics->m_actorMap[it->first],
														bulletPhysics->m_actorMap[it->second]));
	}
	bulletPhysics->m_previousTickCollisionPairs = currentTickCollisionPairs;
}
// -----------------------------------------------------------------------
void BulletPhysics::VRemoveShape(ActorID actorId)
{
	m_dynamicsWorld->removeRigidBody(m_rigidBodyMap[actorId]);
	auto findBody = m_rigidBodyMap.find(actorId);
	btRigidBody* body = findBody->second;
	m_rigidBodyMap.erase(findBody);
	auto findActor = m_actorMap.find(body);
	m_actorMap.erase(findActor);
	SAFE_DELETE(body);
}
// -----------------------------------------------------------------------
void BulletPhysics::AddShape(Actor* actor, btCollisionShape* shape, float mass, string material, Matrix4x4& trans, bool hasLocalInteria)
{
	btTransform transform = BulletMathsHelper::ConvertFromMat4(trans);
	btVector3 intertia(0 ,0 , 0);
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
	m_actorMap[rigidBody] = actor->GetID();
}
// -----------------------------------------------------------------------
void BulletPhysics::VAddSphere(float const radius, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria)
{
	btCollisionShape* shape = ALPHA_NEW btSphereShape(radius);
	float volume = 4 / 3 * pi<float>() * radius * radius;
	float mass = volume * LookUpDensity(density);
	AddShape(actor.get(), shape, mass, material, transform, hasLocalInteria);
}
// -----------------------------------------------------------------------
void BulletPhysics::VAddStaticPlane(StrongActorPtr actor, string density, string material, Matrix4x4& transform, vec3 normal, float planeConstant, bool hasLocalInteria)
{
	btCollisionShape* shape = ALPHA_NEW btStaticPlaneShape(BulletMathsHelper::ConvertFromVec3(normal), planeConstant);
	AddShape(actor.get(), shape, 0, material, transform, hasLocalInteria);
}
// -----------------------------------------------------------------------
void BulletPhysics::VAddBox(vec3 dimensions, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria)
{
	btCollisionShape* shape = ALPHA_NEW btBoxShape(BulletMathsHelper::ConvertFromVec3(dimensions));
	float volume = 8*dimensions.x * dimensions.y * dimensions.z; // times each dimension 2 (2 x 2 x 2 = 8) 
	float mass = volume * LookUpDensity(density);
	AddShape(actor.get(), shape, mass, material, transform, hasLocalInteria);
}
// ----------------------------------------------------------------------
void BulletPhysics::VAddMesh(string meshName, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria)
{
	shared_ptr<Resource> colMeshRes(ALPHA_NEW Resource(meshName));
	PhysicsSystem::Get().GetCollisionMeshManager()->AddResource(colMeshRes);
	
	PendingCollisionMesh pendingMesh;
	pendingMesh.m_actor = actor;
	pendingMesh.m_density = LookUpDensity(density);
	pendingMesh.m_hasLocalInteria = hasLocalInteria;
	pendingMesh.m_material = material;
	pendingMesh.m_transform = transform;
	pendingMesh.m_colMeshRes = colMeshRes;
	pendingMesh.m_meshType = Hull;
	m_pendingColMeshQueue.push_back(pendingMesh);
}
// -----------------------------------------------------------------------
void BulletPhysics::VAddHeightField(string meshName, StrongActorPtr actor, string density, string material, Matrix4x4& transform, bool hasLocalInteria)
{
	shared_ptr<Resource> colMeshRes(ALPHA_NEW Resource(meshName));
	PhysicsSystem::Get().GetCollisionMeshManager()->AddResource(colMeshRes);

	PendingCollisionMesh pendingMesh;
	pendingMesh.m_actor = actor;
	pendingMesh.m_density = LookUpDensity(density);
	pendingMesh.m_hasLocalInteria = hasLocalInteria;
	pendingMesh.m_material = material;
	pendingMesh.m_transform = transform;
	pendingMesh.m_colMeshRes = colMeshRes;
	pendingMesh.m_meshType = HeightField;
	m_pendingColMeshQueue.push_back(pendingMesh);
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
	return BulletMathsHelper::ConvertToMat4(transform);
}
// ----------------------------------------------------------------------
void BulletPhysics::VSetRigidBodyTransform(ActorID actorID, Matrix4x4& transform)
{
	m_rigidBodyMap[actorID]->setWorldTransform(BulletMathsHelper::ConvertFromMat4(transform));
}
// ----------------------------------------------------------------------
void BulletPhysics::VRenderDiagnostics()
{
	m_dynamicsWorld->debugDrawWorld();
}
// ----------------------------------------------------------------------
bool BulletPhysics::VLoadCollisionMeshes()
{
	auto it = m_pendingColMeshQueue.begin();
	bool loaded = true;
	while (it != m_pendingColMeshQueue.end())
	{
		if ((*it).m_colMeshRes->Buffer() != NULL)
		{			
			btCollisionShape* shape = NULL;
			PendingCollisionMesh pendingMesh = (*it);
			int size = 0;
			if ((*it).m_meshType == Hull)
			{
				ModelBufferReader mBR = (*it).m_colMeshRes->Buffer();
				int numMeshes = 0;
				MeshInfo* meshInfo = mBR.GetMeshInfoArray(numMeshes);
				shape = ALPHA_NEW btConvexHullShape(meshInfo->m_data, meshInfo->m_numberOfVertices, 12);
			}
			if ((*it).m_meshType == HeightField)
			{
				HeightFieldDataInfo* heightInfo = (HeightFieldDataInfo*)(*it).m_colMeshRes->Buffer();
				int gridSize = sqrt(heightInfo->m_numVertices);
				shape = ALPHA_NEW btHeightfieldTerrainShape(	gridSize,
																gridSize,
																heightInfo->m_data,
																0,
																-heightInfo->m_maxHeight,
																heightInfo->m_maxHeight,
																1, 
																PHY_FLOAT,
							
																false); 
				shape->setLocalScaling(btVector3(heightInfo->m_scaleXZ / (gridSize - 1), 1, heightInfo->m_scaleXZ / (gridSize - 1)));
			}		
		
			// approximate absolute mass using bounding box
			btVector3 aabbMin(0, 0, 0), aabbMax(0, 0, 0);
			shape->getAabb(btTransform::getIdentity(), aabbMin, aabbMax);
			btVector3 const aabbExtents = aabbMax - aabbMin;
			float const volume = aabbExtents.x() * aabbExtents.y() * aabbExtents.z();
			btScalar const mass = volume * (*it).m_density;
			AddShape((*it).m_actor.get(), shape, mass, (*it).m_material, (*it).m_transform, (*it).m_hasLocalInteria);
			auto eraseIt = it;
			it++;
			m_pendingColMeshQueue.erase(eraseIt);
		}
		else
		{
			loaded = false;
			if (it != m_pendingColMeshQueue.end())
			{
				it++;
			}
		}
	} 
	return loaded;
}
// ----------------------------------------------------------------------
void BulletPhysics::VSetVelocity(ActorID actorID, vec3 velocity)
{
	m_rigidBodyMap[actorID]->setLinearVelocity(BulletMathsHelper::ConvertFromVec3(velocity));
}
// ----------------------------------------------------------------------
vec3 BulletPhysics::VGetVelocity(ActorID actorID) const
{
	auto findIt = m_rigidBodyMap.find(actorID);
	const btRigidBody* body = findIt->second;
	btVector3 vec = body->getLinearVelocity();
	return BulletMathsHelper::ConvertToVec3(vec);
}
// ----------------------------------------------------------------------
void BulletPhysics::VSetAngularVelocity(ActorID actorID, vec3 velocity)
{
	m_rigidBodyMap[actorID]->setAngularVelocity(BulletMathsHelper::ConvertFromVec3(velocity));
}
// ----------------------------------------------------------------------
vec3 BulletPhysics::VGetAngularVelocity(ActorID actorID) const
{
	auto findIt = m_rigidBodyMap.find(actorID);
	const btRigidBody* body = findIt->second;
	btVector3 vec = body->getAngularVelocity();
	return BulletMathsHelper::ConvertToVec3(vec);
}
// ----------------------------------------------------------------------
void BulletPhysics::VApplyImpulse(vec3 force, ActorID actorID)
{
	btRigidBody* body = m_rigidBodyMap[actorID];
	body->setActivationState(DISABLE_DEACTIVATION);
	body->applyCentralImpulse(BulletMathsHelper::ConvertFromVec3(force));
}
// ----------------------------------------------------------------------
void BulletPhysics::VApplyTorqueImpulse(vec3 force, ActorID actorID)
{
	btRigidBody* body = m_rigidBodyMap[actorID];
	body->setActivationState(DISABLE_DEACTIVATION);
	body->applyTorqueImpulse(BulletMathsHelper::ConvertFromVec3(force));
}
// ----------------------------------------------------------------------