#include "SceneNode.h"
#include "Scene.h"
// -----------------------------------------------------------------------
SceneNode::SceneNode() :
m_positionInWorld(0),
m_nodeProperties(),
m_radius(0),
m_isVisible(true),
m_screenZ(0)
{
}
// -----------------------------------------------------------------------
SceneNode::SceneNode(string name) :
m_positionInWorld(0),
m_nodeProperties(),
m_radius(0),
m_name(name),
m_screenZ(0)
{
}
// -----------------------------------------------------------------------
SceneNode::~SceneNode()
{

}

// -----------------------------------------------------------------------
void SceneNode::SetNodeProperties(NodeProperties& nodeProperties)
{
	m_nodeProperties = nodeProperties;
}
// -----------------------------------------------------------------------
void SceneNode::VRender(Scene* pScene)
{
	VRenderChildren(pScene);	
}
// -----------------------------------------------------------------------
void SceneNode::VRenderChildren(Scene* pScene)
{
	//iterate through all children and call their render function
	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		(*child)->VRender(pScene);
	}
}
// -----------------------------------------------------------------------
void SceneNode::VAddChild(StrongSceneNodePtr sceneNode)
{
	m_children.push_back(sceneNode);
}
// -----------------------------------------------------------------------
void SceneNode::SetPositionInWorld(vec3& pos)
{
	m_positionInWorld = pos;
}
// -----------------------------------------------------------------------
void SceneNode::SetRotationInWorld(vec3& rot)
{
	m_rotationInWorld = rot;
}
// -----------------------------------------------------------------------
bool SceneNode::VInitNode()
{
	bool success = true;
	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		if (!(*child)->VInitNode())
		{
			success = false;
		}
	}
	return success;
}
// -----------------------------------------------------------------------
void SceneNode::VUpdateNode(Scene* pScene, float deltaMS)
{
	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		(*child)->VUpdateNode(pScene, deltaMS);
	}
}
// -----------------------------------------------------------------------