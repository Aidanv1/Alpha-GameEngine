#include "SceneNode.h"
// -----------------------------------------------------------------------
SceneNode::SceneNode() :
m_drawable(NULL),
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