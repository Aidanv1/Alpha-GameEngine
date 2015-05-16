#include "SceneNode.h"
// -----------------------------------------------------------------------
SceneNode::SceneNode() :
m_positionInWorld(0),
m_nodeProperties(),
m_radius(0)
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