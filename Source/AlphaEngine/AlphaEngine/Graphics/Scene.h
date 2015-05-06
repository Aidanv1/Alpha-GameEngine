#ifndef SCENE_H
#define SCENE_H
#include "SceneNode.h"

class Scene;
typedef shared_ptr<ISceneNode> StrongNodePtr;
typedef shared_ptr<Scene> StrongScenePtr;
//========================================================================
class Scene
{
public:
	Scene();
	~Scene();
	void SetRootNode(StrongNodePtr node);
	StrongNodePtr GetRootNode() const { return m_rootNode; };
private:
	StrongNodePtr m_rootNode;
};
//========================================================================
#endif