#ifndef SCENE_H
#define SCENE_H
#include "RootNode.h"
#include "Camera.h"

class Scene;
typedef shared_ptr<RootNode> StrongRootNodePtr;
typedef shared_ptr<SceneNode> StrongNodePtr;
typedef shared_ptr<Camera> StrongCameraNodePtr;
typedef shared_ptr<Scene> StrongScenePtr;
//========================================================================
class Scene
{
public:
	Scene();
	~Scene();
	bool Init();
	//updates scene graph
	void Update(float deltaMs);
	void SetCameraNode(StrongCameraNodePtr m_cameraNode);
	//renders scene graph
	void Render();
	//loads all scene nodes
	bool Load();
	StrongCameraNodePtr GetCamera() const { return m_cameraNode; }
	void AddChild(StrongNodePtr child);
private:
	StrongRootNodePtr m_rootNode;
	StrongCameraNodePtr m_cameraNode;
};
//========================================================================
#endif