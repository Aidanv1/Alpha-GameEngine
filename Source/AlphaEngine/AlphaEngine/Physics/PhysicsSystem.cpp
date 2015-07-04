#include "PhysicsSystem.h"
#include "BulletPhysics\BulletPhysics.h"
#include "..\ResourceManager\Loaders\MeshResourceLoader.h"
#include "..\ResourceManager\Loaders\HeightFieldResourceLoader.h"
#include "PhysicsEvents.h"
#include "../EventManager/EventManager.h"
// -----------------------------------------------------------------------
PhysicsSystem::PhysicsSystem() :
m_gamePhysics(NULL),
m_colMeshResourceManager(NULL)
{

}
// -----------------------------------------------------------------------
PhysicsSystem::~PhysicsSystem()
{

}
// -----------------------------------------------------------------------
bool PhysicsSystem::Init(int colMeshResSize)
{
	m_gamePhysics = StrongPhysicsPtr(ALPHA_NEW BulletPhysics());
	if (!m_gamePhysics->VInitPhysics())
	{
		return false;
	}
	
	m_colMeshResourceManager = StrongResourceManagerPtr(ALPHA_NEW ResourceManager("CollisionRM"));
	if (!m_colMeshResourceManager->Init(colMeshResSize))
	{
		return false;
	}	

	m_colMeshResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(ALPHA_NEW DAEResourceLoader()));
	m_colMeshResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(ALPHA_NEW FBXResourceLoader()));
	m_colMeshResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(ALPHA_NEW ThreeDSResourceLoader()));
	m_colMeshResourceManager->AddResourceLoader(shared_ptr<IResourceLoader>(ALPHA_NEW HeightFieldResourceLoader()));
	return true;
}
// -----------------------------------------------------------------------
bool PhysicsSystem::LoadPhysics()
{
	int passes = 0;
	do
	{
		m_colMeshResourceManager->Update();
		passes++;
	} while (	!m_gamePhysics->VLoadCollisionMeshes() &&
				passes < 5);
	bool check = passes < 5;
	ALPHA_ASSERT(check);
	Queue_Event(ALPHA_NEW PhysicsLoadedEvent());
	return true;
}
// -----------------------------------------------------------------------
PhysicsSystem& PhysicsSystem::Get()
{
	static PhysicsSystem* physics = ALPHA_NEW PhysicsSystem();
	return *physics;
}
// -----------------------------------------------------------------------
ResourceManager* PhysicsSystem::GetCollisionMeshManager()
{
	return m_colMeshResourceManager.get();
}