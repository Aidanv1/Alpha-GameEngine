#pragma once
#include "RootNode.h"
#include "CameraNode.h"
#include "LightManager.h"
#include "SceneNode.h"
class Scene;
class IRenderer;
//========================================================================
typedef shared_ptr<RootNode> StrongRootNodePtr;
typedef shared_ptr<CameraNode> StrongCameraNodePtr;
typedef shared_ptr<Scene> StrongScenePtr;
//========================================================================
class TransformStack
{
public:
	TransformStack();
	~TransformStack();
	void Push(mat4& transform);
	void Push(Matrix4x4& transform);
	void Pop();
	int Size() const;
	Matrix4x4 Top();
private:
	list<Matrix4x4> m_stack;
};

//========================================================================
class Scene
{
public:
	Scene();
	~Scene();
	bool Init(IRenderer* renderer);
	//updates scene graph
	void Update(float deltaMs);
	void SetCameraNode(StrongCameraNodePtr m_cameraNode);
	//renders scene graph
	void Render();
	//loads all scene nodes
	bool Load();
	//Camera
	StrongCameraNodePtr GetCamera() const { return m_cameraNode; }
	void AddChild(StrongSceneNodePtr child);
	//Transparency functions
	void AddTransparentNode(ISceneNode* node);
	bool isAlphaPass() const { return m_isAlphaPass; }
	//Lighting
	void GetLightsInScene(WeakLightArray& lights) const { m_lightManager.GetLights(lights); }
	void AddLightToScene(StrongLightPtr light);
	void RemoveLighFromScene(LightNode* light);
	//Transform stack
	TransformStack* Stack() { return &m_worldTransformStack; }

private:
	void RenderTransparentNodes();//alpha pass
	void SortTransparentNodes();
private:
	IRenderer*			m_renderer;
	StrongRootNodePtr	m_rootNode;
	StrongCameraNodePtr m_cameraNode;
	list<ISceneNode*>	m_transparentSceneNodes;
	bool				m_isAlphaPass;
	LightManager		m_lightManager;
	TransformStack		m_worldTransformStack;
};
//========================================================================
