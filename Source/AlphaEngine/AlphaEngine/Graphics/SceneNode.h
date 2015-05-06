#ifndef SCENENODE_H
#define SCENENODE_H
#include"..\AlphaStd.h"
class SceneNode;
typedef shared_ptr<SceneNode> StrongSceneNodePtr;
typedef list<StrongSceneNodePtr> SceneNodeList;
//========================================================================
struct NodeProperties
{
	mat4 ModelMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 RotationMatrix;
	vec4 LightVector;
	NodeProperties()
	{
		ModelMatrix = mat4(0);
		ViewMatrix = mat4(0);
		ProjectionMatrix = mat4(0);
		RotationMatrix = mat4(0);
		LightVector = vec4(0);
	}
};
//========================================================================
class ISceneNode
{
public:
	virtual void VRender() = 0;
	virtual void VAddChild(StrongSceneNodePtr sceneNode) = 0;
protected:
	virtual void VRenderChildren()= 0;
};
//========================================================================
class SceneNode: public ISceneNode
{
public:
	SceneNode();
	~SceneNode();
	NodeProperties GetNodeProperties() const { return m_nodeProperties; }
	void SetNodeProperties(NodeProperties &nodeProperties);
	virtual void VRender() override {}
	virtual void VAddChild(StrongSceneNodePtr sceneNode);
protected:
	virtual void VRenderChildren();
protected:
	vec3 m_positionInWorld;
	vec3 m_rotationAxis;
	float  m_rotationAngle;
	NodeProperties m_nodeProperties;
	SceneNodeList m_children;
};
//========================================================================
#endif