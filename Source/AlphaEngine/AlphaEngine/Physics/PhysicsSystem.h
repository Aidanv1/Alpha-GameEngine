#pragma once
#include "IGamePhysics.h"
#include "../ResourceManager/ResourceManager.h"
//========================================================================
typedef shared_ptr<IGamePhysics> StrongPhysicsPtr;
//========================================================================
class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();
	bool Init(int colMeshResSize);
	bool LoadPhysics();
	IGamePhysics* RigidBodyPhysics() { return m_gamePhysics.get(); }
	static PhysicsSystem& Get();
	ResourceManager* GetCollisionMeshManager();
private:
	StrongPhysicsPtr			m_gamePhysics;
	StrongResourceManagerPtr	m_colMeshResourceManager;
};
//========================================================================