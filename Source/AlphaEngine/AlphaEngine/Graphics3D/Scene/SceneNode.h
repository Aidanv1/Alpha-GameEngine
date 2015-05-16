#ifndef SCENENODE_H
#define SCENENODE_H
#include"../..\AlphaStd.h"
#include "../../Actor/Actor.h"

class Scene;
class SceneNode;
typedef shared_ptr<SceneNode> StrongSceneNodePtr;
typedef list<StrongSceneNodePtr> SceneNodeList;
//========================================================================
struct NodeProperties
{
	ActorID m_ActorID;	
	string m_name;
	mat4 m_toWorld;
	mat4 m_rotationMatrix;
	vec4 m_lightVector;

	NodeProperties() :
		m_ActorID(-1),
		m_name(""),
		m_toWorld(0),
		m_rotationMatrix(0),
		m_lightVector(0)
	{

	}
};

//========================================================================
class ISceneNode
{
public:
	virtual void VRender(Scene* pScene) = 0;
	virtual bool VInitNode() = 0;
	virtual void VAddChild(StrongSceneNodePtr sceneNode) = 0;
	virtual void VUpdateNode(Scene* pScene, float deltaMS) = 0;
protected:
	virtual void VRenderChildren(Scene* pScene) = 0;
};
//========================================================================
class SceneNode: public ISceneNode
{
public:
	SceneNode();
	~SceneNode();
	NodeProperties GetNodeProperties() const { return m_nodeProperties; }
	void SetNodeProperties(NodeProperties &nodeProperties);
	virtual bool VInitNode() override;
	virtual void VUpdateNode(Scene* pScene, float deltaMS) override {}
	virtual void VRender(Scene* pScene) override {}
	virtual void VAddChild(StrongSceneNodePtr sceneNode) override;
	void GetPositionInWorld(vec3& pos) const { pos = m_positionInWorld; }
	void SetPositionInWorld(vec3 pos);
	float GetRadius() const { return m_radius; }
protected:
	virtual void VRenderChildren(Scene* pScene) override;
protected:
	vec3 m_positionInWorld;
	NodeProperties m_nodeProperties;
	SceneNodeList m_children;
	float m_radius;
};
//========================================================================
#endif