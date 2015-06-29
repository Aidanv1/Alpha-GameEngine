#include "PhysicsSystem.h"
#include "BulletPhysics\BulletPhysics.h"
#include "..\ResourceManager\Loaders\MeshResourceLoader.h"
#include "..\ResourceManager\Loaders\HeightFieldResourceLoader.h"
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
	do
	{
		m_colMeshResourceManager->Update();
	} while (!m_gamePhysics->VLoadCollisionMeshes());
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