#include "SceneNode.h"
// -----------------------------------------------------------------------
SceneNode::SceneNode() :
m_positionInWorld(0),
m_rotationInWorld(0),
m_nodeProperties()
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
void SceneNode::VRenderChildren()
{
	//iterate through all children and call their render function
	for (auto child = m_children.begin(); child != m_children.end(); child++)
	{
		(*child)->VRender();
	}
}

void SceneNode::AddChild(StrongSceneNodePtr sceneNode)
{
	m_children.push_back(sceneNode);
}